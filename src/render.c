#define _XOPEN_SOURCE_EXTENDED 1
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "render.h"

void draw_external_art(int animal, int frame, int max_y, int max_x) {
    char filepath[256];
    const char *home = getenv("HOME");

    if (home == NULL) {
        attron(A_BOLD);
        mvprintw(max_y / 2, max_x / 2 - 16, "Error: HOME variable missing!");
        attroff(A_BOLD);
        return; // Exit the function early so it doesn't crash!
    }

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

void draw_stat_bar(int y, int x, const char *label, int val,
                   int max, const char *symbol, int color_pair) {
    // Print the label in default colors
    mvprintw(y, x, "%-8s", label);

    // Turn on the color for the filled symbols
    attron(COLOR_PAIR(color_pair));
    move(y, x + 8);

    // Draw the current value
    for (int i = 0; i < val; i++) {
        printw("%s", symbol);
    }
    attroff(COLOR_PAIR(color_pair));

    // Draw the empty remaining slots as a subtle dot
    for (int i = val; i < max; i++) {
        printw("· ");
    }
}

void render_ui(Gotchi *g) {
    if (g == NULL) return;

    erase();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    box(stdscr, 0, 0);

    // --- DEATH SCREEN ---
    if (g->isDead) {
        attron(A_BOLD);
        // Draw a simple ASCII tombstone in the center
        mvprintw(max_y / 2 - 4, max_x / 2 - 6, "   .---.  ");
        mvprintw(max_y / 2 - 3, max_x / 2 - 6, " / R.I.P \\ ");
        mvprintw(max_y / 2 - 2, max_x / 2 - 6, "|         |");
        mvprintw(max_y / 2 - 1, max_x / 2 - 6, "|  Gone   |");
        mvprintw(max_y / 2    , max_x / 2 - 6, "|_________|");

        // Print the tragic news
        mvprintw(max_y / 2 + 2, (max_x - (strlen(g->name) + 16)) / 2, "%s has passed away.", g->name);
        attroff(A_BOLD);

        mvprintw(max_y - 2, 2, "Controls: [q] Quit   [r] Rest in peace (Delete save & restart)");
        refresh();
        return;
    }

    // Print the Gotchi's name centered at the top
    attron(A_BOLD);
    mvprintw(1, (max_x - strlen(g->name)) / 2, "%s", g->name);
    attroff(A_BOLD);

    int health_pair, hunger_pair, thirst_pair, litter_pair;
    // Health (Max 10)
    if (g->health <= 3) {
        health_pair = 3; // Red
    } else if (g->health <= 6) {
        health_pair = 2; // Yellow
    } else {
        health_pair = 1; // Green
    }
    // Hunger (Max 5)
    if (g->hunger >= TOO_HUNGRY) {
        hunger_pair = 3; // Red
    } else if (g->hunger >= 3) {
        hunger_pair = 2; // Yellow
    } else {
        hunger_pair = 1; // Green
    }
    // Thirst (Max 5)
    if (g->thirst >= TOO_THIRSTY) {
        thirst_pair = 3; // Red
    } else if (g->thirst >= 3) {
        thirst_pair = 2; // Yellow
    } else {
        thirst_pair = 4; // Cyan
    }
    // Litter (Max 5)
    if (g->litter >= TOO_POOPY) {
        litter_pair = 3; // Red
    } else if (g->litter >= 3) {
        litter_pair = 2; // Yellow
    } else {
        litter_pair = 1; // Green
    }

    // --- Draw Stat Bars with Nerd Font Symbols ---
    draw_stat_bar(3, 2, "Health:", g->health, MAX_HEALTH, "󰋑 ", health_pair);
    draw_stat_bar(4, 2, "Mood:",   g->mood,   MAX_MOOD,   " ", 4);
    draw_stat_bar(7, 2, "Hunger:", g->hunger, MAX_HUNGER, "󱐟 ", hunger_pair);
    draw_stat_bar(8, 2, "Thirst:", g->thirst, MAX_THIRST, "󰖌 ", thirst_pair);
    draw_stat_bar(9, 2, "Litter:", g->litter, MAX_LITTER, " ", litter_pair);

    // Show sickness status
    if (g->isSick) {
        attron(COLOR_PAIR(4) | A_STANDOUT);
        mvprintw(12, 2, "STATUS: SICK!");
        attroff(COLOR_PAIR(4) | A_STANDOUT);
    }

    // Determine which animation frame to show (0 or 1) based on time
    int current_frame = time(NULL) % 2;

    // Load and draw the ASCII art from the external text file
    draw_external_art(g->animal, current_frame, max_y, max_x);

    // Draw controls at the bottom
    mvprintw(max_y - 2, 2, "Controls: [f] Feed  [w] Water  [p] Play  [c] Clean  [q] Quit");

    refresh();
}
