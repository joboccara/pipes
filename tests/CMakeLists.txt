add_executable(pipes_test
    main.cpp
    adjacent.cpp
    cartesian_product.cpp
    combinations.cpp
    dev_null.cpp
    do_then.cpp
    drop.cpp
    drop_while.cpp
    for_each.cpp
    fork.cpp
    map_aggregator.cpp
    set_aggregator.cpp
    insert.cpp
    intersperse.cpp
    filter.cpp
    join.cpp
    mux.cpp
    override.cpp
    partition.cpp
    streams.cpp
    stride.cpp
    switch.cpp
    take.cpp
    take_while.cpp
    tap.cpp
    tee.cpp
    transform.cpp
    unzip.cpp
    integration_tests.cpp)
add_test(NAME pipes_test COMMAND pipes_test)

target_include_directories(pipes_test PRIVATE
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(pipes_test PRIVATE cxx_std_14)

target_compile_options(pipes_test
                       PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<CXX_COMPILER_ID:MSVC>:/WX>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>)
