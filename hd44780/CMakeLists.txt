add_library(hd44780 INTERFACE)

target_sources(hd44780
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/hd44780.c
)

target_include_directories(hd44780
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)
