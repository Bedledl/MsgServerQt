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

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(LaunchServer mainServer.cpp)
#add_executable(LunchClient mainClient.cpp)

add_subdirectory(communication)
add_subdirectory(connConfigurator)

add_subdirectory(server)
#add_subdirectory(client)
target_link_libraries(LaunchServer Server Qt::Network Qt6::Widgets)
#target_link_libraries(LaunchClient Client)

set_property(TARGET LaunchServer PROPERTY AUTOMOC ON)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12)
        message(FATAL_ERROR "GCC version must be at least 12! Currently: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
        message(FATAL_ERROR "Clang version must be at least 16! Currently: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()
