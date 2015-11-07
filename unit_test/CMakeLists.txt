########## Auto Updated From Python ###########

cmake_minimum_required(VERSION 2.8)

set (unit_test_SOURCES
	src/catch.hpp
	src/test_table.cpp
	src/test_template.cpp
	src/unit_testing.h
	src/unit_testing_main.cpp
)

add_executable(unit_test ${unit_test_SOURCES})
target_link_libraries(unit_test fsom)
target_include_directories(unit_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
