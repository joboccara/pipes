#include "catch.hpp"
#include "pipes/for_each.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("for_each")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = input;
    
    std::vector<int> results;
    auto legacyInsertion = [&results](int number) { results.push_back(number); };
    
    std::copy(begin(input), end(input), pipes::for_each(legacyInsertion));
    
    REQUIRE(results == expected);
}

TEST_CASE("for_each's iterator category should be std::output_iterator_tag")
{
    auto inserter = [](){};
    static_assert(std::is_same<decltype(pipes::for_each(inserter))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("for_each::operator= (called in the _Recheck function of Visual Studio's STL)")
{
    struct IncrementContext
    {
        int& context_;
        explicit IncrementContext(int& context) : context_(context){}
        void operator()(int) const { ++context_; }
    };

    auto context1 = 42;
    auto for_each1 = pipes::for_each(IncrementContext{context1});
    auto context2 = 42;
    auto for_each2 = pipes::for_each(IncrementContext{context2});
    
    for_each2 = for_each1;
    pipes::send(0, for_each2);
    REQUIRE(context1 == 43);
    REQUIRE(context2 == 42);
}
