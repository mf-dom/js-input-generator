cmake_minimum_required(VERSION 3.16)
project(js_input_generator)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_SOURCE_DIR}/cmake/ProtobufMutator.cmake)
set(PROTOBUF_ROOT ${PBM_PATH}-build/external.protobuf)

add_executable(js_input_generator library.cpp schema.cpp)
if (CMAKE_C_COMPILER MATCHES "emcc$")
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set_target_properties(js_input_generator PROPERTIES LINK_FLAGS "-s WASM=0 --no-entry --bind -s ALLOW_MEMORY_GROWTH=1 -s MALLOC=emmalloc -s EXPORT_ES6=1 -s MODULARIZE=1 -s FILESYSTEM=0")
else ()
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)

    target_link_libraries(js_input_generator Threads::Threads)
endif ()

add_dependencies(js_input_generator ${PBM_PREFIX})
message(${ProtobufMutator_INCLUDE_DIRS})
target_include_directories(js_input_generator PUBLIC ${ProtobufMutator_INCLUDE_DIRS} ${PROTOBUF_ROOT}/include)
target_link_libraries(js_input_generator ${ProtobufMutator_LIBRARIES})
target_link_libraries(js_input_generator ${PROTOBUF_ROOT}/lib/libprotobufd.a)
