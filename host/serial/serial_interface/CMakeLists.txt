cmake_minimum_required(VERSION 3.16)

project(serial_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    
add_executable(serial_interface
    src/main.cpp
    src/serial_port.cpp)

target_include_directories(serial_interface PRIVATE
    ../serial_interface/include
)
    

