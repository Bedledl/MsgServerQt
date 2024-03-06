
add_library(common STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/chat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chatGui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appGui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/configParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/message.cpp
)

target_include_directories(common PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common PUBLIC connConfigurator Qt6::Core Qt6::Widgets)
