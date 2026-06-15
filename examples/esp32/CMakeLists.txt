cmake_minimum_required(VERSION 3.16)

set(DEFAULT_APP_TYPE "isf15acp4_comprehensive_test" CACHE STRING "Default app type")
set(DEFAULT_BUILD_TYPE "Release" CACHE STRING "Default build type")
set(DEFAULT_SOURCE_FILE "isf15acp4_comprehensive_test.cpp" CACHE STRING "Default source file")

if(NOT DEFINED APP_TYPE)
    message(FATAL_ERROR
        "APP_TYPE not defined. Use ./scripts/build_app.sh <app_type> <build_type>")
endif()

if(NOT DEFINED BUILD_TYPE)
    message(FATAL_ERROR
        "BUILD_TYPE not defined. Use ./scripts/build_app.sh <app_type> <build_type>")
endif()

message(STATUS "Building app type: ${APP_TYPE}")
message(STATUS "Build type: ${BUILD_TYPE}")

set(APP_TYPE "${APP_TYPE}" CACHE STRING "App type to build" FORCE)
set(BUILD_TYPE "${BUILD_TYPE}" CACHE STRING "Build type" FORCE)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_isf15acp4_${APP_TYPE}_app)
