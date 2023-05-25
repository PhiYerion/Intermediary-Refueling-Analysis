cmake_minimum_required(VERSION 3.18)
project(IRA)

set(CMAKE_CXX_STANDARD 14)

add_executable(ira main.cpp SpaceShip.cpp Engine.cpp Stage.cpp)

add_subdirectory(lib/Catch2)
add_executable(tests tests/SpaceShip_Tester.cpp)
include(CTest)
include(Catch)
catch_discover_tests(tests)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(ira mpfr)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lmpfr -lgmp -g -O0")
