cmake_minimum_required(VERSION 3.10)

project(pipes
        VERSION 0.0.1
        DESCRIPTION "A header only C++ library to create pipelines operating on collections"
        HOMEPAGE_URL "https://github.com/joboccara/pipes")

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
                           INTERFACE
                           $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_14)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
