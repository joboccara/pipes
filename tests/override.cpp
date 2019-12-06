#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("Override existing contents on STL containers")
{
    auto input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> results = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    input >>= pipes::override(results);
    
    REQUIRE(results == expected);
}

namespace
{
    namespace MyCollectionNamespace
    {
        struct MyCollection
        {
            std::vector<int> data_;
            explicit MyCollection(std::vector<int> data) : data_(std::move(data)) {}
        };
        
        auto begin(MyCollection& myCollection) { return begin(myCollection.data_); }
    }
}

TEST_CASE("Override existing contents on collections with ADL begin")
{
    auto input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto results = MyCollectionNamespace::MyCollection(std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    input >>= pipes::override(results);
    
    REQUIRE(results.data_ == expected);
}

TEST_CASE("Override contents of a C array")
{
    auto input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    int results[10] = {};
    input >>= pipes::override(results);
    
    REQUIRE(std::equal(std::begin(results), std::end(results), begin(expected), end(expected)));
}

TEST_CASE("Override can assign to a data member of the output collection")
{
    struct P
    {
        int x = 0;
        int y = 0;
        
        bool operator==(P const& other) const{ return x == other.x && y == other.y; }
    };
    
    auto const expected = std::vector<P>{ {1,0}, {2,0}, {3,0}, {4,0}, {5,0} };

    auto const xs = std::vector<int>{1, 2, 3, 4, 5};
    auto results = std::vector<P>(5);
    
    xs >>= pipes::override(results, &P::x);
    
    REQUIRE(results == expected);
}
