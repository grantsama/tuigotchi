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


void save(Gotchi *gotchi) {
    if (gotchi == NULL) {
        perror("Cannot save: Gotchi is NULL");
        return;
    }

    char *saveFileDir = init_savefile_dir();
    FILE *f = fopen(saveFileDir, "wb");
    if (f == NULL) {
        perror("Error opening save file for writing\n");
        free(saveFileDir);
        free(gotchi);
        exit(1);
    }

    size_t bytesWritten = fwrite(&gotchi, sizeof(*gotchi), 1, f);
    if (bytesWritten != 1) {
        perror("Error writing save data\n");
    }
    fclose(f);
    free(saveFileDir);
}


void readsave(Gotchi *gotchi) {
    if (gotchi == NULL) {
        perror("Cannot load: Gotchi pointer is NULL\n");
        return;
    }

    char *saveFileDir = init_savefile_dir();
    FILE *f = fopen(saveFileDir, "rb");
    if (f == NULL) {
        perror("Error opening save file for reading\n");
        free(saveFileDir);
        free(gotchi);
        exit(1);
    }

    size_t bytesRead = fread(gotchi, sizeof(Gotchi), 1, f);
    if (bytesRead != 1) {
        perror("Error reading save data\n");
        fclose(f);
        free(saveFileDir);
        free(gotchi);
        exit(1);
    }

    fclose(f);
    free(saveFileDir);
}
