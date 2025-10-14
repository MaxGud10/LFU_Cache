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

    CacheDriver(size_t capacity) : cache_(capacity) {}

    template <class SlowGetter>
    bool lookup_update(const index_type& id, SlowGetter&& slow_get) 
    {
        if (cache_.fetch(id)) 
            return true;                    // hit
        
        auto data = slow_get(id);           // miss -> медленно забираем
        cache_.store(id, std::move(data));  // и кладём в кэш

        return false;                       // miss
    }

          CacheT& cache()       { return cache_; }
    const CacheT& cache() const { return cache_; }

private:
    CacheT cache_; 
};

} // namespace Caches
