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
    generatePublicKey();
    clock_t end = clock();
    cout << "Ellaspes time : " << (end-begin) << endl;
    begin = clock();
    generatePrivateKey();
    end = clock();
    cout << c << endl;
    cout << "Private key ellaspes time : " << (end-begin) << endl;
}

num_t RSAEncrypter::generatePublicKey() {
    cout << "Generating public key" << endl;
    num_t fiN = (p - 1) * (q - 1);
    c = num_t().fillRandom(fiN);
    do {
        ++c;
        cout << "Generated num = " << c << endl;
    } while (c.greatesCommonDevider(fiN) != 1);
}

num_t RSAEncrypter::encrypt(const num_t &mess) {
    return mess.powerModulo(c, N);
}

num_t RSAEncrypter::generatePrivateKey() {
    cout << "Generating private key" << endl;
    num_t a = c;
    num_t b = 1;
    num_t m = N;

    num_t M = N;
    num_t s = 0;
    while (true) {
        num_t t = m / a;
        cout << t << endl;
        num_t r = m % a;
        cout << r << endl;
        if (r == 0) {
            d = b / a;
            return d;
        }
        num_t tmp = (s - t * b) % M;
        m = a;
        a = r;
        s = b;
        b = c;
    }
}
