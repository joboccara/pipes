#include "catch.hpp"
#include "pipes/pipes.hpp"

TEST_CASE("step passes every N th element starting from 0")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 1, 4, 7, 10 };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::step(3)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("step passes all elements if N is 1")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 2, 4, 6, 8, 10 };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0; })
          >>= pipes::step(1)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("step passes only first element if N is greater than or equal to size of incoming input")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 2 };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0; })
          >>= pipes::step(10)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("step passes only first element if N is zero")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 1 };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::step(0)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("step steps over elements coming from several collections")
{
    auto const input1 = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{ 10, 20, 30, 40, 50};

    auto const expected = std::vector<int>{ 11, 33, 55 };
    
    auto result = std::vector<int>{};
    
    pipes::mux(input1, input2)
        >>= pipes::step(2)
        >>= pipes::transform([](int a, int b){ return a + b; })
        >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("step doesn't send anything on if the input is empty")
{
    auto const input = std::vector<int>{ };
    auto const expected = std::vector<int>{ };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::step(10)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}
