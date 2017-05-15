//
// Created by matech on 2017. 05. 15..
//

#include <gtest/gtest.h>
#include "../src/BigInt.h"
#include "../src/Prime.h"

const int primeSize = Prime::primeSize;
using BI_t = BigInt<primeSize>;
using arr_t = std::array<BI_t::digit_t, primeSize>;
using vec_t = std::vector<BI_t::digit_t>;
struct PrimeIsPrimeTestData {
    BigInt<primeSize> number;
    bool res;
};

struct PrimeFunctionsTest : public ::testing::TestWithParam<PrimeIsPrimeTestData> {

protected:
    BigInt<primeSize> number;
    bool res;
public:
    PrimeFunctionsTest() {
        auto p = GetParam();
        number = p.number;
        res = p.res;
    }
};

TEST_P(PrimeFunctionsTest, isPrimeF) {
    auto act = Prime::isPrime(number);
    EXPECT_EQ(act, res);
}

INSTANTIATE_TEST_CASE_P(PTD, PrimeFunctionsTest, testing::Values(
        PrimeIsPrimeTestData {BI_t(vec_t{0,2}), true}
));
