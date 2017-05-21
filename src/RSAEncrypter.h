//
// Created by matech on 2017. 05. 20..
//

#ifndef SSHENCRYPTER_RSAENCRYPTER_H
#define SSHENCRYPTER_RSAENCRYPTER_H

#include "BigInt.h"

const int num_length = 8;
const int key_length = num_length/8;
using num_t = BigInt<num_length>;
/**
 * Can encrypt and decrypt numbers.
 */
class RSAEncrypter {
public:
    num_t N;
    num_t p;
    num_t q;
    num_t c;
    num_t d;

    /**
     * Sets up the encrypter, generates the prime and public keys.
     */
    RSAEncrypter();

    /**
     * Enrypts the given number.
     * @param mess The number to encrypt.
     * @return
     */
    num_t encrypt(const num_t &mess) const;
    /**
     * Decrypts a previously encrypted number.
     * @param mess The number to decrypt.
     * @return
     */
    num_t decrypt(const num_t &mess) const;

private:
    /**
     * Generates a new public key. generatePublicKey() should be calld after
     * @return
     */
    num_t generatePublicKey();
    /**
     * Calculates the private key for the already set public key.
     * @return
     */
    num_t generatePrivateKey();
    /**
     * Generates a prime number, that is smaller than max.
     * @param max
     * @return prime
     */
    num_t generatePrime(const num_t &max);
    num_t maxPrime() const;
};


#endif //SSHENCRYPTER_RSAENCRYPTER_H
