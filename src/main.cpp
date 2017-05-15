#include <iostream>
#include <vector>
#include <bitset>
#include "BigInt.h"

extern template class BigInt<2>;

int main() {
//    const int64_t overflowMask = ((int64_t)1) << 63;
//    int64_t n = 0;
//    if (n & overflowMask) {
//        std::cout << "negative "<< std::endl;
//    } else {
//        std::cout << "positive"<< std::endl;
//    }
    uint64_t a = 0x00000000ffffffff;

    uint64_t x = a*a;
    std::cout << (x&0x00000000ffffffff) << std::endl;
    std::cout << (x>>32) << std::endl;
    return 0;
}