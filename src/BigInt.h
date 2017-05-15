//
// Created by matech on 2017. 05. 13..
//

#ifndef SSHENCRYPTER_BIGINT_H
#define SSHENCRYPTER_BIGINT_H

#include <array>
#include <ostream>

using digit_t = uint32_t;

template<int S>
class BigInt {
    using mulipl_t = uint64_t;
    const static int multCarryShift = 32;

    std::array<digit_t, S> digits = std::array<digit_t, S>();
public:
    BigInt() {}

    BigInt(const std::array<digit_t, S> &digits) : digits(digits) {}

    //region Operators

    BigInt<S> & operator=(const BigInt<S> &o) {
        digits = o.digits;
    }

    bool operator!=(const BigInt &rhs) const {
        return !(rhs == *this);
    }

    bool operator==(const BigInt &rhs) const {
        return digits == rhs.digits;
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
                middleValues[j].digits[j - (S-1-i)] += static_cast<digit_t>(mi);
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

    friend std::ostream &operator<<(std::ostream &os, const BigInt &anInt) {
        for (auto item : anInt.digits) {
            os << item << " ";
        }
        return os;
    }

    //endregion

};


#endif //SSHENCRYPTER_BIGINT_H
