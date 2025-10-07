#pragma once
#include <utility>

namespace Caches 
{

template <class CacheT>
class CacheDriver 
{
public:
    using index_type = typename CacheT::index_type;
    using data_type  = typename CacheT::data_type;

    CacheDriver(CacheT& cache) : cache_(&cache) {}

    // Вариант "как в примере": возвращаем hit/miss,
    // а данные при промахе добываем через slow_get и кладем в кэш.
    template <class SlowGetter>
    bool lookup_update(const index_type& id, SlowGetter&& slow_get) 
    {
        if (auto v = cache_->fetch(id)) 
        {
            return true;                    // hit
        }
        
        auto data = slow_get(id);           // miss -> медленно забираем
        cache_->store(id, data);            // и кладём в кэш

        return false;                       // miss
    }

    // Вариант "вернуть данные" (часто удобно)
    template <class SlowGetter>
    data_type get_page(const index_type& id, SlowGetter&& slow_get) 
    {
        if (auto v = cache_->fetch(id))
        {
            return *v;                      // hit
        }

        auto data = slow_get(id);           // miss
        cache_->store(id, data);

        return data;
    }

          CacheT& cache()       { return *cache_; }
    const CacheT& cache() const { return *cache_; }

private:
    CacheT* cache_; 
};

} // namespace Caches
