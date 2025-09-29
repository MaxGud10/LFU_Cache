#include <gtest/gtest.h>
#include <optional>  

#include "LFU_cache.hpp"  

TEST(LFU_tests, miss_before_put) 
{
  LFUCache<int, int> cache(5);
 
  EXPECT_EQ(cache.fetch(123), std::nullopt);
}

TEST(LFU_tests, single_put_get) 
{
  LFUCache<int, int> cache(5);

  int element = 1;
  cache.store(element, element);

  
  auto p = cache.fetch(element);

  ASSERT_TRUE(p.has_value());               
  EXPECT_EQ(*p, element);  
}

TEST(LFU_tests, many_put_get) 
{
  LFUCache<int, int> cache(5);

  int element_1 = 1, element_2 = 2;

  cache.store(element_1, element_1);
  cache.store(element_2, element_2);

  auto p1 = cache.fetch(element_1);

  ASSERT_TRUE(p1.has_value());              
  EXPECT_EQ(*p1, element_1);



  auto p2 = cache.fetch(element_2);

  ASSERT_TRUE(p2.has_value());              
  EXPECT_EQ(*p2, element_2);
}

