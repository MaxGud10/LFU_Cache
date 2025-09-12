#include <gtest/gtest.h>

#include "ideal_cache.hpp"

TEST(ideal_test, single_put_get) 
{
    IdealCache<int, int> cache(5);

    std::vector<int> input_data = {1};

    for (size_t i = 0; i < input_data.size(); i++) 
    {
        std::queue<size_t> position_queue;
        position_queue.push(i);
        position_queue.push(i);

        cache.input_data.emplace(input_data[i], position_queue);
    } 

    for (int& element : input_data) 
    {
        cache.put(element, element);
    }

    EXPECT_EQ(cache.get(input_data[0])->second, input_data[0]);
}

TEST(ideal_test, many_put_get) 
{
    IdealCache<int, int> cache(5);

    std::vector<int> input_data = {1, 2};

    for (size_t i = 0; i < input_data.size(); i++) 
    {
        std::queue<size_t> position_queue;
        position_queue.push(i);
        position_queue.push(i);

        cache.input_data.emplace(input_data[i], position_queue);
    } 

    for (int& element : input_data) 
    {
        cache.put(element, element);
    }

    EXPECT_EQ(cache.get(input_data[0])->second, input_data[0]);
    EXPECT_EQ(cache.get(input_data[1])->second, input_data[1]);
}

TEST(ideal_test, not_found) 
{
    IdealCache<int, int> cache(5);

    int element_1 = 1;

    std::vector<int> input_data = {2};

    for (size_t i = 0; i < input_data.size(); i++) 
    {
        std::queue<size_t> position_queue;
        position_queue.push(i);

        cache.input_data.emplace(input_data[i], position_queue);
    } 

    for (int& element : input_data) 
    {
        cache.put(element, element);
    }

    EXPECT_EQ(cache.get(element_1), cache.end());
}