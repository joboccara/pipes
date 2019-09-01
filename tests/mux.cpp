#include "catch.hpp"
#include "pipes/pipes.hpp"

TEST_CASE("mux can send info from several ranges to the next pipe")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const expected = std::vector<int>{11, 22, 33, 44, 55};
    
    auto results = std::vector<int>{};
    
    pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("mux stops when the shortest collection is empty")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const expected = std::vector<int>{11, 22, 33, 44};
    
    auto results = std::vector<int>{};
    
    pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("mux doesn't do anything with empty ranges")
{
    SECTION("empty-full")
    {
        auto const input1 = std::vector<int>{};
        auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
        auto const expected = std::vector<int>{};
        
        auto results = std::vector<int>{};
        
        pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
    
    SECTION("full-empty")
    {
        auto const input1 = std::vector<int>{1, 2, 3, 4};
        auto const input2 = std::vector<int>{};
        auto const expected = std::vector<int>{};
        
        auto results = std::vector<int>{};
        
        pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
    
    SECTION("empty-empty")
    {
        auto const input1 = std::vector<int>{};
        auto const input2 = std::vector<int>{};
        auto const expected = std::vector<int>{};
        
        auto results = std::vector<int>{};
        
        pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
}
