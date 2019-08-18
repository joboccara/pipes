cmake_minimum_required(VERSION 3.10)

project(pipes)

add_library(${PROJECT_NAME} INTERFACE)
add_library(joboccara::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)

target_include_directories(${PROJECT_NAME} INTERFACE include)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
