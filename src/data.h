#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*Get save directory*/
char *init_savefile_dir(void);

int check_save(void);

int *get_save(void);

int get_userpet(void);

void display_pet(int petChoice);

void write_save(int *data);
