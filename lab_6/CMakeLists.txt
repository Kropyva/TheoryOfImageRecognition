cmake_minimum_required(VERSION 3.0.0)
project(lab_6 VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

add_executable(lab_6 main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(lab_6 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})