#include "catch.hpp"
#include "pipes/override.hpp"
#include "pipes/transform.hpp"
#include "pipes/push_back.hpp"
#include "pipes/unzip.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <utility>
#include <vector>

TEST_CASE("unzip breaks tuples down to containers")
{
    std::vector<std::tuple<int, int, int>> lines = { std::make_tuple(1, 2, 3), std::make_tuple(4, 5, 6), std::make_tuple(7, 8, 9), std::make_tuple(10, 11, 12) };
    std::vector<int> column1, column2, column3;

    std::copy(begin(lines), end(lines), pipes::unzip(pipes::push_back(column1), pipes::push_back(column2), pipes::push_back(column3)));

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

    std::copy(begin(entries), end(entries), pipes::unzip(pipes::push_back(keys), pipes::push_back(values)));

    REQUIRE(keys == expectedKeys);
    REQUIRE(values == expectedValues);
}

TEST_CASE("unzip's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(pipes::unzip(pipes::push_back(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("unzip can override existing contents")
{
    std::vector<std::tuple<int, int, int>> lines = { std::make_tuple(1, 2, 3), std::make_tuple(4, 5, 6), std::make_tuple(7, 8, 9), std::make_tuple(10, 11, 12) };
    std::vector<int> column1 = {0, 0, 0, 0, 0};
    std::vector<int> column2 = {0, 0, 0, 0, 0};
    std::vector<int> column3 = {0, 0, 0, 0, 0};

    std::copy(begin(lines), end(lines), pipes::unzip(pipes::override(column1), pipes::override(column2), pipes::override(column3)));

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
    std::transform(begin(s), end(s), pipes::push_back(upperString), [](char c){ return static_cast<char>(std::toupper(c)); });
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
              pipes::unzip(pipes::push_back(keys),
                           toUpper >>= pipes::push_back(values)));

    REQUIRE(keys == expectedKeys);
    REQUIRE(values == expectedValues);
}

TEST_CASE("unzip operator=")
{
    auto results1 = std::vector<int>{};
    auto results2 = std::vector<std::string>{};
    auto results3 = std::vector<int>{};
    auto results4 = std::vector<std::string>{};
    auto pipeline1 = pipes::unzip(pipes::push_back(results1), pipes::push_back(results2));
    auto pipeline2 = pipes::unzip(pipes::push_back(results3), pipes::push_back(results4));

    pipeline2 = pipeline1;
    send(std::make_pair(0, "zero"), pipeline2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
