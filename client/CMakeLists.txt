add_library(Client STATIC
${CMAKE_CURRENT_SOURCE_DIR}/communicators.cpp
${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
${CMAKE_CURRENT_SOURCE_DIR}/chatPreviewModel.cpp
${CMAKE_CURRENT_SOURCE_DIR}/chatViewModel.cpp
${CMAKE_CURRENT_SOURCE_DIR}/messageModelItem.cpp
)

target_include_directories(Client PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(qml)

target_link_libraries(Client PUBLIC common commonGui Communication Qt6::Core Qt6::Network Qt6::Widgets)
