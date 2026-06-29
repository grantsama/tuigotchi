#define _XOPEN_SOURCE_EXTENDED 1
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <locale.h>
#include "data.h"
#include "gotchi.h"
#include "render.h"

int main(int argc, const char *argv[]) {
    if (setlocale(LC_ALL, "") == NULL) {
        printf("Warning: Could not set locale! Icons may not render correctly.\n");
        sleep(2);
    }

    if (argc > 1) {
        perror("Too many arguments\n");
        return 1;
    }

    Gotchi *the_gotchi;
    if (check_save()) {
        the_gotchi = gotchi_init(false);
        readsave(the_gotchi);

        // --- OFFLINE DEGRADATION ---
        time_t now = time(NULL);
        // difftime returns the difference in seconds
        double seconds_passed = difftime(now, the_gotchi->last_saved); 
        
        // Calculate how many "ticks" have passed. 
        // 3600 seconds = 1 hour. 
        // For testing purposes, you might want to change 3600 to 60 (1 minute) to see it work quickly!
        int intervals = (int)(seconds_passed / 60); 
        
        if (intervals > 0) {
            // Penalize: 0 health diff, negative mood, positive hunger/thirst/litter
            gotchi_update(the_gotchi, 0, -intervals, intervals, intervals, intervals);
            printf("Your pet missed you! (%d minutes passed)\n", intervals);
        } else {
            printf("Welcome back, %s!\n", the_gotchi->name);
        }
        sleep(2); // Give the user 2 seconds to read the message before ncurses takes over

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
    start_color();
    use_default_colors();

    /* --------------------- GAME LOOP ---------------------- */
    int ch;
    // Draw the initial state before waiting for input
    render_ui(the_gotchi); 
    
    while ((ch = getch()) != 'q') {
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
            case 'c':  // Clean
                lDiff -= 1; // Assuming cleaning reduces litter
                break;
            case ERR:  // This handles the timeout(1000) interval where no key is pressed!
                break; 
        }
        // Apply stat changes
        gotchi_update(the_gotchi, hDiff, mDiff, hungDiff, tDiff, lDiff);
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
