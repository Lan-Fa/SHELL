cmake_minimum_required(VERSION 3.29)
project(interactive_shell)

set(CMAKE_CXX_STANDARD 20 -fsanitize=address)

find_library(READLINE_LIBRARIES NAMES readline)
find_path(READLINE_INCLUDE_DIR NAMES readline/readline.h)

add_executable(interactive_shell main.cpp
        main.h
        builtin_functions.cpp
        builtin_functions.h
        pathinfo.cpp
        pathinfo.h
        command.cpp
        command.h
)

target_link_libraries(interactive_shell PRIVATE ${READLINE_LIBRARIES})

target_include_directories(interactive_shell PRIVATE ${READLINE_INCLUDE_DIR})