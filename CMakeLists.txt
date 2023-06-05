cmake_minimum_required(VERSION 3.18)
project(IRA)

add_subdirectory(lib/Catch2)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lmpfr -lgmp -g -O0")

add_executable(tests tests/Tester.cpp SpaceShip.cpp Engine.cpp Stage.cpp)
add_executable(ira main.cpp SpaceShip.cpp Engine.cpp Stage.cpp)


include(CTest)
include(Catch)
catch_discover_tests(tests)

target_link_libraries(tests PRIVATE mpfr Catch2::Catch2WithMain)
target_link_libraries(ira   PRIVATE mpfr)

