cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif()

set(AUTOMOC ON) # sannsyngligvis unnötig
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(MessageServer LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets Gui Qml Quick)
qt_standard_project_setup()

add_executable(LaunchServer mainServer.cpp)
add_executable(LaunchClient mainClient.cpp)

add_subdirectory(communication)
add_subdirectory(connConfigurator)
add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(test)
target_link_libraries(LaunchServer Server Qt::Network Qt6::Widgets)
target_link_libraries(LaunchClient Client Qt::Network Qt6::Widgets commonGui)

set_property(TARGET LaunchServer PROPERTY AUTOMOC ON)

install(TARGETS LaunchServer DESTINATION bin)
