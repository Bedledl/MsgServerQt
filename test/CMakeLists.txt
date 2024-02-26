find_package(GTest CONFIG REQUIRED)
find_package(GMock CONFIG REQUIRED)

include(GoogleTest)

add_executable(testClient client/clientCommunicatorTest.cpp client/clientTest.cpp client/buildServerCmd.cpp)
add_executable(testServer server/serverCommunicatorTest.cpp server/uniqueKeyTest.cpp server/buildClientCmd.cpp)

target_include_directories(testClient PRIVATE client)
target_include_directories(testServer PRIVATE server)

target_link_libraries(testClient common Client GTest::gtest_main GTest::gmock_main)
target_link_libraries(testServer common Server GTest::gtest_main GTest::gmock_main)

gtest_discover_tests(testClient)
gtest_discover_tests(testServer)
