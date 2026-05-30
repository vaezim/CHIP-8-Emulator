#include "SDL2/SDL_log.h"

#include "cpu.h"
#include "defs.h"
#include "display.h"


uint16_t CPU::FetchInstruction(uint8_t *memory) {
    if (m_pc >= MEMORY_SIZE_BYTES - 1) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,
            "PC=%d exceeds memory size=%d", m_pc, MEMORY_SIZE_BYTES);
        return 0;
    }
    uint16_t instruction = 0;
    instruction += (static_cast<uint16_t>(memory[m_pc]) << 8);
    instruction += static_cast<uint16_t>(memory[m_pc + 1]);
    m_pc += 2;
    return instruction;
}

void CPU::DecodeAndExecute(uint16_t instruction, uint8_t *memory, Display *display) {
    uint8_t   n1 = (instruction >> 12) & 0xF;   // First nibble (half-byte; 4 bits)
    uint8_t    X = (instruction >>  8) & 0xF;   // Second nibble
    uint8_t    Y = (instruction >>  4) & 0xF;   // Third nibble
    uint8_t    N = instruction & 0xF;           // Fourth nibble
    uint8_t   NN = instruction & 0xFF;          // Third and Fourth nibbles
    uint16_t NNN = instruction & 0xFFF;         // Second, Third, and Fourth nibbles (12-bit memory address)

    switch (n1) {
        case 0: {
            if (instruction == 0x00E0) {
                display->ClearScreen();
            }
            break;
        }
        case 1: {
            m_pc = NNN;
            break;
        }
        case 6: {
            m_v[X] = NN;
            break;
        }
        case 7: {
            m_v[X] += NN;
            break;
        }
        case 0xA: {
            m_i = NNN;
            break;
        }
        case 0xD: {
            X = m_v[X] % PIXELS_PER_WIDTH;
            Y = m_v[Y] % PIXELS_PER_HEIGHT;
            m_v[0xF] = 0;
            for (int j{ 0 }; j < N && Y+j < PIXELS_PER_HEIGHT; j++) {
                uint8_t byte = memory[m_i + j];
                for (int i{ 0 }; i < 8 && X+i < PIXELS_PER_WIDTH; i++) {
                    if(byte & (1 << (7-i))) {
                        if (display->GetPixel(X+i, Y+j)) {
                            m_v[0xF] = 1;
                        }
                        display->FlipPixel(X+i, Y+j);
                    }
                }
            }
            display->DrawPixels();
            break;
        }
        default: {
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Invalid first nibble: %d", n1);
            break;
        }
    }
}
