#include <stdbool.h>
#define MAX_HEALTH 20
#define MAX_MOOD 20


typedef struct {
    char name[30];
    int health;
    int mood;
    bool isSick;

} Gotchi;


Gotchi *gotchi_init(void);
/* Initialize a gotchi, ask user for a name
 * and sets all attributes to default values
 * */
