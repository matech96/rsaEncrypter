#include <iostream>
#include "BigInt.h"
#include "Prime.h"
#include "RSAEncrypter.h"

extern template class BigInt<2>;

using namespace std;
int main() {
//    const int64_t overflowMask = ((int64_t)1) << 63;
//    int64_t n = 0;
//    if (n & overflowMask) {
//        std::cout << "negative "<< std::endl;
//    } else {
//        std::cout << "positive"<< std::endl;
//    }

    srand(time(nullptr));
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