cmake_minimum_required (VERSION 2.8)

project(serialize_example)

find_package(Protobuf REQUIRED)

include_directories(../include)

FILE(GLOB_RECURSE LibFiles "../include/*/*.h")
add_custom_target(headers SOURCES ${LibFiles})

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_library(proto_message STATIC ${ProtoSources} ${ProtoHeaders} ${PROTOBUF_LIBRARY})


add_executable(example_protobuf
	example_protobuf.cpp
	${ProtoSources}
	${ProtoHeaders}
	)
target_link_libraries(example_protobuf ${PROTOBUF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

add_executable(example_json
	example_json.cpp
	)

