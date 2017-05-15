//
// Created by matech on 2017. 05. 13..
//

#include <gtest/gtest.h>
#include "../src/BigInt.h"

template <int S>
struct BigIntTestRun : testing::Test {
    BigInt<S> a = BigInt<S>(std::array<digit_t , S> {1,2});
    BigInt<S> b = BigInt<S>(std::array<digit_t, S> {2,4});

};
TEST(BigInt_NotParametarized, equaliti) {
    BigInt<2> a = BigInt<2>(std::array<digit_t, 2> {1,2});
    BigInt<2> b = BigInt<2>(std::array<digit_t, 2> {2,4});
    EXPECT_EQ(a, a);
    EXPECT_EQ(a, BigInt<2>(std::array<digit_t, 2> {1,2}));
    EXPECT_FALSE(a == b);
}

const digit_t maxDigitValue = ~(static_cast<digit_t>(0));
TEST(BigInt_NotParametarized, addition_overflow) {
    BigInt<2> a = BigInt<2>(std::array<digit_t, 2> {1, 2});
    BigInt<2> b = BigInt<2>(std::array<digit_t, 2> {maxDigitValue,4 });
    EXPECT_THROW(a+b, std::overflow_error);
    b = BigInt<2>(std::array<digit_t, 2> {maxDigitValue, maxDigitValue });
    EXPECT_THROW(a+b, std::overflow_error);
}

template <int S>
struct TwoSidedOperationBigIntTestData{
    BigInt<S> a;
    BigInt<S> b;
    BigInt<S> res;

    TwoSidedOperationBigIntTestData(const std::array<digit_t , S> &a,
                   const std::array<digit_t , S> &b,
                   const std::array<digit_t , S> &res) {
        this->a = BigInt<S>(a);
        this->b = BigInt<S>(b);
        this->res = BigInt<S>(res);
    }
};


template <int S>
struct TwoSidedOperationBigIntTestRun : public BigIntTestRun<S>, ::testing::WithParamInterface<TwoSidedOperationBigIntTestData<S>> {
    BigInt<S> res;
public:
    TwoSidedOperationBigIntTestRun() {
        const TwoSidedOperationBigIntTestData<S> &values = ::testing::WithParamInterface<TwoSidedOperationBigIntTestData<S>>::GetParam();
        this->a = values.a;
        this->b = values.b;
        res = values.res;
    }
};

struct Addition : public TwoSidedOperationBigIntTestRun<2> {};
struct Multiplication : public TwoSidedOperationBigIntTestRun<4> {};

TEST_P(Addition, addition) {
    EXPECT_EQ(a+b, res);
}
TEST_P(Multiplication, multiplication) {
    EXPECT_EQ(a*b, res);
}
//
using arr2_t = std::array<digit_t, 2>;
INSTANTIATE_TEST_CASE_P(Default, Addition, testing::Values(
        TwoSidedOperationBigIntTestData<2>(arr2_t {1,2}, arr2_t {1,2}, arr2_t {2,4}),
        TwoSidedOperationBigIntTestData<2>(arr2_t {0,1}, arr2_t {0, maxDigitValue}, arr2_t {1,0}),
        TwoSidedOperationBigIntTestData<2>(arr2_t {1256,3598}, arr2_t {25630, maxDigitValue}, arr2_t {26887,3597})
));
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, Multiplication, testing::Values(
        TwoSidedOperationBigIntTestData<4>(arr4_t {0,0,1,2}, arr4_t {0,0,1,2}, arr4_t {0,1,4,4}),
        TwoSidedOperationBigIntTestData<4>(arr4_t {0,0,0,maxDigitValue}, arr4_t {0,0,0,maxDigitValue}, arr4_t {0,0,4294967294,1})
));
