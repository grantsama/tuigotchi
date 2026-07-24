#ifndef GOTCHI_H
#define GOTCHI_H
#include <stdbool.h>
#include <time.h>

#define MAX_HEALTH 10
#define MAX_MOOD 5
#define TOO_HUNGRY 4
#define TOO_THIRSTY 4
#define TOO_POOPY 4
#define CAT 0
#define DOG 1
#define HAM 2
#define MAX_HUNGER 5
#define MAX_THIRST 5
#define MAX_LITTER 5
#define TICK_RATE 86400 // == 24hrs

typedef struct {
    short animal;
    char name[21];
    int health;
    int mood;
    int litter;
    int hunger;
    int thirst;
    bool isSick;
    bool isDead;
    time_t last_saved;
} Gotchi;

/* Initialize a gotchi,
 * and sets all attributes to default values
 * Caller must free returned pointer.
 * DOES NOT SET ANIMAL CHOICE OR NAME */
Gotchi *gotchi_init(bool isNew);

/* Calculates time since last launched and updates gotchi */
int gotchi_process_offtime(Gotchi *g);

/* Live tick for background stat decay */
void gotchi_tick(Gotchi *g);

/* Update the gotchi.
 * Alter gotchi's attributes based on function params. */
void gotchi_update(Gotchi *gotchi, int healthDiff, int moodDiff,
                   int hungDiff, int thirstDiff, int litterDiff);

#endif
