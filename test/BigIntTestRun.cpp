//
// Created by matech on 2017. 05. 13..
//

#include <gtest/gtest.h>
#include "../src/BigInt.h"

using digit_t = BigInt<0>::digit_t;
using arr2_t = std::array<digit_t, 2>;
using vec_t = std::vector<digit_t>;
using arr4_t = std::array<digit_t, 4>;

template <int S>
struct BigIntTestRun : testing::Test {
    BigInt<S> a;
    BigInt<S> b;

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
struct PrimeData {
    BigInt<S> a;
    bool res;
};
TEST(Prime_T, isPrime) {
    PrimeData<2> data[] = {PrimeData<2> {BigInt<2> {arr2_t {1,2}}, false}};
    for (PrimeData<2> item : data) {
        EXPECT_EQ(item.a.isPrime(), item.res);
    }
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
INSTANTIATE_TEST_CASE_P(Default, Addition, testing::Values(
        Base4BigIntTestData<2>(arr2_t {1, 2}, arr2_t {1, 2}, arr2_t {2, 4}),
        Base4BigIntTestData<2>(vec_t {1,2}, vec_t {1,2}, vec_t {2,4}),
        Base4BigIntTestData<2>(arr2_t {0, 1}, arr2_t {0, maxDigitValue}, arr2_t {1, 0}),
        Base4BigIntTestData<2>(arr2_t {1256, 3598}, arr2_t {25630, maxDigitValue}, arr2_t {26887, 3597})
));

//endregion

//region Substraction

struct Substraction : public Base4BigIntTestRun<2> {};
TEST_P(Substraction, substraction) {
    EXPECT_EQ(a-b, res);
}
using arr2_t = std::array<digit_t, 2>;
using vec_t = std::vector<digit_t>;
INSTANTIATE_TEST_CASE_P(Default, Substraction, testing::Values(
        Base4BigIntTestData<2>(arr2_t {1, 2}, arr2_t {1, 2}, arr2_t {0, 0}),
        Base4BigIntTestData<2>(arr2_t {1, 2}, arr2_t {0, 0}, arr2_t {1, 2}),
        Base4BigIntTestData<2>(arr2_t {0, 1}, arr2_t {0, 0}, arr2_t {0, 1}),
        Base4BigIntTestData<2>(vec_t {1,2}, vec_t {1,2}, vec_t {0,0}),
        Base4BigIntTestData<2>(arr2_t {25630, maxDigitValue}, arr2_t {1256, 3598}, arr2_t {24374, 4294963697})
));

//endregion

//region Substraction4

struct Substraction4 : public Base4BigIntTestRun<4> {};
TEST_P(Substraction4, substraction4) {
    EXPECT_EQ(a-b, res);
}
using arr2_t = std::array<digit_t, 2>;
using vec_t = std::vector<digit_t>;
INSTANTIATE_TEST_CASE_P(Default, Substraction4, testing::Values(
        Base4BigIntTestData<4>(arr4_t {0,0,0, 1}, arr4_t {0,0,0, 0}, arr4_t {0,0,0, 1}),
        Base4BigIntTestData<4>(arr4_t {0,0,25630, maxDigitValue}, arr4_t {0,0,1256, 3598}, arr4_t {0,0,24374, 4294963697})
));

//endregion

//region Multiplication

struct Multiplication : public Base4BigIntTestRun<4> {};
TEST_P(Multiplication, multiplication) {
    EXPECT_EQ(a*b, res);
}
INSTANTIATE_TEST_CASE_P(Default, Multiplication, testing::Values(
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 1, 2}, arr4_t {0, 1, 4, 4}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 0, 0}, arr4_t {0, 0, 0, 0}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 0, maxDigitValue}, arr4_t {0, 0, 0, maxDigitValue}, arr4_t {0, 0, 4294967294, 1}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 0, 127}, arr4_t {0, 0, 0, 233}, arr4_t {0, 0, 0, 29591}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 0, 233}, arr4_t {0, 0, 0, 127}, arr4_t {0, 0, 0, 29591})
));

//endregion

//region Multiplication1

struct Multiplication1 : public Base4BigIntTestRun<1> {};
TEST_P(Multiplication1, multiplication1) {
    EXPECT_EQ(a*b, res);
}
INSTANTIATE_TEST_CASE_P(Default, Multiplication1, testing::Values(
        Base4BigIntTestData<1>(vec_t{127}, vec_t{233}, vec_t{29591}),
        Base4BigIntTestData<1>(vec_t{233}, vec_t{127}, vec_t{29591})
));

//endregion

//region Modulo

