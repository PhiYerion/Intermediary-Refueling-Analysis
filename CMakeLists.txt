cmake_minimum_required(VERSION 3.18)
project(IRA)

add_subdirectory(lib/Catch2)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lmpfr -lgmp -g -O0 -DQT_WIDGETS_LIB -I/usr/include/qt/QtWidgets -I/usr/include/qt -I/usr/include/qt/QtCore -DQT_GUI_LIB -I/usr/include/qt/QtGui -DQT_CORE_LIB -lQt5Widgets -lQt5Core -lQt5Gui")

add_executable(tests tests/Tester.cpp SpaceShip.cpp Engine.cpp Stage.cpp)
add_executable(ira main.cpp SpaceShip.cpp Engine.cpp Stage.cpp Window.cpp)


include(CTest)
include(Catch)
catch_discover_tests(tests)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(tests PRIVATE mpfr Catch2::Catch2WithMain)
target_link_libraries(ira   PRIVATE mpfr Qt5::Widgets)

