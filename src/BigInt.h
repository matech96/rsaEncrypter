//
// Created by matech on 2017. 05. 13..
//

#ifndef SSHENCRYPTER_BIGINT_H
#define SSHENCRYPTER_BIGINT_H

#include <vector>
#include <ostream>
#include <array>
#include <unordered_set>
#include <bitset>
#include <iostream>

using namespace std;

template<int S>
class BigInt {
public:
    using digit_t = uint32_t;
protected:
    using mulipl_t = uint64_t;
    const static int multCarryShift = 32;

    std::array<digit_t, S> digits = std::array<digit_t, S>();
public:
    BigInt() {}

    BigInt(int a) {
        digits[S - 1] = a;
    }

    BigInt(const std::array<digit_t, S> &digits) : digits(digits) {}

    BigInt(const std::vector<digit_t> &digits) {
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
            this->digits[i] = digits[i];
        }
    }

    //region Operators

    BigInt<S> &operator=(const BigInt<S> &o) {
        digits = o.digits;
        return *this;
    }

    bool operator!=(const BigInt &rhs) const {
        return !(rhs == *this);
    }

    bool operator==(const BigInt &rhs) const {
        return digits == rhs.digits;
    }

    bool operator<(const BigInt &rhs) const {
        for (int i = 0; i < S; ++i) {
            auto rd = rhs.digits[i];
            auto td = this->digits[i];
            if (td != rd) {
                return td < rd;
            }
        }
        return false;
    }

    bool operator>(const BigInt &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const BigInt &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const BigInt &rhs) const {
        return !(*this < rhs);
    }

    template<int T>
    BigInt<S> &operator+=(const BigInt<T> &o) {
        uint8_t overflow = performAddition(o);
        handleOverflow<T>(overflow);
        return *this;
    }

    //region addition privates
private:
    template<int T>
    uint8_t performAddition(const BigInt<T> &o) {
        uint8_t carry = 0;
        for (int i = T - 1; i >= 0; --i) {
            digit_t &ti = this->digits[i];
            digit_t oi = o.digits[i];
            ti += oi;
            ti += carry;
            carry = (uint8_t) ((ti < oi) ? 1 : 0);
        }
        return carry;
    }

    template<int T>
    void handleOverflow(uint8_t carry) {
        if (carry) {
            if (S == T) {
                throw std::overflow_error("Numbers are too big");
            } else {
                this->digits[T] += carry;
            }
        }
    }

public:
    //endregion

    template<int T>
    BigInt<S> operator+(const BigInt<T> &o) const {
        BigInt<S> res(*this);
        res += o;
        return res;
    }


    BigInt &operator-=(const BigInt &o) {
        BigInt no = o;
        no.negate();
        try {
            *this += no;
        } catch (std::overflow_error e) {}
        return *this;
    }

    //region subtraction privates
private:
    BigInt &negate() {
        for (int i = 0; i < S; ++i) {
            digits[i] = ~digits[i];
        }
        *this += BigInt<S>(1);
        return *this;
    }

public:
    //endregion

    BigInt operator-(const BigInt &o) const {
        BigInt<S> res(*this);
        res -= o;
        return res;
    }

    template<int T>
    BigInt<S> operator*(const BigInt<T> &o) const {
        BigInt<S> middleValues[T];
        digit_t carry = 0;
        for (int i = S - 1; i >= 0; --i) {
            mulipl_t oi = o.digits[i];
            carry = 0;
            for (int j = T - 1; j > 0; --j) {
                mulipl_t ti = this->digits[j];
                mulipl_t mi = ti * oi;
                mi += carry;
                middleValues[j].digits[j - (S - 1 - i)] += static_cast<digit_t>(mi);
                carry = static_cast<digit_t>(mi >> multCarryShift);
            }
            if (carry) throw std::overflow_error("Too big numbers!");
        }
        BigInt<S> res;
        for (auto value : middleValues) {
            res += value;
        }
        return res;
    }

    BigInt operator%(const BigInt &m) const {
        BigInt left = 0;
        BigInt right = (*this) >> 1;
        while (left < right) {
            BigInt k = (left + right) >> 1;
            if (*this - m >= m * k) {
                left = k + BigInt<S>(1);
            } else {
                right = k;
            }
        }

        return *this - left * m;
    }

    BigInt operator>>(int n) const {
        BigInt<S> res;
        digit_t carry = 0;
        size_t n_non_carry_bits = sizeof(digit_t) * 8 - n;
        digit_t carry_mask = (~digit_t(0)) >> n_non_carry_bits;
        for (int i = 0; i < S; ++i) {
            digit_t carry_old = carry;

            res.digits[i] = digits[i];
            digit_t &rd = res.digits[i];

            carry = rd & carry_mask;
            carry <<= n_non_carry_bits;

            rd >>= n;
            rd += carry_old;
        }
        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &anInt) {
        const bool bitLike = false;
        for (auto item : anInt.digits) {
            if (bitLike) {
                std::bitset<32> s(item);
                os << s << " ";
            } else {
                os << item << " ";
            }
        }
        return os;
    }

    //endregion

    BigInt<S> greatesCommonDevider(const BigInt<S> &o) const {
        //TODO
        BigInt<S> a;
        BigInt<S> m;
        if (o < *this) {
            a = o;
            m = *this;
        }


    }

};


#endif //SSHENCRYPTER_BIGINT_H
