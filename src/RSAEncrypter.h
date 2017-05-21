//
// Created by matech on 2017. 05. 20..
//

#ifndef SSHENCRYPTER_RSAENCRYPTER_H
#define SSHENCRYPTER_RSAENCRYPTER_H

#include "BigInt.h"

const int num_length = 8;
const int key_length = num_length/8;
using num_t = BigInt<num_length>;
class RSAEncrypter {
public:
    num_t N;
    num_t p;
    num_t q;
    num_t c;
    num_t d;

    RSAEncrypter();

    num_t generatePrime(const num_t &max);
    num_t generatePublicKey();
    num_t generatePrivateKey();
    num_t encrypt(const num_t &mess) const;
    num_t decrypt(const num_t &mess) const;

private:
    num_t maxPrime() const;
};


#endif //SSHENCRYPTER_RSAENCRYPTER_H
