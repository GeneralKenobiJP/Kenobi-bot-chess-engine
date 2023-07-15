find_package(SFML COMPONENTS window graphics system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_library(app-lib board.h piece.h test.h fenUtility.h SpriteHandler.h board.cpp piece.cpp test.cpp fenUtility.cpp SpriteHandler.cpp)

target_link_libraries(app-lib sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
#add_executable(app-files board.h piece.h test.h board.cpp piece.cpp test.cpp)