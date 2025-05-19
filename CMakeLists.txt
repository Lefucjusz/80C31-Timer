cmake_minimum_required(VERSION 3.25)

set(CMAKE_C_STANDARD 99)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)

project(80C31-Timer C)

add_executable(${PROJECT_NAME} main.c)

add_subdirectory(system)
add_subdirectory(hd44780)
add_subdirectory(encoder)
add_subdirectory(gui)
add_subdirectory(relay)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        system
        hd44780
        encoder
        gui
        relay
)

# Convert IHX to HEX
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND packihx ${PROJECT_NAME}.ihx > ${PROJECT_NAME}.hex
)

# Convert IHX to BIN
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND objcopy -Iihex -Obinary ${PROJECT_NAME}.ihx ${PROJECT_NAME}.bin
)

# Convert BIN to C array – not needed if you have a "real" (E)EPROM programmer.
# I'm "temporarily" (for the past few years...) using a quick and dirty Arduino-based 
# solution, where the data to be programmed is just pasted as a hex array into the 
# source code.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_SOURCE_DIR}/converter/conv ${PROJECT_NAME}.bin ${PROJECT_NAME}.txt
)
