#include <iostream>
#include <vector>

#include "t_pool_allocator.h"

unsigned int factorial(unsigned int n) {
    if (n == 0) return 1;
    return n * factorial(n - 1);
}

int main(int argc, char const* argv[]) {
    
    constexpr int arraySize = 10;
    std::vector<int, TPoolAllocator<int, 10>> v;
    
    for (int i = 0; i < arraySize; ++i) {
         v.push_back(factorial(i));
         std::cout << v.at(i) << std::endl;
    }
}