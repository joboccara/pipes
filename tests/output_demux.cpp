#include "catch.hpp"
#include "../output/demux.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("output::demux dispatches an input to several destinations")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7};
    
    std::vector<int> multiplesOf3;
    std::vector<int> multiplesOf2Only;
    std::vector<int> multiplesOf1Only;
    
    std::copy(begin(numbers), end(numbers), fluent::output::demux(fluent::demux_if( [](int n){ return n % 3 == 0; } ).sendTo(back_inserter(multiplesOf3)),
                                                                  fluent::demux_if( [](int n){ return n % 2 == 0; } ).sendTo(back_inserter(multiplesOf2Only)),
                                                                  fluent::demux_if( [](int n){ return n % 1 == 0; } ).sendTo(back_inserter(multiplesOf1Only)) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
}

TEST_CASE("output::demux can override existing results")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10, 0, 0, 0, 0, 0, 0};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7, 0, 0, 0, 0, 0, 0, 0};
    
    std::vector<int> multiplesOf3 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> multiplesOf2Only = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> multiplesOf1Only = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::copy(begin(numbers), end(numbers), fluent::output::demux(fluent::demux_if( [](int n){ return n % 3 == 0; } ).sendTo(begin(multiplesOf3)),
                                                                  fluent::demux_if( [](int n){ return n % 2 == 0; } ).sendTo(begin(multiplesOf2Only)),
                                                                  fluent::demux_if( [](int n){ return n % 1 == 0; } ).sendTo(begin(multiplesOf1Only)) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
}

TEST_CASE("output::demux's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    bool isMultipleOf3(int n);
    static_assert(std::is_same<decltype(fluent::output::demux(fluent::demux_if(isMultipleOf3).sendTo(back_inserter(output))))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("output::demux cannot override existing contents")
{
    /* This code should not compile as the output_demux_iterator is plugged on a vector::begin

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expectedMultiplesOf3 =     {3, 6, 9, 0, 0};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10, 0};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7, 0, 0};
    
    std::vector<int> multiplesOf3 = {0, 0, 0, 0, 0};
    std::vector<int> multiplesOf2Only = {0, 0, 0, 0, 0};
    std::vector<int> multiplesOf1Only = {0, 0, 0, 0, 0};
    
    std::copy(begin(numbers), end(numbers), fluent::output::demux(fluent::demux_if( [](int n){ return n % 3 == 0; } ).sendTo(begin(multiplesOf3)),
                                                                  fluent::demux_if( [](int n){ return n % 2 == 0; } ).sendTo(back_inserter(multiplesOf2Only)),
                                                                  fluent::demux_if( [](int n){ return n % 1 == 0; } ).sendTo(back_inserter(multiplesOf1Only)) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
     
    */
}
