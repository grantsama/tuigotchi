#include "data.h"


int main(int argc, const char *argv[]) {
    int *saveData = NULL;  // Declare saveData outside the conditional block

    if (check_save() == 0) {
        int tempData[5] = {0};
        tempData[0] = get_userpet();
        write_save(tempData);
    }
    else {
        saveData = get_save();  // Assign saveData inside the else block
    }

    if (saveData != NULL) {  // Only free if saveData was allocated
        free(saveData);
    }
    
    return 0;
}
