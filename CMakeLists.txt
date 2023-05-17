cmake_minimum_required(VERSION 3.26)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src
        main.cpp SpaceShip.h)

add_subdirectory(lib/Catch2)
add_executable(tests tests/SpaceShip_Tester.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
