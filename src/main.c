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

    Gotchi *gotchi;
    if (!check_save()) {
        gotchi = gotchi_init(true);
    } else {
        gotchi = gotchi_init(false);
        readsave(gotchi);
    }

    // Start ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(0);
    start_color();

    // GAME LOOP
    int ch;
    while ((ch = getch()) != 'q') {
        int hDiff = 0, mDiff = 0, hungDiff = 0, tDiff = 0, lDiff = 0;
        switch(ch) {
            case 'f':  // If feed, decrease hunger
                hungDiff -= 1;
                break;
            case 'p':  // If play, increase mood
                mDiff += 1;
                break;
            case 'c':
                lDiff -= gotchi->litter;
                break;
            default:
                break;
        }
        gotchi_update(gotchi, hDiff, mDiff, hungDiff, tDiff, lDiff);
    }

    // Cleanup
    endwin();  // Stop ncurses
    save(gotchi);
    free(gotchi);
    return 0;
}

