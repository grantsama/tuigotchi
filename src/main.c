#define _XOPEN_SOURCE_EXTENDED 1
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include "data.h"
#include "gotchi.h"
#include "render.h"

int main(int argc, const char *argv[]) {
    if (argc > 2) {
        perror("Error: Too many arguments.\n");
        printf("Try 'tuigotchi -h' for more information.\n");
        return 1;
    }
    if ((argc == 2) && (strcmp(argv[1], "-h") == 0)) {
        printf("Arguments:\n");
        printf("savefile : program run from specified save file\n");
        return 0;
    }

    if (setlocale(LC_ALL, "") == NULL) {
        printf("Warning: Could not set locale! Icons may not render correctly.\n");
        sleep(2);
    }

    Gotchi *the_gotchi;
    if (check_save()) {
        the_gotchi = gotchi_init(false);
        readsave(the_gotchi);

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

    // Start ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(1000);

    // Set ncurses colors
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_CYAN, -1);

    /* --------------------- GAME LOOP ---------------------- */
    int ch;
    // Draw the initial state before waiting for input
    render_ui(the_gotchi);

    while ((ch = getch()) != 'q') {
        if (the_gotchi->isDead) {
            if (ch == 'r') {
                // Delete the save file using the path logic from data.c
                char *saveFileDir = init_savefile_dir();
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
                case ERR:  // This handles no key pressed
                    break;
            }
            // Apply stat changes
            gotchi_update(the_gotchi, hDiff, mDiff, hungDiff, tDiff, lDiff);
        }
        // Redraw the screen with the updated stats
        render_ui(the_gotchi);
    }
    /* ------------------ END OF GAME LOOP ------------------- */

    // Clean up
    endwin();
    save(the_gotchi);
    free(the_gotchi);
    return 0;
}
