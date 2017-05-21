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
#include <ctime>

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

    BigInt<S>(int a) {
        digits[S - 1] = a;
    }

    BigInt(const std::array<digit_t, S> &digits) : digits(digits) {}

    BigInt(const std::vector<digit_t> &digits) {
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
            this->digits[i] = digits[i];
        }
    }

    BigInt<S> &fillRandom(BigInt<S> max) {
        size_t rand_long = 15;
        for (int i = 0; i < S * multCarryShift + 1 && *this < max; i += rand_long) {
            *this += BigInt<S>(rand());
            *this <<= rand_long;
        }
        return (*this) %= max;
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
//        cerr << "Addition: " << *this << " / " << o << endl;
        uint8_t overflow = performAddition(o);
        handleOverflow<T>(overflow, o);
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
            carry = (uint8_t) ((ti < oi || ti < carry) ? 1 : 0);
        }
        return carry;
    }

    template<int T>
    void handleOverflow(uint8_t carry, const BigInt &o) {
        if (carry) {
            if (S == T) {
                throw std::overflow_error(
                        "Addition: Too big numbers: " + (std::string) (*this) + " + " + (std::string) (o));
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
        if (*this < o) throw std::overflow_error("Substraction: Second argument is greater than first! " + std::string(*this) + " - " + std::string(o));
        BigInt no = o;
        try {
            no.negate();
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
//        cerr << "Multiply: " << *this << " / " << o << endl;
        BigInt<S> middleValues[T];
        digit_t carry = 0;
        for (int i = S - 1; i >= 0; --i) {
            mulipl_t oi = o.digits[i];
            carry = 0;
            for (int j = T - 1; j >= 0; --j) {
                mulipl_t ti = this->digits[j];
                mulipl_t mi = ti * oi;
                mi += carry;
                middleValues[j].digits[j - (S - 1 - i)] += static_cast<digit_t>(mi);
                carry = static_cast<digit_t>(mi >> multCarryShift);
            }
            if (carry)
                throw std::overflow_error(
                        "Multiplication: Too big numbers: " + (std::string) (*this) + " * " + (std::string) (o));
        }
        BigInt<S> res;
        for (auto value : middleValues) {
            res += value;
        }
        return res;
    }

    BigInt<S> &operator/=(const BigInt<S> &d) {
//        cerr << "Devise: " << *this << " / " << d << endl;

        if (d == 1) return *this;

        BigInt left = 0;
        BigInt right = (*this) >> 1;
        while (left < right) {
            BigInt k = (left + right) >> 1;
//            cerr << left << " - " << right << " - " << k << " - " << d * k << endl;
            const BigInt &one = BigInt<S>(1);
            if (*this - d >= d * k) {
                left = k + one;
            } else {
                right = k;
            }
        }
        *this = left;
        return *this;
    }

    BigInt operator/(const BigInt &d) const {
        BigInt<S> res(*this);
        res /= d;
        return res;
    }

    BigInt operator%(const BigInt &m) const {
        BigInt<S> res(*this);
        res %= m;
        return res;
    }

    BigInt &operator%=(const BigInt &m) {
//        cerr << "Modulo: " << *this << " % " << m << endl;

        if (BigInt<S>(1) == m) {
            digits = std::array<digit_t, S>();
            return *this;
        }
        if (*this < m) return *this;

        BigInt k = *this/m;
        return *this -= k * m;
    }

    BigInt operator>>(int n) const {
        BigInt<S> res(*this);
        res >>= n;
        return res;
    }

    BigInt<S> &operator>>=(int n) {
        if (n > multCarryShift)
            throw std::length_error("n muss be smaller as multCarryShift = " + std::to_string(multCarryShift));

        digit_t carry = 0;
        size_t n_non_carry_bits = sizeof(digit_t) * 8 - n;
        digit_t carry_mask = (~digit_t(0)) >> n_non_carry_bits;
        for (int i = 0; i < S; ++i) {
            digit_t carry_old = carry;

            this->digits[i] = digits[i];
            digit_t &rd = this->digits[i];

            carry = rd & carry_mask;
            carry <<= n_non_carry_bits;

            rd >>= n;
            rd += carry_old;
        }
        return *this;
    }

    BigInt operator<<(int n) const {
        BigInt<S> res(*this);
        res <<= n;
        return res;
    }

    BigInt<S> &operator<<=(int n) {
        if (n > multCarryShift)
            throw std::length_error("n muss be smaller as multCarryShift = " + std::to_string(multCarryShift));

        digit_t carry = 0;
        size_t n_non_carry_bits = sizeof(digit_t) * 8 - n;
        digit_t carry_mask = (~digit_t(0)) << n_non_carry_bits;
        for (int i = S - 1; i >= 0; --i) {
            digit_t carry_old = carry;

            this->digits[i] = digits[i];
            digit_t &rd = this->digits[i];

            carry = rd & carry_mask;
            carry >>= n_non_carry_bits;

            rd <<= n;
            rd += carry_old;

//            std::cerr << *this << " c " << carry << endl;
        }
        return *this;
    }

    BigInt<S> &operator++() {
        BigInt<S> one = 1;
        *this += one;
        return *this;
    }

    operator std::string() const {
        const bool bitLike = false;
        std::string res = "";
        for (auto item : this->digits) {
            res += std::to_string(item) + " ";
        }
        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &anInt) {
        os << static_cast<std::string>(anInt);
        return os;
    }

    //endregion

    BigInt<S> greatesCommonDevider(const BigInt<S> &o) const {
//        cerr << "greatest common devider: (" << *this << " , " << o << ")" << endl;
        if (*this == 0 || o == 0) return 1;

        BigInt<S> a;
        BigInt<S> m;
        if (o < *this) {
            a = o;
            m = *this;
        } else {
            a = *this;
            m = o;
        }
        while (a != 0) {
//            cerr << a << " - " << m << endl;
            BigInt<S> r = m % a;
            m = a;
            a = r;
        }
        return m;
    }

    BigInt<S> powerModulo(BigInt<S> b, const BigInt<S> &m) const {
        BigInt<S> a = *this;
        a %= m;
        BigInt<S> c(1);
        while (true) {
            if (b.isOdd()) {
                c = (c * a) % m;
            }
            b = b >> 1;
            if (b == 0) {
                return c;
            }
            a = (a * a) % m;
        }
    }

    bool isOdd() const {
        return (this->digits[S - 1] & digit_t(1));
    }

    bool isPrime() const {
        if (!(this->isOdd()) || *this == 1) return false;

        int t = 0;
        BigInt<S> c = *this - 1;
        while (!c.isOdd()) {
            c >>= 1;
            ++t;
        }

        int n_trys = 300;
        for (int i = 0; i < n_trys; ++i) {
            BigInt<S> a;
            a.fillRandom(*this - 2);
            ++a;
//            cerr << "Random number: " << a << endl;
            if (a.greatesCommonDevider(*this) != 1) {
//                cerr << "cd found:" << a.greatesCommonDevider(*this) << endl;
                return false;
            }
            BigInt<S> ac = a.powerModulo(c, *this);
            if (ac != 1 && ac != *this - 1) {
                bool exist_1 = false;
                for (int j = 0; j < t; ++j) {
                    ac = ac.powerModulo(2, *this);
//                    std::cerr << a << "ac:" << ac << std::endl;
                    if (ac == *this - 1) {
                        exist_1 = true;
                        break;
                    }
                }

                if (!exist_1) {
                    return false;
                }
            }
        }
        return true;
    }
};


#endif //SSHENCRYPTER_BIGINT_H
