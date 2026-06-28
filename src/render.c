#include <ncurses.h>
#include <string.h>
#include "render.h"

// Simple ASCII art for the pets
const char* cat_art[] = {
    " /\\_/\\ ",
    "( o.o )",
    " > ^ < "
};

const char* dog_art[] = {
    "  __      _",
    "o'')}____//",
    " `_/      )",
    " (_(_/-(_/"
};

const char* ham_art[] = {
    " (\\ /) ",
    " ( . .) ",
    " c(\")(\")"
};

void render_ui(Gotchi *g) {
    if (g == NULL) return;

    erase(); // Clear the screen for the new frame

    // Get terminal dimensions
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Draw a box border around the terminal
    box(stdscr, 0, 0);

    // Print the Gotchi's name centered at the top
    attron(A_BOLD);
    mvprintw(1, (max_x - strlen(g->name)) / 2, "%s", g->name);
    attroff(A_BOLD);

    // Print Stats on the left side
    mvprintw(3, 2, "Health: %d/%d", g->health, MAX_HEALTH);
    mvprintw(4, 2, "Mood:   %d/%d", g->mood, MAX_MOOD);
    mvprintw(5, 2, "Hunger: %d/%d", g->hunger, TOO_HUNGRY);
    mvprintw(6, 2, "Thirst: %d/%d", g->thirst, TOO_THIRSTY);
    mvprintw(7, 2, "Litter: %d/%d", g->litter, TOO_POOPY);

    // Show sickness status
    if (g->isSick) {
        attron(A_STANDOUT); // Highlight if sick
        mvprintw(9, 2, "STATUS: SICK!");
        attroff(A_STANDOUT);
    } else {
        mvprintw(9, 2, "STATUS: OK");
    }

    // Determine which art to draw based on the animal type
    int art_lines = 0;
    const char** art = NULL;

    if (g->animal == CAT) {
        art = cat_art;
        art_lines = 3;
    } else if (g->animal == DOG) {
        art = dog_art;
        art_lines = 4;
    } else if (g->animal == HAM) {
        art = ham_art;
        art_lines = 3;
    }

    // Draw the ASCII art in the center of the screen
    if (art != NULL) {
        int start_y = max_y / 2 - art_lines / 2;
        for (int i = 0; i < art_lines; i++) {
            int start_x = max_x / 2 - strlen(art[i]) / 2;
            mvprintw(start_y + i, start_x, "%s", art[i]);
        }
    }

    // Draw controls at the bottom
    mvprintw(max_y - 2, 2, "Controls: [f] Feed [w] Water [p] Play  [c] Clean  [q] Quit");

    refresh(); // Push the drawing buffer to the actual terminal screen
}
