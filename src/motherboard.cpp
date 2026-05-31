#include <fstream>

#include "motherboard.h"


Error_t Motherboard::Run() {
    // Load fonts to memory
    for (int i{ 0 }; i < FONTS_SIZE; i++) {
        m_memory[FONTS_START_ADDRESS + i] = FONTS[i];
    }
    // Initialize Display and CPU
    if (m_display.Initialize() != SUCCESS || m_cpu.Initialize() != SUCCESS) {
        return ERROR_CODE;
    }
    // Game loop:
    // Every loop should take TIME_PER_FRAME_MLLISECONDS milliseconds.
    // CPU will run TARGET_INSTRUCTIONS_PER_FRAME instructions each loop.
    // At the end of every loop, timers will be decremented and (if a draw
    // instruction was executed during loop runtime) display will be updated.
    while (m_display.ShouldRun()) {
        auto start = SDL_GetTicks();
        for (int i{ 0 }; i < TARGET_INSTRUCTIONS_PER_FRAME; i++) {
            auto instruction = m_cpu.FetchInstruction(m_memory);
            m_cpu.DecodeAndExecute(instruction, m_memory, &m_display);
        }
        m_cpu.DecrementTimers();
        if (m_display.ShouldUpdateScreen()) {
            m_display.RenderPixels();
        }
        auto loopDuration = SDL_GetTicks() - start;
        if (loopDuration < TARGET_MLLISECONDS_PER_FRAME) {
            SDL_Delay(TARGET_MLLISECONDS_PER_FRAME - loopDuration);
        }
    }
    return SUCCESS;
}

Error_t Motherboard::LoadROM(const std::string &path) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading ROM file: %s", path.c_str());

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read ROM file: %s", path.c_str());
        return ERROR_CODE;
    }

    size_t romSize = file.tellg();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ROM size: %ld bytes", romSize);
    if (romSize > MEMORY_SIZE_BYTES - ROM_START_ADDRESS + 1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "ROM size is larger than available memory space %d",
                     MEMORY_SIZE_BYTES - ROM_START_ADDRESS + 1);
        return ERROR_CODE;
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(&m_memory[ROM_START_ADDRESS]), romSize);
    file.close();

    return SUCCESS;
}
