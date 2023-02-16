cmake_minimum_required(VERSION 3.2)

project(word_converter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HDR "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(word_converter ${HDR} ${SRC} main.cpp)
target_include_directories(word_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# third-party library (tclap-1.2.5)
target_include_directories(word_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/tclap)

#GCC and MinGW support
target_link_libraries(word_converter pthread -lstdc++fs)
