#ifndef CPU_H
#define CPU_H

#include <stack>
#include <string>
#include <cstdint>


class Display;

class CPU {
public:
    inline void SetProgramCounter(uint16_t pc) {
        m_pc = pc;
    }

    uint16_t FetchInstruction(uint8_t *memory);
    void DecodeAndExecute(uint16_t instruction, uint8_t *memory, Display *display);

private:
    uint16_t m_i{};     // Index register
    uint16_t m_pc{};    // Program counter
    uint8_t  m_dt{};    // Delay timer
    uint8_t  m_st{};    // Sound timer
    uint8_t  m_v[0x10] = {};    // Variable registers (V0, ..., VF)
    std::stack<uint16_t> m_stack;   // Function addresses
};

#endif // CPU_H
