find_package(GTest CONFIG REQUIRED)
find_package(GMock CONFIG REQUIRED)

include(GoogleTest)

add_library(testUtils STATIC utils/buildClientCmd.cpp utils/buildServerCmd.cpp)
target_include_directories(testUtils PUBLIC utils/include)
target_link_libraries(testUtils Qt6::Core Communication common)

add_executable(testClient client/testClientMessageBuilder.cpp client/testRawMessageProcessor.cpp)
add_executable(testServer server/testRawMessageProcessor.cpp server/testServerMessageBuilder.cpp)

target_include_directories(testClient PRIVATE client)
target_include_directories(testServer PRIVATE server)

target_link_libraries(testClient common Client testUtils GTest::gtest_main GTest::gmock_main)
target_link_libraries(testServer common Server testUtils GTest::gtest_main GTest::gmock_main)

gtest_discover_tests(testClient)
gtest_discover_tests(testServer)
