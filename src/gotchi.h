#ifndef GOTCHI_H
#define GOTCHI_H
#include <stdbool.h>

#define MAX_HEALTH 20
#define MAX_MOOD 5
#define TOO_HUNGRY 10
#define TOO_THIRSTY 8
#define TOO_POOPY 5
#define CAT 0
#define DOG 1
#define HAM 2

typedef struct {
    short animal;
    char name[21];
    int health;
    int mood;
    int litter;
    int hunger;
    int thirst;
    bool isSick;
} Gotchi;

/* Initialize a gotchi,
 * and sets all attributes to default values
 * Caller must free returned pointer.
 * DOES NOT SET ANIMAL CHOICE OR NAME */
Gotchi *gotchi_init(bool isNew);

/* Update the gotchi.
 * Alter gotchi's attributes based on function params. */
void gotchi_update(Gotchi *gotchi, int healthDiff, int moodDiff,
                   int hungDiff, int thirstDiff, int litterDiff);

#endif
