#Libfsom Folder

cmake_minimum_required(VERSION 2.8.11)
cmake_policy(SET CMP0043 NEW)

project(LIBFSOM)

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -stdlib=libc++ -std=c++11 ")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -stdlib=libc++ -std=c++11 ")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -stdlib=libc++ -std=c++11 ")
else(NOT MSVC)
	# Enable parallel compilation on windows
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(NOT MSVC)

add_subdirectory(src)
add_subdirectory(unit_test)

