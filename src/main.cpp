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

    srand(time(nullptr));
    for (int i = 0; i < 15; ++i) {
        cout << BigInt<5>().fillRandom(BigInt<5>(std::array<BigInt<5>::digit_t, 5>{500, 2, 3, 4, 5})) << endl;
    }
    return 0;

}