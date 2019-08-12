#include "catch.hpp"
#include "custom_inserter.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("custom_inserter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = input;
    
    std::vector<int> results;
    auto legacyInsertion = [&results](int number) { results.push_back(number); };
    
    std::copy(begin(input), end(input), pipes::custom_inserter(legacyInsertion));
    
    REQUIRE(results == expected);
}

TEST_CASE("custom_inserter's iterator category should be std::output_iterator_tag")
{
    auto inserter = [](){};
    static_assert(std::is_same<decltype(pipes::custom_inserter(inserter))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("custom_inserter::operator= (called in the _Recheck function of Visual Studio's STL)")
{
    struct IncrementContext
    {
        int& context_;
        explicit IncrementContext(int& context) : context_(context){}
        void operator()(int) const { ++context_; }
    };

    auto context1 = 42;
    auto custom_inserter1 = pipes::custom_inserter(IncrementContext{context1});
    auto context2 = 42;
    auto custom_inserter2 = pipes::custom_inserter(IncrementContext{context2});
    
    custom_inserter2 = custom_inserter1;
    pipes::send(custom_inserter2, 0);
    REQUIRE(context1 == 43);
    REQUIRE(context2 == 42);
}
