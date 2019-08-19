#include "catch.hpp"
#include "pipes/dev_null.hpp"

#include <vector>

TEST_CASE("dead_end_iterator")
{
    const std::vector<int> input = {1, 2, 3, 4, 5};
    
    std::copy(begin(input), end(input), pipes::dev_null());
}

TEST_CASE("dead_end_iterator::operator=")
{
    pipes::dev_null() = pipes::dev_null(); // should compile
}
