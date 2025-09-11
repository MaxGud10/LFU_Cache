#pragma once

#include <iostream>
#include <unordered_map>

template<typename Page_indexT, typename Page_dataT>
class LFUCache 
{
    std::unordered_map<Page_indexT, Page_dataT> valueMap;
    std::unordered_map<Page_indexT, size_t>     freqMap;
    size_t                                      capacity;
    size_t                                      size;

    Page_indexT get_min_freq_index() 
    {
        size_t      min_freq  = __LONG_MAX__;
        Page_indexT ret_index = (freqMap.begin())->first;

        for (auto node = freqMap.begin(); node != freqMap.end(); node++) 
        {
            if (node->second < min_freq) 
            {
                min_freq  = node->second;
                ret_index = node->first;
            }
        }

        return ret_index;
    }

    LFUCache(size_t capacity_) : capacity(capacity_) {size = 0;}

    Page_dataT* get(Page_indexT index) 
    {
        auto found_page = valueMap.find(index);

        if (found_page == valueMap.end())
            return nullptr;
        
        auto page_freq = freqMap.find(index);
        (page_freq->second)++;

        return &(found_page->second);
    }

    void put(Page_indexT& index, Page_dataT& data) 
    {
        auto found_page = valueMap.find(index);

        if (found_page == valueMap.end()) 
        {

            if (size >= capacity) 
            {
                Page_indexT del_index = get_min_freq_index();

                valueMap.erase(del_index);
                freqMap .erase(del_index);

                size--;
            }

            valueMap.insert(std::make_pair(index, data));
            freqMap .insert(std::make_pair(index, 1));
        }
        else 
        {
            auto page_freq = freqMap.find(index);
            (page_freq->second)++;
            return;
        }

        size++;
    }

    void dump() 
    {
        std::cout << "Capacity: " << capacity << std::endl;
        std::cout << "Size: "     << size     << std::endl;

        std::cout << "Dump value map" << std::endl;
        for (auto node = valueMap.begin(); node != valueMap.end(); node++) 
        {
            std::cout << "Index: " << node->first << " Value: " << node->second << std::endl;
        }

        std::cout << "Dump frequency map" << std::endl;
        for (auto node = freqMap.begin(); node != freqMap.end(); node++) 
        {
            std::cout << "Index: " << node->first << " Frequency: " << node->second << std::endl;
        }
    }
};

