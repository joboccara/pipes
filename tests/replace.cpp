#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <map>
#include <vector>

TEST_CASE("Replace existing contents on STL containers")
{
    auto input                = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int> results = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    input >>= pipes::replace(results);

    REQUIRE(results == expected);
}
