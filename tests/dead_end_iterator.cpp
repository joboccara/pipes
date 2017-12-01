#include "catch.hpp"
#include "dead_end_iterator.hpp"

#include <vector>

TEST_CASE("dead_end_iterator")
{
    const std::vector<int> input = {1, 2, 3, 4, 5};
    
    std::copy(begin(input), end(input), dead_end_iterator());
}
