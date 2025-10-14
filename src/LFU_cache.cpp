#include <cassert>
#include <iostream>

#include "LFU_cache.hpp"
#include "cache_driver.hpp"


static int slow_get_page_int(int page_id) { return page_id; }

int main() 
{
    std::size_t cache_capacity = 0;
    int         request_count  = 0;

    std::cin >> cache_capacity >> request_count;
    assert(std::cin.good());

    // Caches::LFUCache<int, int> cache{cache_capacity};
    // Caches::CacheDriver        driver{cache};
    Caches::CacheDriver<Caches::LFUCache<int,int>> driver(cache_capacity);

    int hit_count = 0;

    for (int i = 0; i < request_count; ++i)
    {
        int page_id = 0;
        std::cin >> page_id;
        assert(std::cin.good());

        if (driver.lookup_update(page_id, slow_get_page_int)) 
        {
            ++hit_count;
        }
    }

    std::cout << hit_count << std::endl;
    
    return 0;
}

