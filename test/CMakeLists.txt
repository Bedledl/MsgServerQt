find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(testClient client/clientCommunicatorTest.cpp client/clientTest.cpp)

target_link_libraries(testClient common Client GTest::gtest_main)

gtest_discover_tests(testClient)
