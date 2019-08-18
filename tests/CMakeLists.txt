cmake_minimum_required(VERSION 3.10)

project(pipes_test)

add_executable(${PROJECT_NAME}
    main.cpp
    custom.cpp
    dev_null.cpp
    do_then.cpp
    map_aggregator.cpp
    set_aggregator.cpp
    sorted_inserter.cpp
    demux.cpp
    filter.cpp
    partition.cpp
    switch.cpp
    tee.cpp
    transform.cpp
    unzip.cpp
    integration_tests.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE joboccara::pipes)
