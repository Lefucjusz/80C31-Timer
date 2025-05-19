add_library(gui INTERFACE)

target_sources(gui
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/gui.c
)

target_include_directories(gui
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)
