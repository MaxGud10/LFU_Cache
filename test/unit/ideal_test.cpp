#include <gtest/gtest.h>
#include <queue>
#include <vector>
#include "ideal_cache.hpp"


TEST(ideal_test, single_store_fetch) 
{
    Caches::IdealCache<int, int> cache(5);

    std::vector<int> req = {1};

    for (size_t i = 0; i < req.size(); ++i) 
    {
        std::queue<size_t> q;
        q.push(i); 
        q.push(i); 
        cache.input_data.emplace(req[i], std::move(q));
    }


    for (int page_id : req) 
    {
        cache.store(page_id, page_id);
    }

    auto v = cache.fetch(req[0]);
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, req[0]);
}

TEST(ideal_test, many_store_fetch) 
{
    Caches::IdealCache<int, int> cache(5);

    std::vector<int> req = {1, 2};

    for (size_t i = 0; i < req.size(); ++i) 
    {
        std::queue<size_t> q;
        q.push(i); 
        q.push(i); 
        cache.input_data.emplace(req[i], std::move(q));
    }

    for (int page_id : req)
    {
        cache.store(page_id, page_id);
    }

    auto v0 = cache.fetch(req[0]);
    ASSERT_TRUE(v0.has_value());
    EXPECT_EQ(*v0, req[0]);

    auto v1 = cache.fetch(req[1]);
    ASSERT_TRUE(v1.has_value());
    EXPECT_EQ(*v1, req[1]);
}


TEST(ideal_test, not_found)
{
    Caches::IdealCache<int, int> cache(5);

    const int missing = 1;
    std::vector<int> present = {2};

    for (size_t i = 0; i < present.size(); ++i)
    {
        std::queue<size_t> q;
        q.push(i); 
        q.push(i); 
        cache.input_data.emplace(present[i], std::move(q));
    }


    for (int page_id : present) 
    {
        cache.store(page_id, page_id);
    }

    auto v = cache.fetch(missing);
    EXPECT_FALSE(v.has_value());
}
