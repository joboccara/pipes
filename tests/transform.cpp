#include "catch.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/transform.hpp"
#include "pipes/push_back.hpp"
#include "pipes/override.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("transform")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    std::vector<int> results;
    
    SECTION("input from range")
    {
        input >>= pipes::transform([](int i) { return i*2; }) >>= pipes::push_back(results);
        REQUIRE(results == expected);
    }

    SECTION("input from STL algorithm")
    {
        std::copy(begin(input), end(input), pipes::transform([](int i) { return i*2; }) >>= pipes::push_back(results));
        REQUIRE(results == expected);
    }
}

TEST_CASE("transform can use pointer to member functions")
{
    struct S
    {
        int get_42() const { return 42; }
    };
    
    auto const input = std::vector<S>(10);
    auto const expected = std::vector<int>(10, 42);
    std::vector<int> results;
    
    input >>= pipes::transform(&S::get_42)
          >>= pipes::push_back(results);
    
    REQUIRE(expected == results);
}

TEST_CASE("transform's iterator category should be std::output_iterator_tag")
{
    auto const times2 = pipes::transform([](int i) { return i*2; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(times2 >>= pipes::push_back(output))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("transform can override existing contents")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    auto const times2 = pipes::transform([](int i) { return i*2; });
    
    std::vector<int> results = {0, 0, 0, 0, 0};
    std::copy(begin(input), end(input), times2 >>= pipes::override(results));
    
    REQUIRE(results == expected);
}

TEST_CASE("transform operator=")
{
    std::vector<int> results1, results2;
    auto func = [](int i){ return i * 2; };
    auto pipeline1 = pipes::transform(func) >>= pipes::push_back(results1);
    auto pipeline2 = pipes::transform(func) >>= pipes::push_back(results2);
    
    pipeline2 = pipeline1;
    send(1, pipeline2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
