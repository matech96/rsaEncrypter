//
// Created by matech on 2017. 05. 20..
//

#ifndef SSHENCRYPTER_RSAENCRYPTER_H
#define SSHENCRYPTER_RSAENCRYPTER_H

#include "BigInt.h"

const int key_length = 4;
using num_t = BigInt<key_length>;
class RSAEncrypter {
public:
    num_t N;
    num_t c;
    num_t d;

    RSAEncrypter();

    num_t generatePrime(const num_t &max);
};


#endif //SSHENCRYPTER_RSAENCRYPTER_H
