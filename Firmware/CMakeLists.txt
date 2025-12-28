cmake_minimum_required(VERSION 3.13)

# Fetch pico SDK from Git
message(STATUS "Fetching pico SDK from git (might take a while on first build)...")
set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(YDR7_Keyboard_Firmware)

pico_sdk_init()

file(GLOB_RECURSE FIRMWARE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(Keyboard_Firmware
    ${FIRMWARE_SOURCES}
)

target_include_directories(Keyboard_Firmware
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

pico_generate_pio_header(Keyboard_Firmware ${CMAKE_CURRENT_LIST_DIR}/src/lib/ws2812/ws2812.pio)


target_link_libraries(Keyboard_Firmware 
    pico_stdlib
    pico_multicore
    hardware_i2c
    hardware_pio
    hardware_adc
    tinyusb_device
    tinyusb_board
)

pico_add_extra_outputs(Keyboard_Firmware)