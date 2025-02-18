#include <stdlib.h>
#include <stdio.h>
#include "gotchi.h"
#include "data.h"


Gotchi *gotchi_init(bool isNew) {
    Gotchi *gotchi = calloc(1, sizeof(Gotchi));
    if (gotchi == NULL) {
        perror("Error allocating memory for Gotchi\n");
        exit(1);
    }
    if (isNew) {
        gotchi->health = MAX_HEALTH;
        gotchi->mood = MAX_MOOD;
        gotchi->isSick = false;
        gotchi->hunger = 0;
        gotchi->thirst = 0;
        gotchi->name[0] = '\0';
    }
    return gotchi;
}


void gotchi_update(Gotchi *g, int hDiff, int mDiff, int hungDiff, int tDiff) {
    if (g == NULL) return;  // avoid modifying a NULL pointer

    g->health += hDiff;
    g->mood += mDiff;
    g->hunger += hungDiff;
    g->thirst += tDiff;
}


void gotchi_save(Gotchi *gotchi) {
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


void gotchi_readsave(Gotchi *gotchi) {
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
