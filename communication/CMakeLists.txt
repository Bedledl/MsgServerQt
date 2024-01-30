find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/serverClientCommunication.proto)
protobuf_generate_cpp(SERVER_MSG_PROTO_SRCS SERVER_MSG_PROTO_HDRS proto/serverMsgFormats.proto)
protobuf_generate_cpp(CLIENT_MSG_PROTO_SRCS CLIENT_MSG_PROTO_HDRS proto/clientMsgFormats.proto)

add_library(Communication STATIC
    ${PROTO_SRCS} ${PROTO_HDRS}
    ${SERVER_MSG_PROTO_SRCS} ${SERVER_MSG_PROTO_HDRS}
    ${CLIENT_MSG_PROTO_SRCS} ${CLIENT_MSG_PROTO_HDRS}
)

target_link_libraries(Communication ${Protobuf_LIBRARIES})

target_include_directories(Communication PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
)
