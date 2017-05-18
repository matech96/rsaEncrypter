#include <iostream>
#include "BigInt.h"
#include "Prime.h"

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

    cout << 15129% 511 << endl;
    return 0;

}