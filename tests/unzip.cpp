#include "catch.hpp"
#include "unzip.hpp"
#include "transform.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

TEST_CASE("unzip breaks tuples down to containers")
{
    std::vector<std::tuple<int, int, int>> lines = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12} };
    std::vector<int> column1, column2, column3;
    
    std::copy(begin(lines), end(lines), pipes::unzip(back_inserter(column1), back_inserter(column2), back_inserter(column3)));
    
    std::vector<int> expectedColumn1 = {1, 4, 7, 10};
    std::vector<int> expectedColumn2 = {2, 5, 8, 11};
    std::vector<int> expectedColumn3 = {3, 6, 9, 12};
    
    REQUIRE(column1 == expectedColumn1);
    REQUIRE(column2 == expectedColumn2);
    REQUIRE(column3 == expectedColumn3);
}

TEST_CASE("unzip breaks pairs down to two containers")
{
    std::map<int, std::string> entries = { {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"} };
    std::vector<int> expectedKeys = {1, 2, 3, 4, 5};
    std::vector<std::string> expectedValues = {"one", "two", "three", "four", "five"};

    std::vector<int> keys;
    std::vector<std::string> values;
    
    std::copy(begin(entries), end(entries), pipes::unzip(back_inserter(keys), back_inserter(values)));
    
    REQUIRE(keys == expectedKeys);
    REQUIRE(values == expectedValues);
}


TEST_CASE("unzip's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(pipes::unzip(back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("unzip can override existing contents")
{
    std::vector<std::tuple<int, int, int>> lines = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12} };
    std::vector<int> column1 = {0, 0, 0, 0, 0};
    std::vector<int> column2 = {0, 0, 0, 0, 0};
    std::vector<int> column3 = {0, 0, 0, 0, 0};
    
    std::copy(begin(lines), end(lines), pipes::unzip(begin(column1), begin(column2), begin(column3)));
    
    std::vector<int> expectedColumn1 = {1, 4, 7, 10, 0};
    std::vector<int> expectedColumn2 = {2, 5, 8, 11, 0};
    std::vector<int> expectedColumn3 = {3, 6, 9, 12, 0};
    
    REQUIRE(column1 == expectedColumn1);
    REQUIRE(column2 == expectedColumn2);
    REQUIRE(column3 == expectedColumn3);
}

std::string toUpperString(std::string const& s)
{
    std::string upperString;
    std::transform(begin(s), end(s), std::back_inserter(upperString), [](char c){ return std::toupper(c); });
    return upperString;
}

TEST_CASE("unzip + transform")
{
    std::map<int, std::string> entries = { {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"} };
    std::vector<int> expectedKeys = {1, 2, 3, 4, 5};
    std::vector<std::string> expectedValues = {"ONE", "TWO", "THREE", "FOUR", "FIVE"};
    
    std::vector<int> keys;
    std::vector<std::string> values;
    
    auto const toUpper = pipes::transform(toUpperString);
    
    std::copy(begin(entries), end(entries),
              pipes::unzip(back_inserter(keys),
                                    toUpper(back_inserter(values))));
    
    REQUIRE(keys == expectedKeys);
    REQUIRE(values == expectedValues);
}

TEST_CASE("unzip operator=")
{
    auto results1 = std::vector<int>{};
    auto results2 = std::vector<std::string>{};
    auto results3 = std::vector<int>{};
    auto results4 = std::vector<std::string>{};
    auto pipeline1 = pipes::unzip(back_inserter(results1), back_inserter(results2));
    auto pipeline2 = pipes::unzip(back_inserter(results3), back_inserter(results4));
    
    pipeline2 = pipeline1;
    send(pipeline2, std::make_pair(0, "zero"));
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
