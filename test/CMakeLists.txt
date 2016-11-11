cmake_minimum_required (VERSION 2.8)

project(unittest)

find_package(GTest REQUIRED)

include_directories(../include
	${GTEST_INCLUDE_DIRS}
	)

set(UNITTEST_SOURCES
	jsontest.cpp
	unittest.cpp
	)

add_executable(unittest
	${UNITTEST_SOURCES}
	)

target_link_libraries(unittest ${GTEST_BOTH_LIBRARIES} )

add_test(NAME unittest 
	COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unittest 
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	)

