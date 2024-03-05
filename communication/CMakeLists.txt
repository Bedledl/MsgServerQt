find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/serverClientCommunication.proto)
protobuf_generate_cpp(CLIENT_MSG_PROTO_SRCS CLIENT_MSG_PROTO_HDRS proto/clientMsgFormats.proto)

add_library(Communication STATIC
    ${PROTO_SRCS} ${PROTO_HDRS}
    ${CLIENT_MSG_PROTO_SRCS} ${CLIENT_MSG_PROTO_HDRS}
    communicator.h
)

target_link_libraries(Communication ${Protobuf_LIBRARIES} protobuf::protobuf)

target_include_directories(Communication PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
)
