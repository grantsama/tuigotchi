#include <stdlib.h>
#include <stdio.h>
#include "gotchi.h"


Gotchi *gotchi_init(bool isNew) {
    Gotchi *gotchi = calloc(1, sizeof(Gotchi));
    if (gotchi == NULL) {
        perror("Error allocating memory for Gotchi\n");
        exit(1);
    }
    if (isNew) {
        gotchi->health = MAX_HEALTH;
        gotchi->mood = MAX_MOOD;
        gotchi->isSick = false;
        gotchi->hunger = 0;
        gotchi->thirst = 0;
        gotchi->litter = 0;
        gotchi->name[0] = '\0';
    }
    return gotchi;
}


void gotchi_update(Gotchi *g, int hDiff, int mDiff,
                   int hungDiff, int tDiff, int lDiff) {
    if (g == NULL) return;  // avoid modifying a NULL pointer

    g->health += hDiff;
    g->mood += mDiff;
    g->hunger += hungDiff;
    g->thirst += tDiff;
    g->litter += lDiff;

    if (g->hunger >= TOO_HUNGRY)
        g->health -= 2;
    if (g->thirst >= TOO_THIRSTY)
        g->health -= 3;
    if (g->mood == 0)
        g->health -= 1;
    if (g->litter >= TOO_POOPY) {
        g->isSick = true;
        g->health -= 2;
    } else {
        g->isSick = false;
    }
}
