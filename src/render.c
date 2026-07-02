#define _XOPEN_SOURCE_EXTENDED 1
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "render.h"

void draw_external_art(int animal, int frame, int max_y, int max_x) {
    char filepath[256];
    const char *home = getenv("HOME");

    // Map the integer animal ID to the prefix of our text files
    const char *animal_names[] = {"cat", "dog", "ham"};

    // Build the full path to the text file
    snprintf(filepath, sizeof(filepath), "%s/.config/tuigotchi/art/%s_%d.txt",
             home, animal_names[animal], frame);

    FILE *f = fopen(filepath, "r");
    if (f == NULL) {
        mvprintw(max_y / 2, max_x / 2 - 10, "Art file not found!");
        return;
    }

    char lines[15][100];
    int line_count = 0;
    int max_width = 0; // NEW: Keep track of the widest line!

    while (fgets(lines[line_count], sizeof(lines[0]), f) && line_count < 15) {
        // Strip the trailing newline
        lines[line_count][strcspn(lines[line_count], "\r\n")] = '\0';

        // NEW: Check if this line is the widest one we've seen so far
        int current_len = strlen(lines[line_count]);
        if (current_len > max_width) {
            max_width = current_len;
        }

        line_count++;
    }
    fclose(f);

    // Calculate the starting position for the ENTIRE block based on max_width
    int start_y = max_y / 2 - line_count / 2;
    int start_x = max_x / 2 - max_width / 2;

    for (int i = 0; i < line_count; i++) {
        // Draw every line starting at the exact same X coordinate!
        mvprintw(start_y + i, start_x, "%s", lines[i]);
    }
}

void draw_stat_bar(int y, int x, const char* label, int val,
                   int max, const char* symbol, int color_pair) {

    mvprintw(y, x, "%-8s", label);
    attron(COLOR_PAIR(color_pair));
    move(y, x + 8);

    for (int i = 0; i < val; i++) {
        printw("%s ", symbol);
    }

    attroff(COLOR_PAIR(color_pair));
}

void render_ui(Gotchi *g) {
    if (g == NULL) return;

    erase();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    box(stdscr, 0, 0);

    // Print the Gotchi's name centered at the top
    attron(A_BOLD);
    mvprintw(1, (max_x - strlen(g->name)) / 2, "%s", g->name);
    attroff(A_BOLD);

    // --- Color Logic ---
    int health_pair = (g->health <= 5) ? 3 : ((g->health <= 12) ? 2 : 1);
    int hunger_pair = (g->hunger >= 10) ? 3 : ((g->hunger >= 5) ? 2 : 1);

    // --- Draw Stat Bars with Nerd Font Symbols ---
    draw_stat_bar(3, 2, "Health:", g->health, 20, "󰋑 ", health_pair);
    draw_stat_bar(4, 2, "Mood:",   g->mood,    5, " ", 4);
    draw_stat_bar(5, 2, "Hunger:", g->hunger, 15, "󱐟 ", hunger_pair);
    draw_stat_bar(6, 2, "Thirst:", g->thirst, 12, "󰖌 ", 4);
    draw_stat_bar(7, 2, "Litter:", g->litter, 10, " ", 2);

    // Show sickness status
    if (g->isSick) {
        attron(COLOR_PAIR(4) | A_STANDOUT);
        mvprintw(9, 2, "STATUS: SICK!");
        attroff(COLOR_PAIR(4) | A_STANDOUT);
    } else {
        mvprintw(9, 2, "STATUS: OK");
    }

    // Determine which animation frame to show (0 or 1) based on time
    int current_frame = time(NULL) % 2;

    // Load and draw the ASCII art from the external text file
    draw_external_art(g->animal, current_frame, max_y, max_x);

    // Draw controls at the bottom
    mvprintw(max_y - 2, 2, "Controls: [f] Feed  [w] Water  [p] Play  [c] Clean  [q] Quit");

    refresh();
}
