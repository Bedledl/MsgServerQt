set(AUTOMOC ON) # wsl unnötig
set(CMAKE_AUTOMOC ON)

add_library(common STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/chat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chatGui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appGui.cpp
)

target_include_directories(common PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common PUBLIC Qt6::Core Qt6::Widgets)
