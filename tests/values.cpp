#include "pipes/values.hpp"
#include "catch.hpp"
#include "pipes/push_back.hpp"
#include "pipes/tee.hpp"
#include "pipes/transform.hpp"

#include <iostream>
#include <map>
#include <string>

TEST_CASE("values sends the values of a map into the next pipe")
{
    auto const inputs = std::map<int, std::string>{{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}};

    auto const expectedIntermediaryResults = std::vector<std::string>{"1", "2", "3", "4"};
    auto const expectedResults             = std::vector<int>{1, 2, 3, 4};

    auto intermediaryResults = std::vector<std::string>{};
    auto results             = std::vector<int>{};

    inputs >>= pipes::values{} >>= pipes::tee(pipes::push_back(intermediaryResults))
        >>= pipes::transform([](auto&& val) { return std::stoi(val); }) >>= pipes::push_back(results);

    REQUIRE(intermediaryResults == expectedIntermediaryResults);
    REQUIRE(results == expectedResults);
}

TEST_CASE("values sends the second value of a pair into the next pipe")
{
    auto const inputs = std::vector<std::pair<int, std::string>>{{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}};

    auto const expectedIntermediaryResults = std::vector<std::string>{"1", "2", "3", "4"};
    auto const expectedResults             = std::vector<int>{1, 2, 3, 4};

    auto intermediaryResults = std::vector<std::string>{};
    auto results             = std::vector<int>{};

    inputs >>= pipes::values{} >>= pipes::tee(pipes::push_back(intermediaryResults))
        >>= pipes::transform([](auto&& val) { return std::stoi(val); }) >>= pipes::push_back(results);

    REQUIRE(intermediaryResults == expectedIntermediaryResults);
    REQUIRE(results == expectedResults);
}

//Custom Pair class

template<typename U, typename V>
struct myPair
{
    U first;
    V second;
};

TEST_CASE("values sends the second value of a custom pair class into the next pipe")
{
    auto const inputs = std::vector<myPair<int, std::string>>{{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}};

    auto const expectedIntermediaryResults = std::vector<std::string>{"1", "2", "3", "4"};
    auto const expectedResults             = std::vector<int>{1, 2, 3, 4};

    auto intermediaryResults = std::vector<std::string>{};
    auto results             = std::vector<int>{};

    inputs >>= pipes::values{} >>= pipes::tee(pipes::push_back(intermediaryResults))
        >>= pipes::transform([](auto&& val) { return std::stoi(val); }) >>= pipes::push_back(results);

    REQUIRE(intermediaryResults == expectedIntermediaryResults);
    REQUIRE(results == expectedResults);
}
