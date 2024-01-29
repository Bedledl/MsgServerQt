add_library(Client STATIC
${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
${CMAKE_CURRENT_SOURCE_DIR}/clientApp.cpp
)

target_include_directories(Client PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Client PUBLIC common Communication Qt6::Core Qt6::Network Qt6::Widgets)