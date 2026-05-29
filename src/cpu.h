#ifndef CPU_H
#define CPU_H

#include <stack>
#include <string>
#include <cstdint>


class Display;

class CPU {
public:
    uint16_t Fetch(char *memory);
    void DecodeAndExecute(uint16_t instruction, Display *display);

private:
    uint16_t m_pc{};    // Program counter
    uint16_t m_i{};     // Index register
    uint8_t  m_dt{};    // Delay timer
    uint8_t  m_st{};    // Sound timer
    uint8_t  m_v[16] = {};  // Variable registers (V0, ..., VF)
    std::stack<uint16_t> m_stack;   // Function addresses
};

#endif // CPU_H
