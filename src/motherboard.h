#ifndef MOTHERBOARD_H
#define MOTHERBOARD_H

#include "defs.h"
#include "cpu.h"
#include "display.h"


class Motherboard {
public:
    Error_t LoadROM(const std::string &path);

private:
    CPU m_cpu;
    Display m_display;

    /**
     * Originally addresses 0 to 0x1FF (512 bytes) were reserved for CHIP-8 interpretor.
     * Since that's not needed for an emulator, address space 0x050 to 0x09F
     * will be used to store fonts representing hexadecimal numbers 0 to F.
     */
    char m_memory[MEMORY_SIZE_BYTES];
};

#endif // MOTHERBOARD_H
