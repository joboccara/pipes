#include "catch.hpp"
#include "switch.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("switch dispatches an input to the first matching destination")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7};
    
    std::vector<int> multiplesOf3;
    std::vector<int> multiplesOf2Only;
    std::vector<int> multiplesOf1Only;
    
    std::copy(begin(numbers), end(numbers), pipes::switch_(pipes::case_([](int n){ return n % 3 == 0; }) >>= back_inserter(multiplesOf3),
                                                           pipes::case_([](int n){ return n % 2 == 0; }) >>= back_inserter(multiplesOf2Only),
                                                           pipes::case_([](int n){ return n % 1 == 0; }) >>= back_inserter(multiplesOf1Only) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
}

TEST_CASE("switch can override existing results")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> expectedMultiplesOf2Only = {2, 4, 8, 10, 0, 0, 0, 0, 0, 0};
    std::vector<int> expectedMultiplesOf1Only = {1, 5, 7, 0, 0, 0, 0, 0, 0, 0};
    
    std::vector<int> multiplesOf3 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> multiplesOf2Only = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> multiplesOf1Only = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::copy(begin(numbers), end(numbers), pipes::switch_(pipes::case_([](int n){ return n % 3 == 0; }) >>= begin(multiplesOf3),
                                                           pipes::case_([](int n){ return n % 2 == 0; }) >>= begin(multiplesOf2Only),
                                                           pipes::case_([](int n){ return n % 1 == 0; }) >>= begin(multiplesOf1Only) ));
    
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf2Only == expectedMultiplesOf2Only);
    REQUIRE(multiplesOf1Only == expectedMultiplesOf1Only);
}

TEST_CASE("switch's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    bool isMultipleOf3(int n);
    static_assert(std::is_same<decltype(pipes::switch_(pipes::case_(isMultipleOf3) >>= back_inserter(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}
