#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include "data.h"
#include "gotchi.h"
#include "render.h"

int main(int argc, const char *argv[]) {
    if (argc > 1) {
        perror("Too many arguments\n");
        return 1;
    }

    Gotchi *the_gotchi;
    if (check_save()) {
        the_gotchi = gotchi_init(false);
        readsave(the_gotchi);
        printf("Welcome back, %s!\n", the_gotchi->name);
        sleep(1); // Give the user a moment to see this before ncurses clears the screen
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


    // GAME LOOP
    int ch;
    
    // Draw the initial state before waiting for input
    render_ui(the_gotchi); 
    
    while ((ch = getch()) != 'q') {
        int hDiff = 0, mDiff = 0, hungDiff = 0, tDiff = 0, lDiff = 0;
        switch(ch) {
            case 'f':  // If feed, decrease hunger
                hungDiff -= 1;
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
    
    // Clean up ncurses and save before exiting
    endwin();
    save(the_gotchi);


    // ... free the_gotchi and return ...
    free(the_gotchi);
    return 0;
}
