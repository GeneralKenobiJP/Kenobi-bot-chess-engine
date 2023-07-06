cmake_minimum_required(VERSION 3.0.0)
project(Kenobi-bot-chess-engine VERSION 0.1.0 LANGUAGES C CXX)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "include/SFML/lib/")
#set(CMAKE_PREFIX_PATH "include/SFML/CMake/")

include(CTest)
enable_testing()

add_executable(Kenobi-bot-chess-engine main.cpp)

find_package(SFML COMPONENTS window graphics system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(Kenobi-bot-chess-engine sfml-network sfml-audio sfml-graphics sfml-window sfml-system)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
