#include "catch.hpp"
#include "custom_inserter.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("custom_inserter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = input;
    
    std::vector<int> results;
    auto legacyInsertion = [&results](int number) { results.push_back(number); };
    
    std::copy(begin(input), end(input), pipes::custom_inserter(legacyInsertion));
    
    REQUIRE(results == expected);
}

TEST_CASE("custom_inserter's iterator category should be std::output_iterator_tag")
{
    auto inserter = [](){};
    static_assert(std::is_same<decltype(pipes::custom_inserter(inserter))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}
