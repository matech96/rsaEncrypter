include_directories(googletest/googletest/include)

set(SOURCE_FILES gtest.cpp BigIntTestRun.cpp ../src/BigInt.h PrimeTestRun.cpp ../src/Prime.h ../src/Prime.cpp)
add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests gtest gtest_main)