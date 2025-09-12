#include <gtest/gtest.h>
#include "LFU_cache.hpp"  

TEST(LFU_tests, miss_before_put) 
{
  LFUCache<int, int> cache(5);
 
  EXPECT_EQ(cache.get(123), nullptr);
}

TEST(LFU_tests, single_put_get) 
{
  LFUCache<int, int> cache(5);

  int element = 1;
  cache.put(element, element);

  
  auto p = cache.get(element);

  ASSERT_NE(p, nullptr);      
  EXPECT_EQ(*p, element);     
}

TEST(LFU_tests, many_put_get) 
{
  LFUCache<int, int> cache(5);

  int element_1 = 1, element_2 = 2;

  cache.put(element_1, element_1);
  cache.put(element_2, element_2);

  auto p1 = cache.get(element_1);

  ASSERT_NE(p1, nullptr);
  EXPECT_EQ(*p1, element_1);



  auto p2 = cache.get(element_2);

  ASSERT_NE(p2, nullptr);
  EXPECT_EQ(*p2, element_2);
}

