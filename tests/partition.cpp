#include "catch.hpp"
#include "pipes/override.hpp"
#include "pipes/partition.hpp"
#include "pipes/push_back.hpp"

TEST_CASE("partition")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9};

    std::vector<int> evens;
    std::vector<int> odds;
    
    std::copy(begin(input), end(input), pipes::partition([](int n){ return n % 2 == 0; },
                                                         pipes::push_back(evens),
                                                         pipes::push_back(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
}

TEST_CASE("partition can override existing results")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10, 0, 0, 0, 0, 0};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9, 0, 0, 0, 0, 0};
    
    std::vector<int> evens = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> odds = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::copy(begin(input), end(input), pipes::partition([](int n){ return n % 2 == 0; },
                                                         pipes::override(evens),
                                                         pipes::override(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
}

TEST_CASE("partition's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output1, output2;
    auto predicate = [](int n){ return n % 2 == 0; };
    static_assert(std::is_same<decltype(pipes::partition(predicate, pipes::push_back(output1), pipes::push_back(output2)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("partition operator>>=")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9};

    std::vector<int> evens;
    std::vector<int> odds;
    
    input >>= pipes::partition([](int n){ return n % 2 == 0; },
                               pipes::push_back(evens),
                               pipes::push_back(odds));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
}

TEST_CASE("partition operator=")
{
    std::vector<int> results1, results2, results3, results4;
    auto predicate = [](int i){ return i > 0; };
    auto pipeline1 = pipes::partition(predicate, pipes::push_back(results1), pipes::push_back(results2));
    auto pipeline2 = pipes::partition(predicate, pipes::push_back(results3), pipes::push_back(results4));
    
    pipeline2 = pipeline1;
    send(1, pipeline2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
