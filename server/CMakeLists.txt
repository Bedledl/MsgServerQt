set(AUTOMOC ON) # wsl unnötig
set(CMAKE_AUTOMOC ON)

add_library(Server STATIC
${CMAKE_CURRENT_SOURCE_DIR}/clientThreadWorker.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tcpserver.cpp
${CMAKE_CURRENT_SOURCE_DIR}/chat.cpp
${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
${CMAKE_CURRENT_SOURCE_DIR}/serverApp.cpp
)

target_include_directories(Server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Server PUBLIC common Communication connConfigurator Qt6::Core Qt6::Network Qt6::Widgets)
