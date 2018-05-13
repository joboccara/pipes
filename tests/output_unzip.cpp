#include "catch.hpp"
#include "../output/unzip.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

TEST_CASE("output::unzip breaks tuples down to containers")
{
    std::vector<std::tuple<int, int, int>> lines = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12} };
    std::vector<int> column1, column2, column3;
    
    std::copy(begin(lines), end(lines), fluent::output::unzip(back_inserter(column1), back_inserter(column2), back_inserter(column3)));
    
    std::vector<int> expectedColumn1 = {1, 4, 7, 10};
    std::vector<int> expectedColumn2 = {2, 5, 8, 11};
    std::vector<int> expectedColumn3 = {3, 6, 9, 12};
    
    REQUIRE(column1 == expectedColumn1);
    REQUIRE(column2 == expectedColumn2);
    REQUIRE(column3 == expectedColumn3);
}

TEST_CASE("output::unzip breaks pairs down to two containers")
{
    std::map<int, std::string> entries = { {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"} };
    std::vector<int> expectedKeys = {1, 2, 3, 4, 5};
    std::vector<std::string> expectedValues = {"one", "two", "three", "four", "five"};

    std::vector<int> keys;
    std::vector<std::string> values;
    
    std::copy(begin(entries), end(entries), fluent::output::unzip(back_inserter(keys), back_inserter(values)));
    
    REQUIRE(keys == expectedKeys);
    REQUIRE(values == expectedValues);
}


TEST_CASE("output::unzip's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(fluent::output::unzip(back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("output::unzip can override existing contents")
{
    std::vector<std::tuple<int, int, int>> lines = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12} };
    std::vector<int> column1 = {0, 0, 0, 0, 0};
    std::vector<int> column2 = {0, 0, 0, 0, 0};
    std::vector<int> column3 = {0, 0, 0, 0, 0};
    
    std::copy(begin(lines), end(lines), fluent::output::unzip(begin(column1), begin(column2), begin(column3)));
    
    std::vector<int> expectedColumn1 = {1, 4, 7, 10, 0};
    std::vector<int> expectedColumn2 = {2, 5, 8, 11, 0};
    std::vector<int> expectedColumn3 = {3, 6, 9, 12, 0};
    
    REQUIRE(column1 == expectedColumn1);
    REQUIRE(column2 == expectedColumn2);
    REQUIRE(column3 == expectedColumn3);
}
