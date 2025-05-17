cmake_minimum_required(VERSION 3.27)
project(MedievalWar C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -Wuninitialized -pedantic -g)

add_executable(game src/data.c src/main.c)
target_include_directories(game PRIVATE include)
target_link_libraries(game -lncurses -lm)





