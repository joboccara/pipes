#include <algorithm>

#include "catch.hpp"
#include "../output/filter.hpp"

TEST_CASE("output::filter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = pipes::output::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    std::vector<int> results;
    std::copy(begin(input), end(input), ifIsEven(std::back_inserter(results)));
    
    REQUIRE(results == expected);
}

TEST_CASE("output::filter can override existing results")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = pipes::output::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10, 0, 0, 0, 0, 0};
    
    std::vector<int> results = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::copy(begin(input), end(input), ifIsEven(begin(results)));
    
    REQUIRE(results == expected);
}

TEST_CASE("output::filter's iterator category should be std::output_iterator_tag")
{
    auto const isEven = pipes::output::filter([](int i) { return i % 2 == 0; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(isEven(std::back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("output::filter cannot override existing contents")
{
    /* This code should not compile as the output_filter_iterator is plugged on a vector::begin
     
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    auto const ifIsEven = pipes::output::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> results = {0, 0, 0, 0, 0};
    std::copy(begin(input), end(input), ifIsEven(begin(results)));
    
    REQUIRE(results == expected);
    
    */ 
}
