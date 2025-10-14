#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

#include "ideal_cache.hpp"
#include "cache_driver.hpp" 



static int slow_get_page_int(int page_id) { return page_id; }

int main() 
{
    std::size_t cache_capacity = 0;
    std::size_t request_count  = 0;

    std::cin >> cache_capacity;
    std::cin >> request_count;
    assert(std::cin.good());

    std::vector<int> requests;
    requests.reserve(request_count);
    for (std::size_t pos = 0; pos < request_count; ++pos) 
    {
        int page_id = 0;
        std::cin >> page_id;
        assert(std::cin.good());
        requests.push_back(page_id);
    }

    Caches::CacheDriver<Caches::IdealCache<int,int>> driver(cache_capacity);

    auto& cache = driver.cache();
    for (std::size_t pos = 0; pos < requests.size(); ++pos) 
    {
        int page_id = requests[pos];
        auto it = cache.input_data.find(page_id);

        if (it == cache.input_data.end()) 
        {
            std::queue<std::size_t> q;
            q.push(pos);
            cache.input_data.emplace(page_id, std::move(q));
        } 
        else 
            it->second.push(pos);
    }


    std::size_t hit_count = 0;
    for (std::size_t pos = 0; pos < requests.size(); ++pos) 
    {
        int page_id = requests[pos];

        if (driver.lookup_update(page_id, slow_get_page_int)) 
        {
            ++hit_count; 
        }

        ++cache.current_position;
    }

    std::cout << hit_count << std::endl;
    return 0;
}