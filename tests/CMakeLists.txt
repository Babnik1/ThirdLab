find_package( GTest REQUIRED )

set ( SOURCES_TESTS
        testmain.cpp 
)


add_executable( custom_memory_tests ${SOURCES_TESTS} )
target_include_directories( custom_memory_tests PRIVATE ${CMAKE_SOURCE_DIR} )
target_link_libraries( custom_memory_tests GTest::gtest GTest::gtest )

add_test( NAME AllCustomMemoryTest COMMAND custom_memory_tests )