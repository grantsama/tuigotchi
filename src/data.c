#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data.h"


char *init_savefile_dir(void) {
    /* Get save directory */
    char *saveFileDir = NULL;

    const char *home = getenv("HOME");
    saveFileDir = malloc(strlen(home) + strlen("/.config/tuigotchi/dat/save") + 1);
    strcpy(saveFileDir, home);
    strcat(saveFileDir, "/.config/tuigotchi/dat/save");

    return saveFileDir;
}


int check_save(void) {
    /* Checks if save exsists
     * returns 1 if true, else returns 0
     */
    char *saveFileDir = init_savefile_dir();
    int result = !(access(saveFileDir, F_OK) == 0);
    free(saveFileDir);
    return result;
}


int *get_save(void) {
/* Reads and returns save data */
    int *data = malloc(5 * sizeof(int));  // Dynamically allocate memory for the array
    int line;

    if (data == NULL) {
        // Handle memory allocation failure
        printf("Memory allocation failed!\n");
        return NULL;
    }

    char *saveFileDir = init_savefile_dir();
    FILE *f = fopen(saveFileDir, "r");
    for (int i = 0; i < 5; i++) {
        line = 0;
        fscanf(f, "%d", &line);
        data[i] = line;
    }
    fclose(f);
    free(saveFileDir);

    return data;  // Return the pointer to the dynamically allocated array
}


// ***WORKING***
int get_userpet(void) {
    /* Prompts user to choose a pet */
    int petChoice;
    printf("Choose your pet: 1. Cat  2. Dog  3. Hamster (input number)\n");
    printf("> ");
    scanf("%d", &petChoice);
    if (petChoice == 1) {
        printf("Cat Selected!\n");
    }
    else if (petChoice == 2) {
        printf("Dog Selected!\n");
    }
    else {
        printf("Hamster Selected!\n");
    }

    return petChoice;
}
