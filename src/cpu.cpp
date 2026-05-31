#include "cpu.h"
#include "defs.h"
#include "display.h"


Error_t CPU::Initialize() {

    // Initialize random generator
    std::random_device rd;
    m_gen = std::mt19937(rd());

    // Initialize audio mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                     "Failed to initialize SDL audio subsystem. %s", SDL_GetError());
        return ERROR_CODE;
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                      MIX_DEFAULT_CHANNELS, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to initialize mixer. %s", Mix_GetError());
        return ERROR_CODE;
    }
    m_beepSound = Mix_LoadWAV("beep.wav");
    if (m_beepSound == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to load beep.wav. %s", Mix_GetError());
        return ERROR_CODE;
    }

    // Set program counter to the start of ROM
    m_pc = ROM_START_ADDRESS;
    return SUCCESS;
}

CPU::~CPU() {
    if (m_beepSound) {
        Mix_FreeChunk(m_beepSound);
    }
    Mix_CloseAudio();
    SDL_Quit();
}

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

    // Decrementing timers
    if (m_dt > 0) {
        m_dt -= 1;
    }
    if (m_st > 0) {
        Mix_PlayChannel(-1, m_beepSound, 0);
        m_st -= 1;
    }

    switch (n1) {
        case 0: {
            if (instruction == 0x00E0) {
                display->ClearScreen();
                return;
            }
            if (instruction == 0x00EE) { // Function call
                if (!m_stack.size()) {
                    SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "[0x00EE] Stack is empty.");
                    return;
                }
                m_pc = m_stack.top();
                m_stack.pop();
                return;
            }
            break;
        }
        case 1: { // Jump
            m_pc = NNN;
            return;
        }
        case 2: { // Function returned
            m_stack.push(m_pc);
            m_pc = NNN;
            return;
        }
        case 3: { // Skip an instruction
            if (m_v[X] == NN) {
                m_pc += 2;
            }
            return;
        }
        case 4: { // Skip an instruction
            if (m_v[X] != NN) {
                m_pc += 2;
            }
            return;
        }
        case 5: { // Skip an instruction
            if (m_v[X] == m_v[Y]) {
                m_pc += 2;
            }
            return;
        }
        case 6: {
            m_v[X] = NN;
            return;
        }
        case 7: {
            m_v[X] += NN;
            return;
        }
        case 8: { // Arithmetic operations
            switch (N) {
                case 0: {
                    m_v[X] = m_v[Y];
                    return;
                }
                case 1: {
                    m_v[X] |= m_v[Y];
                    return;
                }
                case 2: {
                    m_v[X] &= m_v[Y];
                    return;
                }
                case 3: {
                    m_v[X] ^= m_v[Y];
                    return;
                }
                case 4: {
                    uint16_t sum = static_cast<uint16_t>(m_v[X]) + static_cast<uint16_t>(m_v[Y]);
                    m_v[0xF] = (sum > UINT8_MAX) ? 1 : 0;
                    m_v[X] += m_v[Y];
                    return;
                }
                case 5: {
                    m_v[0xF] = (m_v[X] >= m_v[Y]) ? 1 : 0;
                    m_v[X] -= m_v[Y];
                    return;
                }
                case 6: {
                    m_v[0xF] = m_v[X] & 0x1; // 0000 0001
                    m_v[X] >>= 1;
                    return;
                }
                case 7: {
                    m_v[0xF] = (m_v[Y] >= m_v[X]) ? 1 : 0;
                    m_v[X] = (m_v[Y] - m_v[X]);
                    return;
                }
                case 0xE: {
                    m_v[0xF] = (m_v[X] >> 7) & 1; // MSB
                    m_v[X] <<= 1;
                    return;
                }
                default: {
                    break;
                }
            }
            break;
        }
        case 9: { // Skip an instruction
            if (m_v[X] != m_v[Y]) {
                m_pc += 2;
            }
            return;
        }
        case 0xA: {
            m_i = NNN;
            return;
        }
        case 0xB: {
            m_pc = NNN + m_v[0];
            return;
        }
        case 0xC: {
            m_v[X] = NN & m_dist(m_gen);
            return;
        }
        case 0xD: { // Draw instruction
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
            return;
        }
        case 0xE: { // If key pressed
            if (NN == 0x9E) {
                if (display->IsKeyPressed(m_v[X])) {
                    m_pc += 2;
                }
                return;
            }
            if (NN == 0xA1) {
                if (!display->IsKeyPressed(m_v[X])) {
                    m_pc += 2;
                }
                return;
            }
            break;
        }
        case 0xF: {
            switch (NN) {
                case 0x07: {
                    m_v[X] = m_dt;
                    return;
                }
                case 0x15: {
                    m_dt = m_v[X];
                    return;
                }
                case 0x18: {
                    m_st = m_v[X];
                    return;
                }
                case 0x1E: {
                    m_i += m_v[X];
                    if (m_i > 0xFFF) {
                        m_v[0xF] = 1;
                    }
                    return;
                }
                case 0x0A: { // Wait for a key press. Blocking instruction
                    uint8_t key = display->GetPressedKey();
                    if (key > 0xF) { // No key is pressed
                        m_pc -= 2; // Repeat this instruction
                        return;
                    }
                    m_v[X] = key;
                    return;
                }
                case 0x29: {
                    uint8_t chr = m_v[X] & 0xF;
                    m_i = FONTS_START_ADDRESS + chr * BYTES_PER_FONT;
                    return;
                }
                case 0x33: {
                    auto x = m_v[X];
                    memory[m_i] = x / 100; x %= 100;
                    memory[m_i + 1] = x / 10; x %= 10;
                    memory[m_i + 2] = x;
                    return;
                }
                case 0x55: {
                    for (uint16_t i{0}; i <= static_cast<uint16_t>(X); i++) {
                        memory[m_i + i] = m_v[i];
                    }
                    return;
                }
                case 0x65: {
                    for (uint16_t i{0}; i <= static_cast<uint16_t>(X); i++) {
                        m_v[i] = memory[m_i + i];
                    }
                    return;
                }
                default: {
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Invalid instruction: 0x%X", instruction);
}
