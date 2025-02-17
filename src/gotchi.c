#include <stdlib.h>
#include "gotchi.h"


Gotchi *gotchi_init(void) {
    Gotchi *newGotchi = calloc(1, sizeof(Gotchi));
    newGotchi -> health = MAX_HEALTH;
    newGotchi -> mood = MAX_MOOD;
    newGotchi -> isSick = false;

    return newGotchi;
}
