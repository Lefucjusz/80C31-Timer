add_library(relay INTERFACE)

target_sources(relay
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/relay.c
)

target_include_directories(relay
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)
