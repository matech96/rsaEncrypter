//
// Created by matech on 2017. 05. 20..
//

#include "RSAEncrypter.h"

RSAEncrypter::RSAEncrypter() {
    srand(time(nullptr));

    num_t max = maxPrime();
    p = generatePrime(max);
    q = generatePrime(max);
    N = p * q;
    generatePublicKey();
    generatePrivateKey();
}

num_t RSAEncrypter::maxPrime() const {
    std::array<num_t::digit_t, num_length> digits{0};
    for (int i = num_length - key_length; i < num_length; ++i) {
        digits[i] = ~num_t::digit_t(0);
    }
    return num_t(digits);
}

num_t RSAEncrypter::generatePrime(const num_t &max) {
    num_t pr = num_t().fillRandom(max);
    do {
        ++pr;
    } while (!pr.isPrime());
    return pr;
}

num_t RSAEncrypter::generatePublicKey() {
    num_t fiN = (p - 1) * (q - 1);
    c = num_t().fillRandom(fiN);
    do {
        ++c;
    } while (c.greatesCommonDevider(fiN) != 1 && c != 1);
    return c;
}

num_t RSAEncrypter::encrypt(const num_t &mess) const {
    return mess.powerModulo(c, N);
}

num_t RSAEncrypter::generatePrivateKey() {
    num_t a = c;
    num_t b = 1;
    num_t m = (p - 1) * (q - 1);

    num_t M = m;
    num_t s = 0;
    while (true) {
        num_t t = m / a;
        num_t r = m % a;
        if (r == 0) {
            d = b / a;
            return d;
        }

        num_t tb = t * b;
        if (s < tb) {
            s += ((tb-s)/M + num_t(1))*M;
        }
        num_t tmp = (s - tb) % M;
        
        m = a;
        a = r;
        s = b;
        b = tmp;
    }
}

num_t RSAEncrypter::decrypt(const num_t &mess) const {
    return mess.powerModulo(d, N);
}
