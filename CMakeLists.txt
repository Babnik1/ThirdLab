cmake_minimum_required( VERSION 3.12 )
option( ENABLE_TESTS "Enable tests" ON )

project( CustomMemory VERSION 1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( SOURCES 
        main.cpp 
)

add_executable( custom_memory ${SOURCES} )
set_target_properties( custom_memory PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

if ( ENABLE_TESTS )
    enable_testing()
    add_subdirectory( tests )
endif()
