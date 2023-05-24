cmake_minimum_required(VERSION 3.26)
project(IRA)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lmpfr -lgmp -g -O0")

add_executable(ira main.cpp)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -L ./lib -lmpfr -lgmp -g -O0")

add_subdirectory(lib/Catch2)
add_executable(tests tests/SpaceShip_Tester.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)
catch_discover_tests(tests)
