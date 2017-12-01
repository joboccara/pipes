#include "catch.hpp"
#include "output_transformer.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("output_transformer")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    auto const times2 = make_output_transformer([](int i) { return i*2; });
    
    std::vector<int> results;
    std::copy(begin(input), end(input), times2(std::back_inserter(results)));
    
    REQUIRE((results == expected));
}

TEST_CASE("output_transformer's iterator category should be std::output_iterator_tag")
{
    auto const times2 = make_output_transformer([](int i) { return i*2; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(times2(std::back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}
