cmake_minimum_required(VERSION 3.13)

# Fetch pico SDK from Git
message(STATUS "Fetching pico SDK from git (might take a while on first build)...")
set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(YDR7_Keyboard_Firmware)

pico_sdk_init()

add_executable(Keyboard_Firmware
    src/main.cpp
)

target_include_directories(Keyboard_Firmware
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(Keyboard_Firmware 
    pico_stdlib
    tinyusb_device
    tinyusb_board
)

pico_add_extra_outputs(Keyboard_Firmware)