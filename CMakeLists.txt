cmake_minimum_required(VERSION 3.15)

# initialize pico-sdk from submodule
# MUST come before project()
include(pico-sdk/pico_sdk_init.cmake)

project(motor-ctrl
    VERSION 0.1.0
    LANGUAGES C CXX)

# no non-standard language extensions
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# GPS test
add_executable(motors)

pico_generate_pio_header(motors ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder.pio)

target_sources(motors PRIVATE motors.cpp)

# pull in common dependencies
target_link_libraries(motors pico_stdlib pico_multicore hardware_uart hardware_pio hardware_timer hardware_pwm)

# direct serial output to USB rather than UART
pico_enable_stdio_usb(motors 1)
pico_enable_stdio_uart(motors 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(motors)