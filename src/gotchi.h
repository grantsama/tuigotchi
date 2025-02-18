#include <stdbool.h>
#define MAX_HEALTH 20
#define MAX_MOOD 10
#define TOO_HUNGRY 10
#define TOO_THIRSTY 8
#define TOO_POOPY 8
#define CAT 0
#define DOG 1
#define HAM 2


typedef struct {
    short animal;
    char name[21];
    int health;
    int mood;
    int hunger;
    int thirst;
    bool isSick;

} Gotchi;


Gotchi *gotchi_init(bool isNew);
/* Initialize a gotchi,
 * and sets all attributes to default values
 * DOES NOT SET ANIMAL CHOICE OR NAME
 * */

void gotchi_update(Gotchi *gotchi, int healthDiff, int moodDiff, int hungDiff, int thirstDiff);
/* Update the gotchi.
 * Alter gotchi's attributes based on function params.
 * */

void gotchi_save(Gotchi *gotchi);

void gotchi_readsave(Gotchi *gotchi);
