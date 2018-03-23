#include "catch.hpp"
#include "output_partitioner.hpp"

TEST_CASE("output_partitioner")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9};

    auto const isEvenPartition = fluent::make_output_partitioner([](int n){ return n % 2 == 0; });
    
    std::vector<int> evens;
    std::vector<int> odds;
    
    std::copy(begin(input), end(input), isEvenPartition(std::back_inserter(evens), std::back_inserter(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
}

TEST_CASE("output_partitioner's iterator category should be std::output_iterator_tag")
{
    auto const isEvenPartition = fluent::make_output_partitioner([](int n){ return n % 2 == 0; });
    std::vector<int> output1, output2;
    static_assert(std::is_same<decltype(isEvenPartition(std::back_inserter(output1), std::back_inserter(output2)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("output_partitioner cannot override existing contents")
{
    /* This code should not compile
    
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9};
    
    auto const isEvenPartition = fluent::make_output_partitioner([](int n){ return n % 2 == 0; });
    
    std::vector<int> evens = {0, 0, 0, 0, 0};
    std::vector<int> odds = {0, 0, 0, 0, 0};
    
    std::copy(begin(input), end(input), isEvenPartition(begin(evens), end(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
     
    */
}
