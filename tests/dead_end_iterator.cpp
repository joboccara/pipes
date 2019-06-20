#include "catch.hpp"
#include "dead_end_iterator.hpp"

#include <vector>

TEST_CASE("dead_end_iterator")
{
    const std::vector<int> input = {1, 2, 3, 4, 5};
    
    std::copy(begin(input), end(input), pipes::dead_end_iterator());
}

TEST_CASE("dead_end_iterator::operator=")
{
    pipes::dead_end_iterator{} = pipes::dead_end_iterator{}; // should copmile
}
