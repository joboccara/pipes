#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <set>
#include <string>
#include <vector>

TEST_CASE("cartesian_product transform")
{
    auto const inputs1 = std::vector<int>{1, 2, 3};
    auto const inputs2 = std::vector<std::string>{"up", "down"};
    
    auto const expected = std::vector<std::string>{"1-up", "1-down", "2-up", "2-down", "3-up", "3-down"};
    
    auto results = std::vector<std::string>{};
    
    pipes::cartesian_product(inputs1, inputs2)
        >>= pipes::transform([](int i, std::string const&s){ return std::to_string(i) + '-' + s; })
        >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("cartesian_product with std::set")
{
    auto const inputs1 = std::set<int>{1, 2, 3};
    auto const inputs2 = std::set<std::string>{"down", "up"};
    
    auto const expected = std::vector<std::string>{"1-down", "1-up", "2-down", "2-up", "3-down", "3-up"};
    
    auto results = std::vector<std::string>{};
    
    pipes::cartesian_product(inputs1, inputs2)
        >>= pipes::transform([](int i, std::string const&s){ return std::to_string(i) + '-' + s; })
        >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("cartesian_product with empty collections")
{
    SECTION("first collection empty")
    {
        auto const inputs1 = std::vector<int>{};
        auto const inputs2 = std::vector<std::string>{"up", "down"};
        
        auto const expected = std::vector<std::string>{};
        
        auto results = std::vector<std::string>{};
        
        pipes::cartesian_product(inputs1, inputs2)
            >>= pipes::transform([](int i, std::string const&s){ return std::to_string(i) + '-' + s; })
            >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
    
    SECTION("another collection empty")
    {
        auto const inputs1 = std::vector<int>{1, 2, 3};
        auto const inputs2 = std::vector<std::string>{};
        
        auto const expected = std::vector<std::string>{};
        
        auto results = std::vector<std::string>{};
        
        pipes::cartesian_product(inputs1, inputs2)
            >>= pipes::transform([](int i, std::string const&s){ return std::to_string(i) + '-' + s; })
            >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
    
    SECTION("only empty collections")
    {
        auto const inputs1 = std::vector<int>{};
        auto const inputs2 = std::vector<std::string>{};
        
        auto const expected = std::vector<std::string>{};
        
        auto results = std::vector<std::string>{};
        
        pipes::cartesian_product(inputs1, inputs2)
            >>= pipes::transform([](int i, std::string const&s){ return std::to_string(i) + '-' + s; })
            >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
}
