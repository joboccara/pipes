#include "catch.hpp"
#include "transform.hpp"
#include "filter.hpp"
#include "partition.hpp"
#include "unzip.hpp"
#include "switch.hpp"
#include "funnel.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("Mix of various output iterators")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedOutput1 = {6, 12, 18};
    std::vector<int> expectedOutput2 = {2, 4};
    std::vector<int> expectedOutput3 = {2, 10};
    std::vector<int> expectedOutput4 = {1, 5, 7};
    std::vector<char> expectedOutput5 = {'A', 'A', 'A'};
    
    std::vector<int> output1;
    std::vector<int> output2;
    std::vector<int> output3;
    std::vector<int> output4;
    std::vector<char> output5;

    auto const times2 = pipes::transform([](int i) { return i*2; });
    auto const divideBy2 = pipes::transform([](int i) { return i/2; });
    auto const pairUpWithA = pipes::transform([](int i) { return std::make_pair(i, 'A'); });


    std::copy(begin(numbers), end(numbers), pipes::switch_(pipes::case_([](int n){ return n % 3 == 0; }) >>= times2(back_inserter(output1)),
                                                           pipes::case_([](int n){ return n % 2 == 0; }) >>= divideBy2(
                                                                                                                       pipes::partition([](int n){ return n % 2 == 0; },
                                                                                                                                         back_inserter(output2),
                                                                                                                                         times2(back_inserter(output3))
                                                                                                                                      )
                                                                                                                      ),
                                                           pipes::case_( [](int n){ return n % 1 == 0; } ) >>= pairUpWithA(
                                                                                                                           pipes::unzip(
                                                                                                                                        back_inserter(output4),
                                                                                                                                        back_inserter(output5)
                                                                                                                                       )
                                                                                                                          )
                                                                                                                     ));
    
    REQUIRE(output1 == expectedOutput1);
    REQUIRE(output2 == expectedOutput2);
    REQUIRE(output3 == expectedOutput3);
    REQUIRE(output4 == expectedOutput4);
    REQUIRE(output5 == expectedOutput5);
}

TEST_CASE("Mix of various output iterators with pipe")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedOutput1 = {6, 12, 18};
    std::vector<int> expectedOutput2 = {2, 4};
    std::vector<int> expectedOutput3 = {2, 10};
    std::vector<int> expectedOutput4 = {1, 5, 7};
    std::vector<char> expectedOutput5 = {'A', 'A', 'A'};
    
    std::vector<int> output1;
    std::vector<int> output2;
    std::vector<int> output3;
    std::vector<int> output4;
    std::vector<char> output5;
    
    auto const times2 = pipes::transform([](int i) { return i*2; });
    auto const divideBy2 = pipes::transform([](int i) { return i/2; });
    auto const pairUpWithA = pipes::transform([](int i) { return std::make_pair(i, 'A'); });
    
    
    std::copy(begin(numbers), end(numbers), pipes::switch_(pipes::case_( [](int n){ return n % 3 == 0; } ) >>= times2 >>= back_inserter(output1),
                                                           pipes::case_( [](int n){ return n % 2 == 0; } ) >>= divideBy2 >>= pipes::partition([](int n){ return n % 2 == 0; },
                                                                                                                                             back_inserter(output2),
                                                                                                                                             times2 >>= back_inserter(output3)
                                                                                                                                            ),
                                                           pipes::case_( [](int n){ return n % 1 == 0; } ) >>= pairUpWithA >>= pipes::unzip(
                                                                                                                                            back_inserter(output4),
                                                                                                                                            back_inserter(output5)
                                                                                                                                           )
                                                                                                                      ));
    
    REQUIRE(output1 == expectedOutput1);
    REQUIRE(output2 == expectedOutput2);
    REQUIRE(output3 == expectedOutput3);
    REQUIRE(output4 == expectedOutput4);
    REQUIRE(output5 == expectedOutput5);
}

TEST_CASE("Transform and filter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {8, 16, 24, 32, 40};
    
    auto const isEven = [](int n){ return n % 2 == 0; };
    auto const times2 = [](int n){ return n * 2; };

    std::vector<int> results;
    std::copy(begin(input), end(input), pipes::filter(isEven)(pipes::transform(times2)(pipes::transform(times2)(back_inserter(results)))));

    REQUIRE(results == expected);
}

TEST_CASE("Sequence of output iterators, no algorithms")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
    
    auto const times2 = pipes::transform([](int n){ return n * 2; });
    std::vector<int> results;
    
    numbers >>= pipes::funnel >>= times2(times2(back_inserter(results)));
    
    REQUIRE(results == expected);
}

TEST_CASE("Sequence of output iterators, no algorithms, with pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
    
    auto const times2 = [](int n){ return n * 2; };
    std::vector<int> results;
    
    numbers >>= pipes::funnel >>= pipes::transform(times2) >>= pipes::transform(times2) >>= back_inserter(results);
    
    REQUIRE(results == expected);
}

std::vector<int> operator|(std::vector<int> const& v1, std::vector<int> const& v2)
{
    return v2;
}

TEST_CASE("Sequence of input ranges and output iterators, with pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};
    
    auto const times2 = [](int n){ return n * 2; };
    std::vector<int> results;
    
    numbers | numbers | numbers >>= pipes::funnel >>= pipes::transform(times2) >>= pipes::transform(times2) >>= back_inserter(results);
    
    REQUIRE(results == expected);
}
