#include "catch.hpp"
#include "pipes/fork.hpp"
#include "pipes/filter.hpp"
#include "pipes/override.hpp"
#include "pipes/transform.hpp"
#include "pipes/push_back.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("fork dispatches an input to several destinations")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expected1 = numbers;
    std::vector<int> expected2 = numbers;
    std::vector<int> expected3 = numbers;

    std::vector<int> results1, results2, results3;
    
    std::copy(begin(numbers), end(numbers), pipes::fork(pipes::push_back(results1), pipes::push_back(results2), pipes::push_back(results3)));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
    REQUIRE(results3 == expected3);
}

TEST_CASE("fork can override existing results")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expected1 = numbers;
    std::vector<int> expected2 = numbers;
    std::vector<int> expected3 = numbers;
    
    std::vector<int> results1(numbers.size(), 0);
    std::vector<int> results2(numbers.size(), 0);
    std::vector<int> results3(numbers.size(), 0);
    
    std::copy(begin(numbers), end(numbers), pipes::fork(pipes::override(results1), pipes::override(results2), pipes::override(results3)));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
    REQUIRE(results3 == expected3);
}

TEST_CASE("fork can send data to other pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf2 = {2, 4, 6, 8, 10};
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9};
    std::vector<int> expectedMultiplesOf4 = {4, 8};

    std::vector<int> multiplesOf2, multiplesOf3, multiplesOf4;
    
    std::copy(begin(numbers), end(numbers), pipes::fork(pipes::filter([](int i){return i%2 == 0;}) >>= pipes::push_back(multiplesOf2),
                                                         pipes::filter([](int i){return i%3 == 0;}) >>= pipes::push_back(multiplesOf3),
                                                         pipes::filter([](int i){return i%4 == 0;}) >>= pipes::push_back(multiplesOf4)));
    
    REQUIRE(multiplesOf2 == expectedMultiplesOf2);
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf4 == expectedMultiplesOf4);
}

TEST_CASE("fork can be used as a tee")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedResults = {4, 8, 12, 16, 20};
    std::vector<int> expectedLog = {2, 4, 6, 8, 10};
    
    std::vector<int> results, log;
    
    std::copy(begin(numbers), end(numbers), pipes::filter([](int i){return i%2 == 0;})
                                        >>= pipes::fork(pipes::push_back(log),
                                                         pipes::transform([](int i){ return i*2; })
                                                         >>= pipes::push_back(results)));
    
    REQUIRE(results == expectedResults);
    REQUIRE(log == expectedLog);
}

TEST_CASE("fork's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(pipes::fork(pipes::push_back(output), pipes::push_back(output), pipes::push_back(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("fork operator=")
{
    std::vector<int> results1, results2, results3, results4;
    auto fork1 = pipes::fork(pipes::push_back(results1), pipes::push_back(results2));
    auto fork2 = pipes::fork(pipes::push_back(results1), pipes::push_back(results2));

    fork2 = fork1;
    pipes::send(0, fork2);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
