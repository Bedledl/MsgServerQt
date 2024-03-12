cmake_minimum_required(VERSION 3.10)

project(MessageServer LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif()

set(AUTOMOC ON) # sannsyngligvis unnötig
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
if(CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        include(CodeCoverage)
        append_coverage_compiler_flags()
        setup_target_for_coverage_lcov(
            NAME unittest_coverage
            EXECUTABLE testClient)
    else()
        message( STATUS "No debug build, so not generating coverage report")
    endif()
else()
    message( STATUS "Not using GCC, so not generating coverage report")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(LaunchServer mainServer.cpp)
add_executable(LaunchClient mainClient.cpp)

add_subdirectory(communication)
add_subdirectory(connConfigurator)
add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
target_link_libraries(LaunchServer Server Qt::Network Qt6::Widgets)
target_link_libraries(LaunchClient Client Qt::Network Qt6::Widgets)

set_property(TARGET LaunchServer PROPERTY AUTOMOC ON)


install(TARGETS LaunchServer DESTINATION bin)
