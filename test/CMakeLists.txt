include_directories(googletest/googletest/include)

set(SOURCE_FILES gtest.cpp BigIntTestRun.cpp ../src/BigInt.h)
add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests gtest gtest_main)