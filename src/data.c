#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "data.h"

char *init_savefile_dir(const char *dirpath) {
    // Save file passed via -s flag
    if (dirpath != NULL) {
        char *saveFileDir = malloc(strlen(dirpath) + 1);
        if (saveFileDir == NULL) {
            perror("Error allocating memory for save directory\n");
            exit(1);
        }
        strcpy(saveFileDir, dirpath);
        return(saveFileDir);
    }

    // Default save file
    char *saveFileDir = NULL;
    const char *home = getenv("HOME");

    if (home == NULL) {
        fprintf(stderr, "Error: HOME environment variable is not set.\n");
        fprintf(stderr, "TUIGotchi requires a home directory to save your pet's data.\n");
        exit(1);
    }

    const char *pathSuffix = "/.config/tuigotchi/dat/save";
    saveFileDir = malloc(strlen(home) + strlen(pathSuffix) + 1);

    if (saveFileDir == NULL) {
        perror("Error allocating memory for save directory\n");
        exit(1);
    }

    // Build the final save file path
    strcpy(saveFileDir, home);
    strcat(saveFileDir, pathSuffix);

    // 2. DIRECTORY CREATION: Make sure the folders actually exist!
    char dirPath[512];

    // Create ~/.config/tuigotchi
    snprintf(dirPath, sizeof(dirPath), "%s/.config/tuigotchi", home);
    mkdir(dirPath, 0777);

    // Create ~/.config/tuigotchi/dat
    snprintf(dirPath, sizeof(dirPath), "%s/.config/tuigotchi/dat", home);
    mkdir(dirPath, 0777);

    return saveFileDir;
}

int check_save(const char *dirpath) {
    /* Checks if save exists and is the correct size
     * returns 1 if true, else returns 0
     */
    char *saveFileDir = init_savefile_dir(dirpath);
    int result = 0;

    FILE *f = fopen(saveFileDir, "rb");
    if (f != NULL) {
        // Seek to the end to check the file size
        fseek(f, 0, SEEK_END);
        long fileSize = ftell(f);

        // Only return true if the file contains exactly one Gotchi struct
        if (fileSize == sizeof(Gotchi)) {
            result = 1;
        }
        fclose(f);
    }

    free(saveFileDir);
    return result;
}

int get_userpet(void) {
    int petChoice = 0;
    while (1) {
        printf("Choose your pet: 1. Cat  2. Dog  3. Hamster (input number)\n> ");

        if (scanf("%d", &petChoice) == 1) {
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            if (petChoice >= 1 && petChoice <= 3) {
                break;
            }
        } else {
            // Check if the user pressed Ctrl+D
            if (feof(stdin)) {
                printf("\nInput stream closed. Exiting...\n");
                exit(1);
            }
            int c;
            while((c = getchar()) != '\n' && c != EOF);
        }
        printf("Invalid pet choice! Please try again.\n\n");
    }

    if (petChoice == 1) {
        printf("Cat Selected!\n");
    }
    else if (petChoice == 2) {
        printf("Dog Selected!\n");
    }
    else if (petChoice == 3) {
        printf("Hamster Selected!\n");
    }

    return petChoice - 1;
}

void get_petname(char *nameBuf, int maxLen) {
    while (1) {
        printf("What will you name your pet?\n> ");

        if (fgets(nameBuf, maxLen, stdin) != NULL) {
            char *newline = strchr(nameBuf, '\n');
            if (newline != NULL) {
                *newline = '\0';
            } else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }

            if (strlen(nameBuf) > 0) {
                break;
            }

        } else {
            // If fgets returns NULL because of Ctrl+D
            if (feof(stdin)) {
                printf("\nInput stream closed. Exiting...\n");
                exit(1);
            }
        }

        printf("Name cannot be empty! Try again.\n\n");
    }
}

void save(Gotchi *gotchi, const char *dirpath) {
    if (gotchi == NULL) {
        perror("Cannot save: Gotchi is NULL");
        return;
    }

    gotchi->last_saved = time(NULL);  // record time before saving data

    char *saveFileDir = init_savefile_dir(dirpath);
    FILE *f = fopen(saveFileDir, "wb");

    // --- GRACEFUL FAIL & FALLBACK LOGIC ---
    if (f == NULL) {
        fprintf(stderr, "Warning: Could not open save path '%s' for writing.\n", saveFileDir);

        // If they used a custom path, try falling back to the default path
        if (dirpath != NULL) {
            fprintf(stderr, "Attempting to save to the default directory instead...\n");
            free(saveFileDir);
            saveFileDir = init_savefile_dir(NULL); // Pass NULL to get the default path
            f = fopen(saveFileDir, "wb");
        }

        // If the file pointer is STILL NULL, abort the save cleanly
        if (f == NULL) {
            fprintf(stderr, "Error: Could not save your Gotchi. Progress for this session is lost.\n");
            free(saveFileDir);
            return; // Return instead of exit(1)
        }
    }
    // --------------------------------------

    size_t bytesWritten = fwrite(gotchi, sizeof(Gotchi), 1, f);
    if (bytesWritten != 1) {
        perror("Error writing save data\n");
    } else {
        printf("Gotchi saved successfully to: %s\n", saveFileDir);
    }

    fclose(f);
    free(saveFileDir);
}

void readsave(Gotchi *gotchi, const char *dirpath) {
    if (gotchi == NULL) {
        perror("Cannot load: Gotchi pointer is NULL\n");
        return;
    }

    // Pass the custom_path down to our directory initializer
    char *saveFileDir = init_savefile_dir(dirpath);
    FILE *f = fopen(saveFileDir, "rb");
    if (f == NULL) {
        perror("Error opening save file for reading\n");
        free(saveFileDir);
        free(gotchi);
        exit(1);
    }

    size_t bytesRead = fread(gotchi, sizeof(Gotchi), 1, f);
    if (bytesRead != 1) {
        if (feof(f) != 1) {
            fprintf(stderr, "Error: Save file is empty or incomplete.\n");
        } else {
            perror("Error reading save data\n");
        }
        fclose(f);
        free(saveFileDir);
        free(gotchi);
        exit(1);
    }

    fclose(f);
    free(saveFileDir);
}
