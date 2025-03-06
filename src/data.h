#ifndef DATA_H
#define DATA_H
#include "gotchi.h"

/*Get save directory*/
char *init_savefile_dir(void);

void save(Gotchi *gotchi);

void readsave(Gotchi *gotchi);

int check_save(void);

int get_userpet(void);

#endif
