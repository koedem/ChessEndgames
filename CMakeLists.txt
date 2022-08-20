cmake_minimum_required(VERSION 3.23)
project(ChessEndgames)

set(CMAKE_CXX_STANDARD 20)

# Process Abseil's CMake build system
add_subdirectory(abseil-cpp)

add_executable(ChessEndgames main.cpp PawnBoard.h TranspositionTable.h)

# Declare dependency on the absl::strings library
target_link_libraries(ChessEndgames absl::flat_hash_map)
