set(AUTOMOC ON) # wsl unnötig
set(CMAKE_AUTOMOC ON)

add_library(common STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/chat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/configParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/message.cpp
)

target_include_directories(common PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(qml)

target_link_libraries(common PUBLIC
    connConfigurator
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)
