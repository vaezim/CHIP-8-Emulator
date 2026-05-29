#include <fstream>

#include "motherboard.h"


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
    file.read(m_memory + ROM_START_ADDRESS, romSize);
    file.close();

    return SUCCESS;
}
