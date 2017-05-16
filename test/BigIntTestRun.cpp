//
// Created by matech on 2017. 05. 13..
//

#include <gtest/gtest.h>
#include "../src/BigInt.h"

using digit_t = BigInt<0>::digit_t;

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
struct TwoSidedOperationBigIntTestDataBase{
    BigInt<S> a;
    BigInt<S> b;

    TwoSidedOperationBigIntTestDataBase(const std::array<digit_t , S> &a,
                                    const std::array<digit_t , S> &b) {
        this->a = BigInt<S>(a);
        this->b = BigInt<S>(b);
    }
    TwoSidedOperationBigIntTestDataBase(const std::vector<digit_t> &a,
                                    const std::vector<digit_t> &b) {
        this->a = BigInt<S>(a);
        this->b = BigInt<S>(b);
    }
};

//region base4 struct

template <int S>
struct Base4BigIntTestData : public TwoSidedOperationBigIntTestDataBase<S>{
    BigInt<S> res;

    Base4BigIntTestData(const std::array<digit_t, S> &a,
                        const std::array<digit_t, S> &b,
                        const std::array<digit_t, S> &res) : TwoSidedOperationBigIntTestDataBase<S>(a, b) {
        this->res = BigInt<S>(res);
    }
    Base4BigIntTestData(const std::vector<digit_t> &a,
                        const std::vector<digit_t> &b,
                        const std::vector<digit_t> &res) : TwoSidedOperationBigIntTestDataBase<S>(a, b) {
        this->res = BigInt<S>(res);
    }
};


template <int S>
struct Base4BigIntTestRun : public BigIntTestRun<S>, ::testing::WithParamInterface<Base4BigIntTestData<S>> {
    BigInt<S> res;
public:
    Base4BigIntTestRun() {
        const Base4BigIntTestData<S> &values = ::testing::WithParamInterface<Base4BigIntTestData<S>>::GetParam();
        this->a = values.a;
        this->b = values.b;
        res = values.res;
    }
};

//endregion

//region Addition

struct Addition : public Base4BigIntTestRun<2> {};
TEST_P(Addition, addition) {
    EXPECT_EQ(a+b, res);
}
using arr2_t = std::array<digit_t, 2>;
using vec_t = std::vector<digit_t>;
INSTANTIATE_TEST_CASE_P(Default, Addition, testing::Values(
        Base4BigIntTestData<2>(arr2_t {1, 2}, arr2_t {1, 2}, arr2_t {2, 4}),
        Base4BigIntTestData<2>(vec_t {1,2}, vec_t {1,2}, vec_t {2,4}),
        Base4BigIntTestData<2>(arr2_t {0, 1}, arr2_t {0, maxDigitValue}, arr2_t {1, 0}),
        Base4BigIntTestData<2>(arr2_t {1256, 3598}, arr2_t {25630, maxDigitValue}, arr2_t {26887, 3597})
));

//endregion

//region Addition

struct Substraction : public Base4BigIntTestRun<2> {};
TEST_P(Substraction, substraction) {
    EXPECT_EQ(a-b, res);
}
using arr2_t = std::array<digit_t, 2>;
using vec_t = std::vector<digit_t>;
INSTANTIATE_TEST_CASE_P(Default, Substraction, testing::Values(
        Base4BigIntTestData<2>(arr2_t {1, 2}, arr2_t {1, 2}, arr2_t {0, 0}),
        Base4BigIntTestData<2>(vec_t {1,2}, vec_t {1,2}, vec_t {0,0}),
        Base4BigIntTestData<2>(arr2_t {25630, maxDigitValue}, arr2_t {1256, 3598}, arr2_t {24374, 4294963697})
));

//endregion

//region Multiplication

struct Multiplication : public Base4BigIntTestRun<4> {};
TEST_P(Multiplication, multiplication) {
    EXPECT_EQ(a*b, res);
}
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, Multiplication, testing::Values(
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 1, 2}, arr4_t {0, 1, 4, 4}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 0, maxDigitValue}, arr4_t {0, 0, 0, maxDigitValue}, arr4_t {0, 0, 4294967294, 1})
));

//endregion

//region Multiplication

struct Modulo : public Base4BigIntTestRun<4> {};
TEST_P(Modulo, modulo) {
    EXPECT_EQ(a%b, res);
}
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, Modulo, testing::Values(
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 3}, arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 0, 1}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 0}, arr4_t {0, 0, 0, 2}, arr4_t {0, 0, 0, 0})
));

//endregion

//region Shift

template<int S>
struct ModuloBITD{
    BigInt<S> a;
    int b;
    BigInt<S> res;
};
TEST(Shift, shift) {
    ModuloBITD<4> data[] = {{arr4_t {0, 0, maxDigitValue, maxDigitValue}, 5, arr4_t {0, 0, maxDigitValue>>5, maxDigitValue}},
                         {arr4_t {0, 0, 13, 3}, 2, arr4_t {0, 0, 3,1073741824}}};
    for(auto rec : data) {
        EXPECT_EQ(rec.a >> rec.b, rec.res);
    }
}

//endregion

//region compare struct

template <int S>
struct CompareBigIntTestData : public TwoSidedOperationBigIntTestDataBase<S>{
    bool res;

    CompareBigIntTestData(const std::array<digit_t, S> &a,
                          const std::array<digit_t, S> &b,
                          bool res) : TwoSidedOperationBigIntTestDataBase<S>(a, b) {
        this->res = res;
    }
    CompareBigIntTestData(const std::vector<digit_t> &a,
                          const std::vector<digit_t> &b,
                          bool res) : TwoSidedOperationBigIntTestDataBase<S>(a, b) {
        this->res = res;
    }
};

template <int S>
struct CompareBigIntTestRun : public BigIntTestRun<S>, ::testing::WithParamInterface<CompareBigIntTestData<S>> {
    bool res;
public:
    CompareBigIntTestRun() {
        const CompareBigIntTestData<S> &values = ::testing::WithParamInterface<CompareBigIntTestData<S>>::GetParam();
        this->a = values.a;
        this->b = values.b;
        res = values.res;
    }
};

//endregion

//region GreaterThan

struct GreaterThan : public CompareBigIntTestRun<4> {};
TEST_P(GreaterThan, greater_than) {
    EXPECT_EQ(a>b, res);
}
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, GreaterThan, testing::Values(
        CompareBigIntTestData<4>(arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 1, 2}, false),
        CompareBigIntTestData<4>(arr4_t {0, 0, maxDigitValue, maxDigitValue}, arr4_t {0, 0, 0, maxDigitValue}, true)
));

//endregion