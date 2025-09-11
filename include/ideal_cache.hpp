#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>



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
        Page_indexT remove_index;
        size_t      max_distance = 0;

        for (auto& elem : cache) 
        {
            auto elem_info = input_data.find(elem.first);
            size_t elem_distance = (elem_info->second).front();

            if (max_distance < elem_distance) 
            {
                max_distance = elem_distance;
                remove_index = elem_info->first;
            }
        }

        cache.erase(remove_index);
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
        (elem_info->second).pop();

        return found_page;
    }

    void put(Page_indexT& index, Page_dataT& data) 
    {
        auto found_page = cache.find(index);
        auto elem_info  = input_data.find(index);
        (elem_info->second).pop();

        if (found_page != cache.end())   return; 

        if ((elem_info->second).empty()) return;    

        if (size < capacity) 
        {
            cache.emplace(index, data);
            size++;

            return;
        }

        for (auto& cache_elem : cache) 
        {
            if (((input_data.find(cache_elem.first))->second).front() < (elem_info->second).front()) 
            {
                remove_farthest();
                cache.emplace(index, data);
                size++;

                return;
            }
        }
    }

    void cache_dump()
    { 
        std::cout << "Capacity: " << capacity << std::endl;
        std::cout << "Size: "     << size     << std::endl;

        for (auto &[key, page] : cache)
            std::cout << "Index: " << key << " Data: " << page << std::endl;
    }
};

