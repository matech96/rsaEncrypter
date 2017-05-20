//
// Created by matech on 2017. 05. 20..
//

#include "RSAEncrypter.h"

num_t RSAEncrypter::generatePrime(const num_t &max) {
    num_t p = num_t().fillRandom(max);
    do {
        ++p;
        cout << p << endl;
    } while (!p.isPrime());
    return p;
}

RSAEncrypter::RSAEncrypter() {
    srand(time(nullptr));
    std::array<num_t::digit_t, key_length> digits{0};
    for (int i = key_length / 1; i < key_length; ++i) {
        digits[i] = ~num_t::digit_t(0);
    }
    num_t max = num_t(digits);
    num_t p = generatePrime(max);
    num_t q = generatePrime(max);
    N = p * q;
}
