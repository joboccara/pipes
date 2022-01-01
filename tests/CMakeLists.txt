add_executable(pipes_test
    main.cpp
    adjacent.cpp
    buffered_transform.cpp
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

string(REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "/O2" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "/O2" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "/O2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "/O2" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_MINSIZE "${CMAKE_CXX_FLAGS_MINSIZE}")
string(REPLACE "/O2" "" CMAKE_CXX_FLAGS_MINSIZE "${CMAKE_CXX_FLAGS_MINSIZE}")
target_compile_options(pipes_test
                       PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<CXX_COMPILER_ID:MSVC>:/WX>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>)
