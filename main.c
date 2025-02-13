#include "data.h"


int main(int argc, const char *argv[]) {
    if (check_save() == 0) {
        int tempData[5] = {0};
        tempData[0] = get_userpet();
        write_save(tempData);
    }
    else {
        int save = get_save();
    }
    return 0;
}
