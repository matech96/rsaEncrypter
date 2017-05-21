//
// Created by matech on 2017. 05. 20..
//

#include "RSAEncrypter.h"

num_t RSAEncrypter::generatePrime(const num_t &max) {
    num_t pr = num_t().fillRandom(max);
    do {
        ++pr;
//        cerr << "Generated num = " << pr << endl;
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
    cerr << "p= " << p << endl;
    cerr << "q= " << q << endl;
    cerr << "Ellaspes time : " << (end-begin) << endl;
    generatePublicKey();
    begin = clock();
    generatePrivateKey();
    end = clock();
    cerr << c << endl;
    cerr << "Private key ellaspes time : " << (end-begin) << endl;
}

num_t RSAEncrypter::generatePublicKey() {
    cerr << "Generating public key" << endl;
    num_t fiN = (p - 1) * (q - 1);
    c = num_t().fillRandom(fiN>>2);
    do {
        ++c;
        cerr << "Public key candidate= " << c << endl;
    } while (c.greatesCommonDevider(fiN) != 1);
}

num_t RSAEncrypter::encrypt(const num_t &mess) {
    return mess.powerModulo(c, N);
}

num_t RSAEncrypter::generatePrivateKey() {
    cerr << "Generating private key" << endl;
    num_t a = c;
    num_t b = 1;
    num_t m = N;

    num_t M = N;
    num_t s = 0;
    while (true) {
        cerr << "m: " << m << "a: " << a << "b: " << b << endl;
        num_t t = m / a;
        cerr << "t: " << t << endl;
        num_t r = m % a;
        cerr << "r: " << r << endl;
        if (r == 0) {
            d = b / a;
            return d;
        }
        while(true) {
            try {
                num_t tmp = (s - t * b) % M;
                break;
            } catch (std::overflow_error e) {
                s+=M;
            }
        }
        m = a;
        a = r;
        s = b;
        b = c;
    }
}
