#include "catch.hpp"

#include "pipes/pipes.hpp"

#include <vector>

TEST_CASE("join pipe")
{
    auto const input = std::vector<std::vector<int>>{ {1, 2}, {3, 4}, {5, 6} };
    auto const expected = std::vector<int>{1, 2, 3, 4, 5, 6};
    
    auto results = std::vector<int>{};
    
    input >>= pipes::join
          >>= pipes::push_back(results);
    
    REQUIRE(expected == results);
}

TEST_CASE("join pipe operator=")
{
    std::vector<int> results1, results2;
    auto pipeline1 = pipes::join >>= pipes::push_back(results1);
    auto pipeline2 = pipes::join >>= pipes::push_back(results2);
    
    pipeline2 = pipeline1;
    send(std::vector<int>{1, 2}, pipeline2);
    REQUIRE(results1.size() == 2);
    REQUIRE(results2.size() == 0);
}
