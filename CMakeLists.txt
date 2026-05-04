cmake_minimum_required(VERSION 3.16)
project(BearStacker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(game
    src/main.cpp
    src/block.cpp
    src/game.cpp
    src/gameLogic.cpp
    src/gameRender.cpp
)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)

target_link_libraries(game PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
)

file(COPY ${CMAKE_SOURCE_DIR}/bear DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/font DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Sound DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/background2.png DESTINATION ${CMAKE_BINARY_DIR})