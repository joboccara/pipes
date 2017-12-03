#include <algorithm>

#include "catch.hpp"
#include "output_filter.hpp"

TEST_CASE("output_filter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = fluent::make_output_filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    std::vector<int> results;
    std::copy(begin(input), end(input), ifIsEven(std::back_inserter(results)));
    
    REQUIRE(results == expected);
}

TEST_CASE("output_filter's iterator category should be std::output_iterator_tag")
{
    auto const isEven = fluent::make_output_filter([](int i) { return i % 2 == 0; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(isEven(std::back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}
