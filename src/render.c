#define _XOPEN_SOURCE_EXTENDED 1
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "render.h"

// 2 frames of ASCII art for the Cat (Blinking)
const char* cat_art[2][3] = {
    {
        " /\\_/\\ ",
        "( o.o )",
        " > ^ < "
    },
    {
        " /\\_/\\ ",
        "( -.- )",
        " > ^ < "
    }
};

// 2 frames of ASCII art for the Dog (Tail wagging / Panting)
const char* dog_art[2][4] = {
    {
        "  __      _",
        "o'')}____//",
        " `_/      )",
        " (_(_/-(_/"
    },
    {
        "  __      _",
        "o'')}____/|", // Tail moves up
        " `_/      )",
        " (_(_/-(_/"
    }
};

// 2 frames of ASCII art for the Hamster (Sniffing/Chewing)
const char* ham_art[2][3] = {
    {
        " (\\ /) ",
        " ( . .) ",
        " c(\")(\")"
    },
    {
        " (\\ /) ",
        " ( o.o) ", // Eyes open wider
        " c(\")(\")"
    }
};

void draw_stat_bar(int y, int x, const char* label, int val,
                   int max, const char* symbol) {
// Print the label in default colors
    mvprintw(y, x, "%-8s", label); 
    
    // Turn on the color for the symbols
    //attron(COLOR_PAIR(color_pair));
    
    // Move the cursor to the starting position for the symbols
    move(y, x + 8);
    
    // Print the symbol 'val' times, letting ncurses advance the cursor
    for (int i = 0; i < val; i++) {
        // We add a space after the symbol so ncurses naturally advances
        // past the double-width character!
        printw("%s ", symbol); 
    }
    
    // Turn the color back off
    //attroff(COLOR_PAIR(color_pair));
}

void render_ui(Gotchi *g) {
    if (g == NULL) return;

    erase();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    box(stdscr, 0, 0);

    attron(A_BOLD);
    mvprintw(1, (max_x - strlen(g->name)) / 2, "%s", g->name);
    attroff(A_BOLD);

    // --- Color Logic (Assuming you added init_pair in main.c!) ---
    int health_pair = (g->health <= 5) ? 3 : ((g->health <= 12) ? 2 : 1);
    int hunger_pair = (g->hunger >= 10) ? 3 : ((g->hunger >= 5) ? 2 : 1);
    
    // --- Draw Stat Bars with Nerd Font Symbols ---
    // Feel free to replace the symbols below with whatever Nerd Font icons you prefer!
    draw_stat_bar(3, 2, "Health:", g->health, 20, "󰋑");  // Heart
    draw_stat_bar(4, 2, "Mood:",   g->mood,    5, "");  // Smiley
    draw_stat_bar(5, 2, "Hunger:", g->hunger, 15, "󱐟");  // Food/Drumstick
    draw_stat_bar(6, 2, "Thirst:", g->thirst, 12, "󰖌");  // Water Drop
    draw_stat_bar(7, 2, "Litter:", g->litter, 10, "");  // Poop

    // Show sickness status
    if (g->isSick) {
        attron(COLOR_PAIR(4) | A_STANDOUT);
        mvprintw(9, 2, "STATUS: SICK!");
        attroff(COLOR_PAIR(4) | A_STANDOUT);
    } else {
        mvprintw(9, 2, "STATUS: OK");
    }

    // ... [The rest of your ASCII art logic stays exactly the same!] ...

    int current_frame = time(NULL) % 2;
    // ... art logic ...

    mvprintw(max_y - 2, 2, "Controls: [f] Feed  [w] Water  [p] Play  [c] Clean  [q] Quit");
    refresh();
}
