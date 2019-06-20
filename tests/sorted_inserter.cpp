#include <algorithm>
#include <set>
#include <vector>
#include "catch.hpp"

#include "sorted_inserter.hpp"

TEST_CASE("sorted_inserter")
{
    std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
    std::set<int> expected = { -4, 1, 2, 3, 7, 8, 10 };
    std::set<int> results;
    std::copy(begin(v), end(v), pipes::sorted_inserter(results));
    
    REQUIRE((results == expected));
}

TEST_CASE("sorted_inserter should not insert existing values")
{
    std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
    std::set<int> expected = { -4, 1, 2, 3, 7, 8, 10 };
    std::set<int> results = { 3 };
    std::copy(begin(v), end(v), pipes::sorted_inserter(results));
    
    REQUIRE((results == expected));
}

TEST_CASE("sorted_inserter's iterator category should be std::output_iterator_tag")
{
    std::set<int> results;
    static_assert(std::is_same<decltype(pipes::sorted_inserter(results))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("sorted_inserter::operator=")
{
    auto results1 = std::set<int>{};
    auto sorted_inserter1 = pipes::sorted_inserter(results1);
    auto results2 = std::set<int>{};
    auto sorted_inserter2 = pipes::sorted_inserter(results2);

    sorted_inserter2 = sorted_inserter1;
    pipes::send(sorted_inserter2, 0);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
