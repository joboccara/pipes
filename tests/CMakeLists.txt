cmake_minimum_required(VERSION 3.12)

add_executable(pipes_test
    main.cpp
    cartesian_product.cpp
    custom.cpp
    dev_null.cpp
    do_then.cpp
    map_aggregator.cpp
    set_aggregator.cpp
    sorted_inserter.cpp
    demux.cpp
    filter.cpp
    join.cpp
    mux.cpp
    override.cpp
    partition.cpp
    streams.cpp
    switch.cpp
    take.cpp
    take_while.cpp
    tee.cpp
    transform.cpp
    unzip.cpp
    integration_tests.cpp)

target_include_directories(pipes_test PRIVATE
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(pipes_test PRIVATE cxx_std_14)

target_compile_options(pipes_test
                       PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<CXX_COMPILER_ID:MSVC>:/WX>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>)
