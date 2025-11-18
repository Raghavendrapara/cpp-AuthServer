cmake_minimum_required(VERSION 3.10)
project(AuthServer)

set(CMAKE_CXX_STANDARD 17)

# 1. Find Dependencies (gRPC + Crypto++)
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(c-ares REQUIRED)

# 2. Find the Protobuf Compiler
get_target_property(gRPC_CPP_PLUGIN_EXECUTABLE gRPC::grpc_cpp_plugin LOCATION)

# 3. Generate gRPC Code from .proto
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/protos/auth.proto")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
    OUTPUT "${GENERATED_DIR}/auth.pb.cc"
           "${GENERATED_DIR}/auth.pb.h"
           "${GENERATED_DIR}/auth.grpc.pb.cc"
           "${GENERATED_DIR}/auth.grpc.pb.h"
    COMMAND protobuf::protoc
    ARGS --grpc_out="${GENERATED_DIR}"
         --cpp_out="${GENERATED_DIR}"
         --plugin=protoc-gen-grpc="${gRPC_CPP_PLUGIN_EXECUTABLE}"
         -I "${CMAKE_CURRENT_SOURCE_DIR}/protos"
         "${PROTO_FILE}"
    DEPENDS "${PROTO_FILE}"
)

# 4. Define SERVER Executable
add_executable(AuthServer 
    src/main.cpp 
    src/User.cpp
    "${GENERATED_DIR}/auth.pb.cc"
    "${GENERATED_DIR}/auth.grpc.pb.cc"
)

target_include_directories(AuthServer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GENERATED_DIR}
)

target_link_libraries(AuthServer PRIVATE 
    gRPC::grpc++ 
    protobuf::libprotobuf 
    cryptopp::cryptopp
)

# 5. Define CLIENT Executable (Test Tool)
add_executable(ClientTool 
    src/AuthClient.cpp
    "${GENERATED_DIR}/auth.pb.cc"
    "${GENERATED_DIR}/auth.grpc.pb.cc"
)

target_include_directories(ClientTool PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GENERATED_DIR}
)

target_link_libraries(ClientTool PRIVATE 
    gRPC::grpc++ 
    protobuf::libprotobuf 
    cryptopp::cryptopp
)