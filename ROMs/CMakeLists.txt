set(ROM_FILENAME_LIST
    "IBM_logo.ch8"
    "BC_test.ch8"
    "test_opcode.ch8"
)
set(DOWNLOAD_URL_LIST
    "https://github.com/loktar00/chip8/raw/refs/heads/master/roms/IBM%20Logo.ch8"
    "https://github.com/cj1128/chip8-emulator/raw/refs/heads/master/rom/BC_test.ch8"
    "https://github.com/corax89/chip8-test-rom/raw/refs/heads/master/test_opcode.ch8"
)

foreach(item IN ZIP_LISTS ROM_FILENAME_LIST DOWNLOAD_URL_LIST)
    set(ROM_FILENAME "${item_0}")
    set(DOWNLOAD_URL "${item_1}")

    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ROM_FILENAME}")
        message(STATUS "Downloading ROM file ${ROM_FILENAME}")
        file(DOWNLOAD
            ${DOWNLOAD_URL}
            "${CMAKE_CURRENT_SOURCE_DIR}/${ROM_FILENAME}"
            SHOW_PROGRESS
            TIMEOUT 10
            STATUS DOWNLOAD_STATUS
        )
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        list(GET DOWNLOAD_STATUS 1 ERROR_MESSAGE)
        if(STATUS_CODE)
            message(SEND_ERROR "Failed to download ${ROM_FILENAME}. Error message: ${ERROR_MESSAGE}")
        endif()
    endif()

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ROM_FILENAME}")
        file(COPY
            "${CMAKE_CURRENT_SOURCE_DIR}/${ROM_FILENAME}"
            DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
    endif()
endforeach()
