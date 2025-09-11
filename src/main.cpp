#include <iostream>

#include "LFU_cache.hpp"

int main() {

    LFU_cache<int, int> cache(3);

    int data[][2] = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};

    for (size_t i = 0; i < sizeof(data) / (sizeof(int) * 2) - 1; i++) {
        cache.put(data[i][0], data[i][1]);
    }

    std::cout << *(cache.get(2)) << std::endl;
    std::cout << *(cache.get(1)) << std::endl;

    cache.put(data[3][0], data[3][1]);

    cache.dump();

    return 0;
}