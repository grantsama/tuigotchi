#define _XOPEN_SOURCE_EXTENDED 1
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <getopt.h>
#include "data.h"
#include "gotchi.h"
#include "render.h"

int main(int argc, char *argv[]) {
    char *custom_save_path = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "hvs:")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: tuigotchi [OPTIONS]\n\n");
                printf("Options:\n");
                printf("  -h          Show this help message and exit\n");
                printf("  -v          Show version information\n");
                printf("  -s <file>   Run program from a specified save file\n");
                return 0;
            case 'v':
                printf("TuiGotchi v0.91\n");
                return 0;
            case 's':
                custom_save_path = optarg;
                break;
            case '?':
                fprintf(stderr, "Try 'tuigotchi -h' for more information.\n");
                return 1;
        }
    }
    if (optind < argc) {
        fprintf(stderr, "Error: Too many arguments or unknown positionals.\n");
        fprintf(stderr, "Try 'tuigotchi -h' for more information.\n");
        return 1;
    }

    if (setlocale(LC_ALL, "") == NULL) {
        printf("Warning: Could not set locale! Icons may not render correctly.\n");
        sleep(2);
    }

    Gotchi *the_gotchi;
    if (check_save(custom_save_path)) {
        the_gotchi = gotchi_init(false);
        readsave(the_gotchi, custom_save_path);

        // Offline degradation
        int intervals = gotchi_process_offtime(the_gotchi);
        if (intervals > 0) {
            printf("Your pet missed you! (%d days have passed)\n", intervals);
        } else {
            printf("Welcome back, owner of %s!\n", the_gotchi->name);
        }
        sleep(1);
    } else {
        // No save found, start new game flow!
        printf("Welcome to TuiGotchi!\n");
        the_gotchi = gotchi_init(true);
        the_gotchi->animal = get_userpet();
        get_petname(the_gotchi->name, sizeof(the_gotchi->name));
    }

    // Ncurses setup
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(1000);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_CYAN, -1);

    /* --------------------- GAME LOOP ---------------------- */
    int ch;
    int tick_counter = 0; // NEW: Track seconds passed for live stat decay
    render_ui(the_gotchi); // Draw initial state before waiting for input
    while ((ch = getch()) != 'q') {
        if (the_gotchi->isDead) {
            if (ch == 'r') {
                // Delete the save file using the path logic from data.c
                char *saveFileDir = init_savefile_dir(custom_save_path);
                remove(saveFileDir); // remove() deletes a file from the hard drive
                free(saveFileDir);

                // End ncurses and exit early so we don't save the dead pet again!
                endwin();
                free(the_gotchi);
                printf("You buried your pet. Run the game again to adopt a new one.\n");
                return 0;
            }
        }
        else {
            int hDiff = 0, mDiff = 0, hungDiff = 0, tDiff = 0, lDiff = 0;
            bool action_taken = true;

            switch(ch) {
                case 'f':  // If feed, decrease hunger
                    hungDiff -= 1;
                    break;
                case 'w':  // If water, decrease thirst
                    tDiff -= 1;
                    break;
                case 'p':  // If play, increase mood
                    mDiff += 1;
                    break;
                case 'c':  // Clean litter
                    lDiff -= 1;
                    break;
                case 'm':  // NEW: Give Medicine
                    the_gotchi->isSick = false;
                    break;
                case ERR:  // This handles no key pressed (1 second passed)
                    action_taken = false;
                    tick_counter++;
                    if (tick_counter >= TICK_RATE) {
                        gotchi_tick(the_gotchi);
                        tick_counter = 0; // Reset timer
                    }
                    break;
                default:
                    action_taken = false;
                    break;
            }

            // Only apply stat manual stat changes if a key was actually pressed
            if (action_taken) {
                gotchi_update(the_gotchi, hDiff, mDiff, hungDiff, tDiff, lDiff);
            }
        }
        render_ui(the_gotchi);
    }
    /* ------------------ END OF GAME LOOP ------------------- */

    // Clean up
    endwin();
    save(the_gotchi, custom_save_path);
    free(the_gotchi);
    return 0;
}
