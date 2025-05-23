set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR mcs51)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_C_FLAGS "-V -mmcs51 --verbose --std-c99 --model-small --acall-ajmp --fsigned-char --stack-auto --opt-code-size --code-size 2048")
set(CMAKE_C_COMPILER sdcc)

set(CMAKE_FIND_ROOT_PATH ~/sdcc-4.5.0)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
