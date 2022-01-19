cmake_minimum_required(VERSION 3.21)
project(game2 C)

set(CMAKE_C_STANDARD 11)


add_executable(game2 main.c)
add_executable(createenemy createenemy.c)
add_executable(bullet bullet.c)
add_executable(Prop Prop.c)

target_link_libraries(game2 mingw32 SDL2main SDL2)
target_link_libraries(game2 mingw32 SDL2_image SDL2)
target_link_libraries(game2 mingw32 SDL2_ttf SDL2)



