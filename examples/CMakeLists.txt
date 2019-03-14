cmake_minimum_required (VERSION 2.8)

project(serialize_example)

include_directories(../include)

add_executable(example
	example.cpp
	)

if(ENABLE_PROTOBUF_EXAMPLE)
	find_package(Protobuf REQUIRED)


	FILE(GLOB_RECURSE LibFiles "../include/*/*.h")
	add_custom_target(headers SOURCES ${LibFiles})

	file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
	PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
	include_directories(${CMAKE_CURRENT_BINARY_DIR})

	add_executable(example_protobuf
		example_protobuf.cpp
		${ProtoSources}
		${ProtoHeaders}
		)
	target_link_libraries(example_protobuf ${PROTOBUF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
endif()

