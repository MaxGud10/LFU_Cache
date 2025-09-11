#include <iostream>

#include "LFU_cache.hpp"

int main() {
    size_t CacheSize    = 0;
    size_t ElementCount = 0;
    size_t HitCount     = 0;

    std::cin >> CacheSize;
    std::cin >> ElementCount;

    LFUCache<int, int> cache(CacheSize);

    for (size_t i = 0; i < ElementCount; i++) 
    {
        int element = 0;
        std::cin >> element;
        if (cache.get(element)) HitCount++;

        else 
            cache.put(element, element);
    }

    std::cout << HitCount << std::endl;

    return 0;
}