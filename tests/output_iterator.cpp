#include "catch.hpp"
#include "output_filter.hpp"
#include "output_iterator.hpp"
#include "output_transformer.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("output_iterator makes operator++ a no-op and increments in operator=")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 0, 0, 0};

    std::vector<int> results = {0, 0, 0, 0, 0};
    
    auto const isEven = pipes::make_output_filter([](int n) { return n % 2 == 0; });
    std::copy(begin(numbers), end(numbers), isEven(pipes::output(begin(results))));
  
    REQUIRE(results == expected);
}

TEST_CASE("output_iterator is compatible with smart iterators that don't change iteration")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    std::vector<int> results = {0, 0, 0, 0, 0};
    
    auto const times2 = pipes::make_output_transformer([](int n) { return n * 2; });
    std::copy(begin(numbers), end(numbers), times2(pipes::output(begin(results))));
    
    REQUIRE(results == expected);
}

TEST_CASE("output_iterator's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(pipes::output(begin(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}
