#ifndef CPU_H
#define CPU_H

#include <stack>
#include <string>
#include <random>
#include <cstdint>

#include <SDL2/SDL_mixer.h>

#include "defs.h"


class Display;

class CPU {
public:
    CPU() = default;
    ~CPU();

    Error_t Initialize();

    void DecrementTimers();
    uint16_t FetchInstruction(uint8_t *memory);
    void DecodeAndExecute(uint16_t instruction, uint8_t *memory, Display *display);

private:
    uint16_t m_i{};     // Index register (points to memory locations)
    uint16_t m_pc{};    // Program counter (instruction pointer for CPU)
    uint8_t  m_dt{};    // Delay timer
    uint8_t  m_st{};    // Sound timer
    uint8_t  m_v[0x10] = {};    // Variable registers (V0, ..., VF)
    std::stack<uint16_t> m_stack;   // Function addresses

    std::mt19937 m_gen;
    std::uniform_int_distribution<uint8_t> m_dist{ 1, UINT8_MAX };

    Mix_Chunk *m_beepSound = nullptr;
};

#endif // CPU_H
