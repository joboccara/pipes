#include "catch.hpp"
#include "demux.hpp"
#include "filter.hpp"
#include "transform.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("demux dispatches an input to several destinations")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expected1 = numbers;
    std::vector<int> expected2 = numbers;
    std::vector<int> expected3 = numbers;

    std::vector<int> results1, results2, results3;
    
    std::copy(begin(numbers), end(numbers), pipes::demux(back_inserter(results1), back_inserter(results2), back_inserter(results3)));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
    REQUIRE(results3 == expected3);
}

TEST_CASE("demux can override existing results")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expected1 = numbers;
    std::vector<int> expected2 = numbers;
    std::vector<int> expected3 = numbers;
    
    std::vector<int> results1(numbers.size(), 0);
    std::vector<int> results2(numbers.size(), 0);
    std::vector<int> results3(numbers.size(), 0);
    
    std::copy(begin(numbers), end(numbers), pipes::demux(begin(results1), begin(results2), begin(results3)));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
    REQUIRE(results3 == expected3);
}

TEST_CASE("demux can send data to other pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf2 = {2, 4, 6, 8, 10};
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9};
    std::vector<int> expectedMultiplesOf4 = {4, 8};

    std::vector<int> multiplesOf2, multiplesOf3, multiplesOf4;
    
    std::copy(begin(numbers), end(numbers), pipes::demux(pipes::filter([](int i){return i%2 == 0;}) >>= back_inserter(multiplesOf2),
                                                         pipes::filter([](int i){return i%3 == 0;}) >>= back_inserter(multiplesOf3),
                                                         pipes::filter([](int i){return i%4 == 0;}) >>= back_inserter(multiplesOf4)));
    
    REQUIRE(multiplesOf2 == expectedMultiplesOf2);
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf4 == expectedMultiplesOf4);
}

TEST_CASE("demux can be used as a tee")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedResults = {4, 8, 12, 16, 20};
    std::vector<int> expectedLog = {2, 4, 6, 8, 10};
    
    std::vector<int> results, log;
    
    std::copy(begin(numbers), end(numbers), pipes::filter([](int i){return i%2 == 0;})
                                        >>= pipes::demux(back_inserter(log),
                                                         pipes::transform([](int i){ return i*2; })
                                                         >>= back_inserter(results)));
    
    REQUIRE(results == expectedResults);
    REQUIRE(log == expectedLog);
}

TEST_CASE("demux's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(pipes::demux(back_inserter(output), back_inserter(output), back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("demux operator=")
{
    std::vector<int> results1, results2, results3, results4;
    auto demux1 = pipes::demux(back_inserter(results1), back_inserter(results2));
    auto demux2 = pipes::demux(back_inserter(results1), back_inserter(results2));

    demux2 = demux1;
    pipes::send(demux2, 0);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
