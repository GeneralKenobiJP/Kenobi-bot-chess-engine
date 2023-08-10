find_package(SFML COMPONENTS window graphics system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_library(engine-lib Search.h VirtualBoard.h VirtualMoveTable.h VirtualEvaluation.h Search.cpp VirtualBoard.cpp VirtualMoveTable.cpp VirtualEvaluation.cpp)

target_link_libraries(engine-lib sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
#add_executable(app-files board.h piece.h test.h board.cpp piece.cpp test.cpp)