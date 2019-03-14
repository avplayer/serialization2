cmake_minimum_required (VERSION 2.8)

option(ENABLE_TEST "enablle unittest" OFF)
option(ENABLE_PROTOBUF_EXAMPLE "protobuf example" OFF)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(examples)

if (ENABLE_TEST)
	enable_testing()
	include(GNUInstallDirs)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
	add_subdirectory(test)
endif()

