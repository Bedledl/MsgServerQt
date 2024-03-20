find_package(GTest CONFIG REQUIRED)
find_package(GMock CONFIG REQUIRED)

include(GoogleTest)

add_executable(testClient client/testClientMessageBuilder.cpp client/testRawMessageProcessor.cpp utils/buildClientCmd.cpp utils/buildServerCmd.cpp)
add_executable(testServer utils/buildClientCmd.cpp utils/buildServerCmd.cpp)

target_include_directories(testClient PRIVATE client)
target_include_directories(testServer PRIVATE server)

target_include_directories(testClient PRIVATE utils/include)
target_include_directories(testServer PRIVATE utils/include)

target_link_libraries(testClient common Client GTest::gtest_main GTest::gmock_main)
target_link_libraries(testServer common Server GTest::gtest_main GTest::gmock_main)

gtest_discover_tests(testClient)
gtest_discover_tests(testServer)
