#include <iostream>

#include "ideal_cache.hpp"

size_t ideal_cache_driver();



int main() 
{
    size_t HitCount = ideal_cache_driver();

    std::cout << HitCount << std::endl;

    return 0;
}

size_t ideal_cache_driver() 
{
    size_t CacheSize    = 0;
    size_t ElementCount = 0;
    size_t HitCount     = 0;

    std::cin >> CacheSize;
    std::cin >> ElementCount;

    IdealCache<int, int> cache(CacheSize);
    std::vector<int>     input_data;


    for (size_t position = 0; position < ElementCount; position++) 
    {
        int elem = 0;
        std::cin >> elem;
 
        input_data.push_back(elem);

        auto elem_info = cache.input_data.find(elem);

        if (elem_info == cache.input_data.end()) 
        {
            std::queue<size_t> position_queue;
            position_queue.push(position);

            cache.input_data.emplace(elem, position_queue);
        }
        else 
            elem_info->second.push(position);
    }

    for (int& element : input_data) 
    {
        if (cache.get(element) != cache.end()) HitCount++;
        else 
        {
            cache.put(element, element);
        }
        cache.current_position++;
    }

    return HitCount;
}