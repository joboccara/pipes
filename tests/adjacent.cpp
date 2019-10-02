#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <set>
#include <vector>

TEST_CASE("adjacent sends pairs of consecutive elements to the next pipe")
{
    auto const input = std::vector<int>{1, 2, 4, 7, 11, 16};
    auto const expected = std::vector<int>{1, 2, 3, 4, 5};
    
    auto results = std::vector<int>{};
    
    pipes::adjacent(input) >>= pipes::transform([](int a, int b){ return b - a; })
                           >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("adjacent operates on std::set")
{
    auto const input = std::set<int>{1, 2, 4, 7, 11, 16};
    auto const expected = std::vector<int>{1, 2, 3, 4, 5};
    
    auto results = std::vector<int>{};
    
    pipes::adjacent(input) >>= pipes::transform([](int a, int b){ return b - a; })
                           >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("adjacent of an empty collection doesn't send anything")
{
    auto const expected = std::vector<int>{};
    
    auto results = std::vector<int>{};

    pipes::adjacent(std::vector<int>{}) >>= pipes::transform([](int a, int b){ return a + b; })
                                        >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("adjacent of a collection with one value doesn't send anything")
{
    auto const expected = std::vector<int>{};
    
    auto results = std::vector<int>{};
    
    pipes::adjacent(std::vector<int>{1}) >>= pipes::transform([](int a, int b){ return a + b; })
                                         >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("adjacent of a collection of two elements sends the two elements")
{
    auto const expected = std::vector<int>{11};
    
    auto results = std::vector<int>{};
    
    pipes::adjacent(std::vector<int>{1, 10}) >>= pipes::transform([](int a, int b){ return a + b; })
                                             >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}
