/**
 * Reference: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
 */

#include "motherboard.h"


int main() {

    Motherboard motherboard;
    if (motherboard.LoadROM("IBM_logo.ch8") != SUCCESS) {
        return 1;
    }

    return 0;
}
