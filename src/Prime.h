//
// Created by matech on 2017. 05. 15..
//

#ifndef SSHENCRYPTER_PRIME_H
#define SSHENCRYPTER_PRIME_H


#include "BigInt.h"

class Prime {
public:
    const static int primeSize = 64;

    bool static isPrime(const BigInt<primeSize> &n);
};


#endif //SSHENCRYPTER_PRIME_H
