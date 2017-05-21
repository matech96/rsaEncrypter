#include <iostream>
#include "BigInt.h"
#include "RSAEncrypter.h"

extern template class BigInt<2>;

using namespace std;
int main() {
    RSAEncrypter encrypter = RSAEncrypter();
    cerr << encrypter.generatePublicKey() << endl;
    cerr << encrypter.generatePrivateKey() << endl;
    int c[] = {2,3,4};
    for(char i : c) {
        num_t s = encrypter.encrypt(num_t(i));
        s = encrypter.decrypt(s);
        cerr << s << endl;
    }
    return 0;

}