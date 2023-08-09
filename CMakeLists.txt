cmake_minimum_required(VERSION 3.0.0)
project(Kenobi-bot-chess-engine VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 17)
#target_compile_features(Kenobi-bot-chess-engine PUBLIC cxx_std_17)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "include/SFML/lib/")
#set(CMAKE_PREFIX_PATH "include/SFML/CMake/")

include(CTest)
enable_testing()

#include_directories(src/app)

#file(GLOB app-executables RELATIVE "src/app" "*.cpp")
#file(GLOB app-headers RELATIVE "src/app" "*.h")

#set(app-executables 
#src/app/piece.cpp
#src/app/board.cpp
#src/app/test.cpp)

#set(app-headers
#src/app/piece.h
#src/app/board.h
#src/app/test.h)


#add_subdirectory(src/kenobi-lib)

add_executable(Kenobi-bot-chess-engine main.cpp)

find_package(SFML COMPONENTS window graphics system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
include_directories(src/app/)
include_directories(src/engine/)
add_subdirectory(src/app)
add_subdirectory(src/engine)
target_link_libraries(Kenobi-bot-chess-engine sfml-network sfml-audio sfml-graphics sfml-window sfml-system app-lib engine-lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
