#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>
#include <limits>
#include <optional>

namespace Caches
{

template<typename Page_indexT, typename Page_dataT>
class IdealCache 
{
public:
    using index_type = Page_indexT;
    using data_type  = Page_dataT;

    std::unordered_map<Page_indexT, std::queue<size_t>> input_data; 
    size_t                                              current_position;

private:
    size_t                                              capacity;        
    size_t                                              size;
    std::unordered_map<Page_indexT, Page_dataT>         cache;

    void remove_farthest()
    {
        Page_indexT victim  = cache.begin()->first;
        size_t      max_pos = 0;
        const size_t INF    = std::numeric_limits<size_t>::max();

        for (const auto& [key, _] : cache) 
        {
            auto       it         = input_data.find(key);
            const bool has_future = (it != input_data.end() && !it->second.empty());
            size_t     next_pos   = has_future ? it->second.front() : INF;

            if (next_pos >= max_pos) 
            {
                max_pos = next_pos;
                victim  = key;
            }
        }
        cache.erase(victim);
    }
  
public:
    IdealCache(size_t capacity_) : capacity(capacity_), size(0), current_position(0) {} 

    auto begin()       { return cache.begin(); }
    auto end()         { return cache.end();   }
    auto begin() const { return cache.begin(); }
    auto end()   const { return cache.end();   }

    std::optional<Page_dataT> fetch(const Page_indexT& index)
    {
        auto it = cache.find(index);
        if (it == cache.end()) 
            return std::nullopt;


        auto qit = input_data.find(index);
        if (qit != input_data.end() && !qit->second.empty())
            qit->second.pop();

        return it->second; 
    }

    void store(const Page_indexT& index, const Page_dataT& data)
    {
        auto qit = input_data.find(index);
        if (qit != input_data.end() && !qit->second.empty())
            qit->second.pop(); 


        if (cache.find(index) != cache.end())
            return;

        const bool no_future = (qit == input_data.end() || qit->second.empty());
        if (no_future)
            return;

        if (cache.size() < capacity) 
        {
            cache.emplace(index, data);
            return;
        }

        remove_farthest();
        cache.emplace(index, data);
    }

    void cache_dump() const
    { 
        std::cout << "Capacity: " << capacity << std::endl;
        std::cout << "Size: "     << size     << std::endl;

        for (auto &[key, page] : cache)
            std::cout << "Index: " << key << " Data: " << page << std::endl;
    }
};

} // namespace Caches
