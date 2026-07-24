#ifndef DATA_H
#define DATA_H
#include "gotchi.h"

/* Get save directory */
char *init_savefile_dir(const char *dirpath);

void save(Gotchi *gotchi, const char *dirpath);

void readsave(Gotchi *gotchi, const char *dirpath);

void get_petname(char *nameBuf, int maxLen);

int check_save(const char *dirpath);

int get_userpet(void);

#endif
