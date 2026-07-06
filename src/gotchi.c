#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gotchi.h"

Gotchi *gotchi_init(bool isNew) {
    Gotchi *gotchi = calloc(1, sizeof(Gotchi));
    if (gotchi == NULL) {
        perror("Error allocating memory for Gotchi\n");
        exit(1);
    }
    if (isNew) {
        gotchi->health = MAX_HEALTH;
        gotchi->mood = 3;
        gotchi->isSick = false;
        gotchi->hunger = 2;
        gotchi->thirst = 1;
        gotchi->litter = 0;
        gotchi->name[0] = '\0';
        gotchi->last_saved = time(NULL);
    }
    return gotchi;
}

void gotchi_tick(Gotchi *g) {
    /* Helper function for gotchi_process_offline_time */
    if (g == NULL) return;

    int health_penalty = 0;
    if (g->isSick) {
        health_penalty -= 2;
    }
    if (g->hunger >= TOO_HUNGRY) {
        health_penalty -= 2;
    }
    if (g->thirst >= TOO_THIRSTY) {
        health_penalty -= 2;
    }

    gotchi_update(g, health_penalty, -1, 1, 1, 1);
}

int gotchi_process_offtime(Gotchi *g) {
    if (g == NULL) {
        return 0;
    }

    time_t now = time(NULL);
    double seconds_passed = difftime(now, g->last_saved);

    int intervals = (int)(seconds_passed / 86400);  // 86400 seconds == 24 hours
    for (int i = 0; i < intervals; i++) {
        gotchi_tick(g);
    }

    return intervals;
}

void gotchi_update(Gotchi *g, int hDiff, int mDiff,
                   int hungDiff, int tDiff, int lDiff) {
    if (g == NULL) return;  // avoid modifying a NULL pointer

    // Apply changes
    g->health += hDiff;
    g->mood += mDiff;
    g->hunger += hungDiff;
    g->thirst += tDiff;
    g->litter += lDiff;

    // Clamp values between 0 and their maximums
    if (g->health > MAX_HEALTH) g->health = MAX_HEALTH;
    if (g->health < 0) g->health = 0;

    if (g->mood > MAX_MOOD) g->mood = MAX_MOOD;
    if (g->mood < 0) g->mood = 0;

    if (g->hunger > MAX_HUNGER) g->hunger = MAX_HUNGER;
    if (g->hunger < 0) g->hunger = 0;

    if (g->thirst > MAX_THIRST) g->thirst = MAX_THIRST;
    if (g->thirst < 0) g->thirst = 0;

    if (g->litter > MAX_LITTER) g->litter = MAX_LITTER;
    if (g->litter < 0) g->litter = 0;

    // Apply penalties if thresholds are crossed
    if (g->hunger >= TOO_HUNGRY)
        g->health -= 2;
    if (g->thirst >= TOO_THIRSTY)
        g->health -= 3;
    if (g->mood == 0)
        g->health -= 1;
    if (g->litter >= TOO_POOPY) {
        g->isSick = true;
        g->health -= 2; // Adding a health penalty for sickness logic
    }

    // One final clamp for health, just in case penalties dropped it below 0
    if (g->health < 0) g->health = 0;
}
