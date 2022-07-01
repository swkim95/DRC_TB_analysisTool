#include <cstdio>
#include <algorithm>
#include <vector>
#include <numeric>
#include <TPad.h>
#include <utility>
#include <iostream>
// #include <location.h>

void test() {
    std::cout << BASE_DIR << std::endl;
    std::cout << DATA_DIR << std::endl;
    std::cout << chInput.size() << std::endl;

    for ( int i = 0; i < chInput.size(); i++ ) {
        std::cout << chInput.at(i) << std::endl;
    }
}