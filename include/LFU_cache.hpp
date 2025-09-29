#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <optional>
#include <limits> 



template<typename Page_indexT, typename Page_dataT>
class LFUCache 
{
public:
    LFUCache(size_t capacity_) : capacity(capacity_) {}

    auto begin() { return valueMap.begin(); }
    auto end()   { return valueMap.end();   }


    // ищем элемент по ключу
    std::optional<Page_dataT> fetch(const Page_indexT& index)
    {
        auto it = valueMap.find(index);
        if (it == valueMap.end())
            return std::nullopt;

        touch(index);     // нашли -> увеличиваем частоту   
        return it->second;
    }


    // записываем новыый элемент или обновляем уже существующий
    void store(const Page_indexT& index, const Page_dataT& data)
    {
        if (capacity == 0) return;

        auto itVal = valueMap.find(index);

        if (itVal != valueMap.end())
        {
            // ключ уже существует -> обновляем значение и частоту
            itVal->second = data;
            touch(index); 

            return;
        }

        if (valueMap.size() >= capacity)
        {
            auto& lst = buckets[minFreq];
            const Page_indexT victim = lst.back();
            
            lst.pop_back();
            if (lst.empty())
                buckets.erase(minFreq);

            valueMap.erase(victim);
            meta    .erase(victim);
        }

        // новый элемент всегда создаётся с частотой 1
        valueMap[index] = data;
        buckets [1    ].push_front(index);
        meta    [index] = EntryInfo{1, buckets[1].begin()};
        minFreq = 1;
    }

    void dump() const
    {
        std::cout << "Capacity: " << capacity << "\n";
        std::cout << "Size: "     << valueMap.size() << "\n\n";

        std::cout << "Dump value map\n";
        for (const auto& kv : valueMap)
            std::cout << "Index: " << kv.first << " Value: " << kv.second << "\n";

        std::cout << "\nDump meta (frequencies)\n";
        for (const auto& kv : meta)
            std::cout << "Index: " << kv.first << " Frequency: " << kv.second.freq << "\n";
        std::cout << std::endl;
    }

private:ё
    struct EntryInfo 
    {
        size_t freq = 0;                              // частота использования
        typename std::list<Page_indexT>::iterator it; // итератор на позицию в списке buckets[freq]
    };

    // увеличиваем частоту использования ключа
    void touch(const Page_indexT& index)
    {
        auto& info = meta.at(index);
        size_t f = info.freq;

        auto& lst = buckets[f];
        lst.erase(info.it);
        if (lst.empty())
        {
            buckets.erase(f);
            if (minFreq == f) 
                ++minFreq;
        }


        size_t nf = f + 1;
        auto& nlist = buckets[nf];
        nlist.push_front(index);
        info.freq = nf;
        info.it = nlist.begin();
    }

private:
    std::unordered_map<Page_indexT, Page_dataT>        valueMap; 
    std::unordered_map<size_t, std::list<Page_indexT>> buckets; 
    std::unordered_map<Page_indexT, EntryInfo>         meta; 
    size_t                                             capacity = 0;
    size_t                                             minFreq = 0;
};

