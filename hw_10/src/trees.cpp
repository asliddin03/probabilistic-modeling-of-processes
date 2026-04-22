#include "trees.h"
#include <stdexcept>

long long unlabeledTreesCount(int n) {
    static const long long a[] = {
        0,
        1LL,
        1LL,
        1LL,
        2LL,
        3LL,
        6LL,
        11LL,
        23LL,
        47LL,
        106LL,
        235LL,
        551LL,
        1301LL,
        3159LL,
        7741LL,
        19320LL,
        48629LL,
        123867LL,
        317955LL,
        823065LL,
        2144505LL,
        5623756LL,
        14828074LL,
        39299897LL,
        104636890LL,
        279793450LL,
        751065460LL,
        2023443032LL,
        5469566585LL,
        14830871802LL
    };

    if (n < 1 || n > 30) {
        throw std::out_of_range("Для F поддерживаются n от 1 до 30.");
    }

    return a[n];
}