struct Modulo : public Base4BigIntTestRun<4> {};
TEST_P(Modulo, modulo) {
    EXPECT_EQ(a%b, res);
}
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, Modulo, testing::Values(
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 3}, arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 0, 1}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 0, 1}, arr4_t {0, 0, 0, 1}, arr4_t {0, 0, 0, 0}),
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
TEST(Shift, shiftRight) {
    ModuloBITD<4> data[] = {{arr4_t {0, 0, maxDigitValue, maxDigitValue}, 5, arr4_t {0, 0, maxDigitValue>>5, maxDigitValue}},
                            {arr4_t {0, 0, 13, 3}, 2, arr4_t {0, 0, 3,1073741824}}};
    for(auto rec : data) {
        EXPECT_EQ(rec.a >> rec.b, rec.res);
    }
}
TEST(Shift, shiftLeft) {
    ModuloBITD<4> data[] = {{arr4_t {0, 0, maxDigitValue, maxDigitValue}, 5, arr4_t {0, 31, maxDigitValue, maxDigitValue<<5}},
                            {arr4_t {0, maxDigitValue, maxDigitValue, maxDigitValue}, 5, arr4_t {31, maxDigitValue, maxDigitValue, maxDigitValue<<5}},
                            {arr4_t {0, 0, 13, 3}, 2, arr4_t {0, 0, 13*4,3*4}}};
    for(auto rec : data) {
        EXPECT_EQ(rec.a << rec.b, rec.res);
    }
}

//endregion

//region GreatesCommonDevider

struct GreatesCommonDevider : public Base4BigIntTestRun<4> {};
TEST_P(GreatesCommonDevider, greatesCommonDevider) {
    EXPECT_EQ(a.greatesCommonDevider(b), res);
}
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, GreatesCommonDevider, testing::Values(
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 1, 2}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 1, 0}, arr4_t {0, 0, 0, 2}, arr4_t {0, 0, 0, 2}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 13, 2}, arr4_t {0, 0, 131, 2}, arr4_t {0, 0, 0, 2}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 13, 2}, arr4_t {0, 0, 131, 1}, arr4_t {0, 0, 0, 3}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 13, 2}, arr4_t {0, 0, 11, 1}, arr4_t {0, 0, 0, 9}),
        Base4BigIntTestData<4>(arr4_t {0, 0, 13, 2}, arr4_t {0, 0, 121, 1}, arr4_t {0, 0, 0, 1})
));

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

//region threeArgument struct

template <int S>
struct threeArgumentBigIntTestData : public TwoSidedOperationBigIntTestDataBase<S>{
    BigInt<S> c;
    BigInt<S> res;

    threeArgumentBigIntTestData(const std::array<digit_t, S> &a,
                                const std::array<digit_t, S> &b,
                                const std::array<digit_t, S> &c,
                                const std::array<digit_t, S> &res) : TwoSidedOperationBigIntTestDataBase<S>(a, b) {
        this->c = c;
        this->res = res;
    }
    threeArgumentBigIntTestData(const vec_t &a,
                                const vec_t &b,
                                const vec_t &c,
                                const vec_t &res) : TwoSidedOperationBigIntTestDataBase<S>(a, b) {
        this->c = c;
        this->res = res;
    }
};

template <int S>
struct threeArgumentBigIntTestRun : public BigIntTestRun<S>, ::testing::WithParamInterface<threeArgumentBigIntTestData<S>> {
    BigInt<S> c;
    BigInt<S> res;
public:
    threeArgumentBigIntTestRun() {
        const threeArgumentBigIntTestData<S> &values = ::testing::WithParamInterface<threeArgumentBigIntTestData<S>>::GetParam();
        this->a = values.a;
        this->b = values.b;
        this->c = values.c;
        this->res = values.res;
    }
};

//endregion

//region PowerModulo4

struct PowerModulo4 : public threeArgumentBigIntTestRun<4> {};
TEST_P(PowerModulo4, powerModulo4) {
    EXPECT_EQ(a.powerModulo(b, c), res);
}
using arr4_t = std::array<digit_t, 4>;
INSTANTIATE_TEST_CASE_P(Default, PowerModulo4, testing::Values(
        threeArgumentBigIntTestData<4>(arr4_t {0, 0, 0, 1}, arr4_t {0, 1, 1, 2}, arr4_t {0, 0, 1, 2}, arr4_t {0, 0, 0, 1}),
        threeArgumentBigIntTestData<4>(arr4_t {0, 0, 43, 2}, arr4_t {0, 0, 0, 123}, arr4_t {0, 0, 0, 2}, arr4_t {0, 0, 0, 0})
));

//endregion