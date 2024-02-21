find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(testCommon common/chatTest.cpp common/participantTest.cpp common/messageTest.cpp)

target_link_libraries(testCommon common GTest::gtest_main)

gtest_discover_tests(testCommon)
