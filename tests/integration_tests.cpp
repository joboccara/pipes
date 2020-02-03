#include "catch.hpp"
#include "pipes/transform.hpp"
#include "pipes/filter.hpp"
#include "pipes/partition.hpp"
#include "pipes/unzip.hpp"
#include "pipes/switch.hpp"
#include "pipes/push_back.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("Mix of various pipes")
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


    std::copy(begin(numbers), end(numbers), pipes::switch_(pipes::case_( [](int n){ return n % 3 == 0; } ) >>= times2 >>= pipes::push_back(output1),
                                                           pipes::case_( [](int n){ return n % 2 == 0; } ) >>= divideBy2 >>= pipes::partition([](int n){ return n % 2 == 0; },
                                                                                                                                             pipes::push_back(output2),
                                                                                                                                             times2 >>= pipes::push_back(output3)
                                                                                                                                            ),
                                                           pipes::case_( [](int n){ return n % 1 == 0; } ) >>= pairUpWithA >>= pipes::unzip(
                                                                                                                                            pipes::push_back(output4),
                                                                                                                                            pipes::push_back(output5)
                                                                                                                                           )
                                                                                                                      ));

    REQUIRE(output1 == expectedOutput1);
    REQUIRE(output2 == expectedOutput2);
    REQUIRE(output3 == expectedOutput3);
    REQUIRE(output4 == expectedOutput4);
    REQUIRE(output5 == expectedOutput5);
}

TEST_CASE("Compatibility with STL algorithms")
{
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = {2, 4, 6, 8, 10};
    std::vector<int> results;

    std::copy(begin(input), end(input), pipes::transform([](int i){ return i * 2; }) >>= pipes::push_back(results));

    REQUIRE(expected == results);
}

TEST_CASE("Transform and filter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {8, 16, 24, 32, 40};

    auto const isEven = [](int n){ return n % 2 == 0; };
    auto const times2 = [](int n){ return n * 2; };

    std::vector<int> results;

    SECTION("chaining with >>=")
    {
        input >>= pipes::filter(isEven)
              >>= pipes::transform(times2)
              >>= pipes::transform(times2)
              >>= pipes::push_back(results);
        REQUIRE(results == expected);
    }
}

TEST_CASE("transform then filter calls the transform function only once per element")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto results = std::vector<int>{};
    auto output = std::vector<int>{};

    input >>= pipes::transform([&results](int i){ results.push_back(i); return i * 2; })
          >>= pipes::filter([](int i){ return i % 3 == 0; })
          >>= pipes::push_back(output);

    REQUIRE(results == expected);
}

TEST_CASE("Sequence of output iterators, no algorithms")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};

    auto const times2 = pipes::transform([](int n){ return n * 2; });
    std::vector<int> results;

    numbers >>= times2 >>= times2 >>= pipes::push_back(results);

    REQUIRE(results == expected);
}

TEST_CASE("Sequence of output iterators, no algorithms, with pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};

    auto const times2 = [](int n){ return n * 2; };
    std::vector<int> results;

    numbers >>= pipes::transform(times2) >>= pipes::transform(times2) >>= pipes::push_back(results);

    REQUIRE(results == expected);
}

std::vector<int> operator|(std::vector<int> const&, std::vector<int> const& v2)
{
    return v2;
}

TEST_CASE("Sequence of input ranges and output iterators, with pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40};

    auto const times2 = [](int n){ return n * 2; };
    std::vector<int> results;

    numbers | numbers | numbers >>= pipes::transform(times2) >>= pipes::transform(times2) >>= pipes::push_back(results);

    REQUIRE(results == expected);
}

namespace
{
namespace MyCollectionNamespace
{
    struct MyCollection
    {
        std::vector<int> data_;
    };


    auto begin(MyCollection const& myCollection)
    {
        using std::begin;
        return begin(myCollection.data_);
    }

    auto end(MyCollection const& myCollection)
    {
        using std::end;
        return end(myCollection.data_);
    }
}
}

TEST_CASE("Reading from a collection with ADL begin and end")
{
    auto input = MyCollectionNamespace::MyCollection{std::vector<int>{1, 2, 3, 4, 5}};
    auto const expected = std::vector<int>{2, 4, 6, 8, 10};
    auto results = std::vector<int>{};

    input >>= pipes::transform([](int i){ return i *2; }) >>= pipes::push_back(results);

    REQUIRE(results == expected);
}

TEST_CASE("A pipeline can accept rvalues as inputs")
{
    auto const getInput = [](){ return std::vector<int>{1, 2, 3, 4, 5}; };
    auto const expected = std::vector<int>{4, 8};
    
    auto results = std::vector<int>{};
    
    getInput() >>= pipes::filter([](int i){ return i % 2 == 0; })
               >>= pipes::transform([](int i){ return i * 2; })
               >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("Aggregation of pipes into reusable components")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto results = std::vector<int>{};

    SECTION("End of pipeline aggregated")
    {
        auto const expected = std::vector<int>{4, 8, 12, 16, 20};
        auto pipeline = pipes::filter([](int i) { return i % 2 == 0; })
                    >>= pipes::transform([](int i){ return i * 2;})
                    >>= pipes::push_back(results);

        input >>= pipeline;

        REQUIRE(results == expected);
    }

    SECTION("Middle of pipeline aggregated")
    {
        SECTION("Composite of simple pipes")
        {
            auto const expected = std::vector<int>{4, 8, 12, 16, 20};
            auto pipeline = pipes::filter([](int i) { return i % 2 == 0; })
                        >>= pipes::transform([](int i){ return i * 2;});

            input >>= pipeline >>= pipes::push_back(results);

            REQUIRE(results == expected);
        }

        SECTION("Composite + pipe")
        {
            auto const expected = std::vector<int>{8, 16, 24, 32, 40};

            auto pipeline = pipes::filter([](int i) { return i % 2 == 0; })
                        >>= pipes::transform([](int i){ return i * 2;});

            auto pipeline2 = pipeline >>= pipes::transform([](int i){ return i * 2;});

            input >>= pipeline2 >>= pipes::push_back(results);

            REQUIRE(results == expected);
        }

        SECTION("pipe + composite")
        {
            auto const expected = std::vector<int>{4, 8, 12, 16, 20, 24, 28, 32, 36, 40};

            auto pipeline = pipes::filter([](int i) { return i % 2 == 0; })
                        >>= pipes::transform([](int i){ return i * 2;});

            auto pipeline2 = pipes::transform([](int i){ return i * 2;}) >>= pipeline;

            input >>= pipeline2 >>= pipes::push_back(results);

            REQUIRE(results == expected);
        }

        SECTION("composite + composite")
        {
            auto const expected = std::vector<int>{8, 16, 24, 32, 40};

            auto pipeline = pipes::filter([](int i) { return i % 2 == 0; })
                        >>= pipes::transform([](int i){ return i * 2;});

            auto pipeline2 = pipeline >>= pipeline;

            input >>= pipeline2 >>= pipes::push_back(results);

            REQUIRE(results == expected);
        }
    }
}
