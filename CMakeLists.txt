cmake_minimum_required(VERSION 2.8)

project(benchy)

option(BENCHY_BUILD_APP "Build main benchy application" ON)
option(BENCHY_BUILD_MODULES "Build benchy modules" ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-extras/;${CMAKE_MODULE_PATH}")

if(BENCHY_BUILD_APP)
    add_subdirectory(app)
endif()

if(BENCHY_BUILD_MODULES)
    add_subdirectory(modules)
endif()

