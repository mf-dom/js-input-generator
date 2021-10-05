# from LLVM, with modifications: https://github.com/llvm/llvm-project/blob/d480f968ad8b56d3ee4a6b6df5532d485b0ad01e/clang/cmake/modules/ProtobufMutator.cmake

include(ExternalProject)
set(PBM_PREFIX protobuf_mutator)
set(PBM_PATH ${CMAKE_CURRENT_BINARY_DIR}/${PBM_PREFIX}/src/${PBM_PREFIX})
set(PBM_LIB_PATH ${PBM_PATH}-build/src/libprotobuf-mutator.a)
set(PBM_FUZZ_LIB_PATH ${PBM_PATH}-build/src/libfuzzer/libprotobuf-mutator-libfuzzer.a)
set(PROTOBUF_ROOT ${PBM_PATH}-build/external.protobuf)

ExternalProject_Add(${PBM_PREFIX}
  PREFIX ${PBM_PREFIX}
  GIT_REPOSITORY https://github.com/google/libprotobuf-mutator.git
  GIT_TAG master
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DLIB_PROTO_MUTATOR_DOWNLOAD_PROTOBUF:BOOL=on -DLIB_PROTO_MUTATOR_TESTING:BOOL=off
  CMAKE_CACHE_ARGS -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
  BUILD_BYPRODUCTS ${PBM_LIB_PATH} ${PBM_FUZZ_LIB_PATH} ${PROTOBUF_ROOT}/lib/libprotobufd.a ${PROTOBUF_ROOT}/lib/libprotobuf.a
  PATCH_COMMAND patch CMakeLists.txt ${CMAKE_SOURCE_DIR}/libprotobuf-mutator.patch
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
  )

set(ProtobufMutator_INCLUDE_DIRS ${PBM_PATH})
set(ProtobufMutator_LIBRARIES ${PBM_FUZZ_LIB_PATH} ${PBM_LIB_PATH})
