#include "checkfiles.h"

const char *saveFileDir = "dat/save";


// ***WORKING***
int check_save(void) {
    /* Checks if save_data.txt exsists
     * returns 1 if true, else returns 0
     */
    if (access(saveFileDir, F_OK) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}


int get_save(void) {
    /* Reads and returns save data */
    int line;
    int data[5] = {0};
    FILE *f = fopen(saveFileDir, "r");
    for (int i = 0; i < 5; i++) { // I HAVE NO CLUE HOW TO DO THIS
        line = 0;
        fscanf(f, "%d", &line);
        data[i] = line;
    }
    fclose(f);
    return *data;
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


void display_pet(int petChoice) {
    /* Print ascii art from txt file depending on petChoice */
    if (petChoice == 1) {
        printf("1\n");
    }
    else if (petChoice == 2) {
        printf("2\n");
    }
    else {
        printf("3\n");
    }
}


// ***WORKING***
void write_save(int *data) {
    /* Writes game data to txt file */
    FILE *f = fopen(saveFileDir, "w");
    for (int i = 0; i < 5; i++) {
        fprintf(f, "%d\n", data[i]);
    }
    fclose(f);
}
