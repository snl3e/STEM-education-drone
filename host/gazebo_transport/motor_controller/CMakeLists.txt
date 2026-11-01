cmake_minimum_required(VERSION 3.16)

project(imu_reader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GZ_TRANSPORT REQUIRED 
    IMPORTED_TARGET gz-transport13)
pkg_check_modules(GZ_MSGS REQUIRED 
    IMPORTED_TARGET gz-msgs10)

add_executable(motor_controller
    src/main.cpp)

target_link_libraries(motor_controller
    PRIVATE
        PkgConfig::GZ_TRANSPORT
        PkgConfig::GZ_MSGS)


    

