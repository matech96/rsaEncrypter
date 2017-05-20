//
// Created by matech on 2017. 05. 20..
//

#include "RSAEncrypter.h"

num_t RSAEncrypter::generatePrime(const num_t &max) {
    num_t pr = num_t().fillRandom(max);
    do {
        ++pr;
        cout << "Generated num = " << pr << endl;
    } while (!pr.isPrime());
    return pr;
}

RSAEncrypter::RSAEncrypter() {
//    srand(time(nullptr));
    clock_t begin = clock();

    std::array<num_t::digit_t, num_length> digits{0};
    for (int i = num_length - key_length; i < num_length; ++i) {
        digits[i] = ~num_t::digit_t(0);
    }
    num_t max = num_t(digits);
    p = generatePrime(max);
    q = generatePrime(max);
    N = p * q;
    clock_t end = clock();
    cout << "Ellaspes time : " << (end-begin) << endl;
    begin = clock();
    generatePublicKey();
    end = clock();
    cout << c << endl;
    cout << "Public key ellaspes time : " << (end-begin) << endl;
}

num_t RSAEncrypter::generatePublicKey() {
    num_t fiN = (p - 1) * (q - 1);
    c = num_t().fillRandom(fiN);
    do {
        ++c;
        cout << "Generated num = " << c << endl;
    } while (c.greatesCommonDevider(fiN) != 1);
}
