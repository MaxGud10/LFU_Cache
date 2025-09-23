#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

template<typename Page_indexT, typename Page_dataT>
class LFUCache 
{
    std::unordered_map<Page_indexT, Page_dataT> valueMap;
    std::unordered_map<Page_indexT, size_t>     freqMap;
    size_t                                      capacity;
    size_t                                      size;


private:
    std::unordered_map<Page_indexT, typename std::list<Page_indexT>::iterator> iterMap;
    std::unordered_map<size_t, std::list<Page_indexT>> buckets;
    size_t minFreq = 0;

    Page_indexT get_min_freq_index() const
    {
        size_t      min_freq  = __LONG_MAX__;
        Page_indexT ret_index = (freqMap.begin())->first;

        for (auto& node : freqMap) 
        {
            if (node.second < min_freq) 
            {
                min_freq  = node.second;
                ret_index = node.first;
            }
        }

        return ret_index;
    }


public:
    LFUCache(size_t capacity_) : capacity(capacity_), size(0) {}

    auto begin() { return valueMap.begin(); }
    auto end()   { return valueMap.end();   }

    Page_dataT* get(Page_indexT index) 
    {
        auto it_val = valueMap.find(index);
        if (it_val == valueMap.end())
            return nullptr;

        size_t f = freqMap[index];


        auto it_it = iterMap.find(index);
        if (it_it != iterMap.end())
        {
            auto &lst = buckets[f];
            lst.erase(it_it->second);
        }

        if (f == minFreq && buckets[f].empty())
        {
            buckets.erase(f);
            ++minFreq;
        }

        size_t nf = f + 1;

        freqMap[index] = nf;
        buckets[nf   ].push_front(index);
        iterMap[index] = buckets[nf].begin();

        return &(it_val->second);
    }

    void put(Page_indexT& index, Page_dataT& data) 
    {
        if (capacity == 0) return;

        auto itVal = valueMap.find(index);

        if (itVal == valueMap.end()) 
        {
            if (size >= capacity) 
            {
                auto &lst = buckets[minFreq];
                Page_indexT victim = lst.back();
                lst.pop_back();

                if (lst.empty()) buckets.erase(minFreq);

                valueMap.erase(victim);
                freqMap .erase(victim);
                iterMap .erase(victim);
                --size;
            }

            valueMap[index] = data;
            freqMap [index] = 1;

            buckets [1].push_front(index);

            iterMap [index] = buckets[1].begin();
            minFreq = 1;
            ++size;
        }
        else 
        {
            itVal->second = data;
            (void)get(index);  
            return;
        }
    }

    void dump() const
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

