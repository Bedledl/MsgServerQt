set(AUTOMOC ON) # wsl unnötig
set(CMAKE_AUTOMOC ON)

add_library(connConfigurator STATIC
${CMAKE_CURRENT_SOURCE_DIR}/connConfigGraphic.cpp
${CMAKE_CURRENT_SOURCE_DIR}/connConfigFile.cpp
)

target_include_directories(connConfigurator PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(connConfigurator PUBLIC config++ Qt6::Core Qt6::Network Qt6::Widgets)
