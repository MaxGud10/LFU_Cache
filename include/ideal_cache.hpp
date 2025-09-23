#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>
#include <limits>

template<typename Page_indexT, typename Page_dataT>
class IdealCache 
{
public:
    std::unordered_map<Page_indexT, std::queue<size_t>> input_data; 
    size_t                                              current_position;

private:
    size_t                                              capacity;        
    size_t                                              size;
    std::unordered_map<Page_indexT, Page_dataT>         cache;

    void remove_farthest() 
    {
        Page_indexT remove_index = cache.begin()->first;
        size_t      max_distance = 0;
        const size_t INF = std::numeric_limits<size_t>::max();

        for (auto& elem : cache) 
        {
            auto elem_info = input_data.find(elem.first);
            size_t elem_distance = (elem_info == input_data.end() || elem_info->second.empty())
                                   ? INF
                                   : elem_info->second.front();

            if (elem_distance >= max_distance) 
            {
                max_distance = elem_distance;
                remove_index = elem.first;
            }
        }

        cache.erase(remove_index);
        if (size > 0) --size;
    }
  
public:

    IdealCache(size_t capacity_) : capacity(capacity_), size(0), current_position(0) {} 

    auto begin() { return cache.begin(); }
    auto end()   { return cache.end();   }

    auto get(Page_indexT& index) 
    { 
        auto found_page = cache.find(index);

        if (found_page == cache.end()) 
            return cache.end();

        auto elem_info = input_data.find(index);
        if (elem_info != input_data.end() && !elem_info->second.empty())
            (elem_info->second).pop();

        return found_page;
    }

    void put(Page_indexT& index, Page_dataT& data) 
    {
        auto found_page = cache.find(index);
        auto elem_info  = input_data.find(index);

        if (elem_info != input_data.end() && !elem_info->second.empty())
            (elem_info->second).pop();

        if (found_page != cache.end())   return; 

        // если больше не будет обращений — можно не хранить
        if (elem_info == input_data.end() || elem_info->second.empty()) return;    

        if (size < capacity) 
        {
            cache.emplace(index, data);
            size++;
            return;
        }

        remove_farthest();
        cache.emplace(index, data);
        size++;
    }

    void cache_dump()
    { 
        std::cout << "Capacity: " << capacity << std::endl;
        std::cout << "Size: "     << size     << std::endl;

        for (auto &[key, page] : cache)
            std::cout << "Index: " << key << " Data: " << page << std::endl;
    }
};
