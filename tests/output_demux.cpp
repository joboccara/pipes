#include "catch.hpp"
#include "demux.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("demux dispatches an input to several destinations")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7};
    
    std::vector<int> multiplesOf3;
    std::vector<int> multiplesOf2Only;
    std::vector<int> multiplesOf1Only;
    
    std::copy(begin(numbers), end(numbers), pipes::demux(pipes::demux_if( [](int n){ return n % 3 == 0; } ).send_to(back_inserter(multiplesOf3)),
                                                                  pipes::demux_if( [](int n){ return n % 2 == 0; } ).send_to(back_inserter(multiplesOf2Only)),
                                                                  pipes::demux_if( [](int n){ return n % 1 == 0; } ).send_to(back_inserter(multiplesOf1Only)) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
}

TEST_CASE("demux can override existing results")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10, 0, 0, 0, 0, 0, 0};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7, 0, 0, 0, 0, 0, 0, 0};
    
    std::vector<int> multiplesOf3 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> multiplesOf2Only = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> multiplesOf1Only = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::copy(begin(numbers), end(numbers), pipes::demux(pipes::demux_if( [](int n){ return n % 3 == 0; } ).send_to(begin(multiplesOf3)),
                                                                  pipes::demux_if( [](int n){ return n % 2 == 0; } ).send_to(begin(multiplesOf2Only)),
                                                                  pipes::demux_if( [](int n){ return n % 1 == 0; } ).send_to(begin(multiplesOf1Only)) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
}

TEST_CASE("demux's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    bool isMultipleOf3(int n);
    static_assert(std::is_same<decltype(pipes::demux(pipes::demux_if(isMultipleOf3).send_to(back_inserter(output))))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("demux cannot override existing contents")
{
    /* This code should not compile as the output_demux_pipe is plugged on a vector::begin

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expectedMultiplesOf3 =     {3, 6, 9, 0, 0};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10, 0};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7, 0, 0};
    
    std::vector<int> multiplesOf3 = {0, 0, 0, 0, 0};
    std::vector<int> multiplesOf2Only = {0, 0, 0, 0, 0};
    std::vector<int> multiplesOf1Only = {0, 0, 0, 0, 0};
    
    std::copy(begin(numbers), end(numbers), pipes::demux(pipes::demux_if( [](int n){ return n % 3 == 0; } ).sendTo(begin(multiplesOf3)),
                                                                  pipes::demux_if( [](int n){ return n % 2 == 0; } ).sendTo(back_inserter(multiplesOf2Only)),
                                                                  pipes::demux_if( [](int n){ return n % 1 == 0; } ).sendTo(back_inserter(multiplesOf1Only)) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
     
    */
}
