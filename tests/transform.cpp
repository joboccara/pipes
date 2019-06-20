#include "catch.hpp"
#include "helpers/FWD.hpp"
#include "transform.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("transform")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    std::vector<int> results;

    SECTION("one transform")
    {
        auto const times2 = pipes::transform([](int i) { return i*2; });
        
        std::copy(begin(input), end(input), times2(std::back_inserter(results)));
        REQUIRE(results == expected);
    }
    
    SECTION("operator>>=")
    {
        std::copy(begin(input), end(input), pipes::transform([](int i) { return i*2; }) >>= std::back_inserter(results));
        REQUIRE(results == expected);
    }
}

TEST_CASE("transform's iterator category should be std::output_iterator_tag")
{
    auto const times2 = pipes::transform([](int i) { return i*2; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(times2(std::back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("transform can override existing contents")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    auto const times2 = pipes::transform([](int i) { return i*2; });
    
    std::vector<int> results = {0, 0, 0, 0, 0};
    std::copy(begin(input), end(input), times2(begin(results)));
    
    REQUIRE(results == expected);
}

TEST_CASE("transform with several outputs")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected1 = {2, 4, 6, 8, 10};
    std::vector<int> expected2 = {3, 6, 9, 12, 15};
    std::vector<int> expected3 = {4, 8, 12, 16, 20};

    auto const multiply = pipes::transform([](int i) { return i*2; },
                                                    [](int i) { return i*3; },
                                                    [](int i) { return i*4; });
    
    std::vector<int> results1;
    std::vector<int> results2;
    std::vector<int> results3;
    
    SECTION("one transform")
    {
        std::copy(begin(input), end(input), multiply(std::back_inserter(results1), std::back_inserter(results2), std::back_inserter(results3)));
        
        REQUIRE(results1 == expected1);
        REQUIRE(results2 == expected2);
        REQUIRE(results3 == expected3);
    }
    
    SECTION("operator >>=")
    {
        auto const identity = pipes::transform([](auto&& value){ return FWD(value); });
        std::copy(begin(input), end(input), identity >>= multiply(std::back_inserter(results1), std::back_inserter(results2), std::back_inserter(results3)));
        
        REQUIRE(results1 == expected1);
        REQUIRE(results2 == expected2);
        REQUIRE(results3 == expected3);
    }
}

TEST_CASE("transform operator=")
{
    std::vector<int> results1, results2;
    auto func = [](int i){ return i * 2; };
    auto pipeline1 = pipes::transform(func) >>= back_inserter(results1);
    auto pipeline2 = pipes::transform(func) >>= back_inserter(results2);
    
    pipeline2 = pipeline1;
    send(pipeline2, 1);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
