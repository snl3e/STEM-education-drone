cmake_minimum_required(VERSION 3.16)

project(imu_reader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GZ_TRANSPORT REQUIRED 
    IMPORTED_TARGET gz-transport13)
pkg_check_modules(GZ_MSGS REQUIRED 
    IMPORTED_TARGET gz-msgs10)

add_executable(imu_reader
    src/main.cpp
    src/imu_callback.cpp)

target_link_libraries(imu_reader 
    PRIVATE
        PkgConfig::GZ_TRANSPORT
        PkgConfig::GZ_MSGS)

    

