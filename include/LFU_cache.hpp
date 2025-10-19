#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <optional>
#include <limits> 
#include <utility>


namespace Caches
{

template<typename Page_indexT, typename Page_dataT>
class LFUCache 
{
public:
    using index_type = Page_indexT;
    using data_type  = Page_dataT;

private:
    using PairT   = std::pair<std::size_t, Page_dataT>;
    using NodeT   = std::pair<Page_indexT, PairT>;
    using ListT   = std::list<NodeT>;
    using ListItT = typename ListT::iterator;
    
public:
    LFUCache(size_t capacity_) : capacity(capacity_) {}

    auto begin()       { return valueMap.begin(); }
    auto end()         { return valueMap.end();   }
    auto begin() const { return valueMap.begin(); }  
    auto end()   const { return valueMap.end();   }


    // ищем элемент по ключу
    std::optional<Page_dataT> fetch(const Page_indexT& index)
    {
        auto it = valueMap.find(index);
        if (it == valueMap.end())
            return std::nullopt;

        touch(index);                    // нашли -> увеличиваем частоту   
        auto newIt = valueMap.at(index); 

        return newIt->second.second;
    }


    // записываем новыый элемент или обновляем уже существующий
    void store(const Page_indexT& index, const Page_dataT& data)
    {
        if (capacity == 0) 
            return;

        auto it = valueMap.find(index);
        if (it != valueMap.end()) 
        {
            it->second->second.second = data; // обновили value
            touch(index);

            return;
        }

        if (is_full())
            evict_one();

        insert_new(index, data);
    }

void dump() const
{
    std::cout << "=========================\\n";
    std::cout << "Capacity: "      << capacity << "\n";
    std::cout << "Current size: "  << valueMap.size() << "\n";
    std::cout << "Min frequency: " << minFreq  << "\n\n";

    std::cout << "Buckets (freq → [index:value]):\n";
    for (const auto& [freq, lst] : buckets)
    {
        std::cout << "  [" << freq << "] -> ";
        bool first = true;
        for (const auto& [index, pairData] : lst)
        {
            if (!first) std::cout << ", ";
            std::cout << index << ":" << pairData.second;
            first = false;
        }
        std::cout << "\n";
    }

    std::cout << "\nValueMap (index → (freq,value)):\n";
    for (const auto& [index, it] : valueMap)
    {
        const auto& [key, pairData] = *it;
        std::cout << "  " << index
                  << " → (freq=" << pairData.first
                  << ", value="  << pairData.second << ")\n";
    }

    std::cout << "=========================\n" << std::endl;
}


private:
    bool is_full() const 
    {
        return valueMap.size() >= capacity;
    }

    // вытесение одного элемента по minFreq
    void evict_one()
    {
              auto&       lst        = buckets[minFreq];
        const auto&       victimNode = lst.back();      // (key, (freq, value))
        const Page_indexT& victimKey = victimNode.first;

        // убраем из списка и из мапы
        lst.pop_back();
        valueMap.erase(victimKey);

        if (lst.empty())
            buckets.erase(minFreq);
    }

    // вставляем новый элемент с частотой 1
    void insert_new(const Page_indexT& index, const Page_dataT& data)
    {
        auto& lst = buckets[1];
        lst.emplace_front(index, PairT{1, data}); 
        valueMap[index] = lst.begin();
        minFreq = 1;
    }

    // увеличиваем частоту использования ключа
    void touch(const Page_indexT& index)
    {
        // берем текущий итератор и распаковывваем его 
        auto        itCur    = valueMap.at(index);        // -> NodeT
        std::size_t curFreq = itCur->second.first;       
        Page_dataT  curVal  = std::move(itCur->second.second); 

        // удаляем из текущего списка
        auto& curList = buckets[curFreq];
              curList.erase(itCur);
        if (curList.empty()) 
        {
            buckets.erase(curFreq);
            if (minFreq == curFreq) 
                ++minFreq;
        }

        const std::size_t newFreq  = curFreq + 1;
        auto&             nextList = buckets[newFreq];
        nextList.emplace_front(index, PairT{newFreq, std::move(curVal)});
        valueMap[index]            = nextList.begin();
    }

private:
    std::unordered_map<Page_indexT, ListItT> valueMap;
    std::unordered_map<std::size_t, ListT  > buckets;
    size_t                                   capacity = 0;
    size_t                                   minFreq  = 0;
};

} // namespace Caches
