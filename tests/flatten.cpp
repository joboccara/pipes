#include "catch.hpp"

#include "pipes/pipes.hpp"

#include <vector>

TEST_CASE("flatten pipe")
{
    auto const input = std::vector<std::vector<int>>{ {1, 2}, {3, 4}, {5, 6} };
    auto const expected = std::vector<int>{1, 2, 3, 4, 5, 6};
    
    auto results = std::vector<int>{};
    
    input >>= pipes::flatten
          >>= pipes::push_back(results);
    
    REQUIRE(expected == results);
}
