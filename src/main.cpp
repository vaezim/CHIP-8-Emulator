/**
 * Reference: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
 */

#include "motherboard.h"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Usage: ./chip8-emulator [ROM_FILENAME]");
        return ERROR_CODE;
    }

    Motherboard motherboard;
    if (motherboard.LoadROM(argv[1]) != SUCCESS) {
        return 1;
    }

    return motherboard.Run();
}
