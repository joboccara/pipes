#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <map>
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

        auto begin(MyCollection& myCollection)
        {
            using std::begin;
            return begin(myCollection.data_);
        }
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

TEST_CASE("Override can assign to the value of the output map")
{
    struct P
    {
        int x = 0;
        int y = 0;
        
        bool operator<(P const& other) const
        {
            if (x < other.x) return true;
            if (x > other.x) return false;
            return y < other.y;
        }
    };
    
    auto const expected = std::map<int, std::string>{ {1,"one"}, {2,"two"}, {3,"three"}, {4,"four"}, {5,"five"} };

    auto const xs = std::vector<std::string>{"one", "two", "three", "four", "five"};
    auto results = std::map<int, std::string>{ {1,""}, {2,""}, {3,""}, {4,""}, {5,""} };;
    
    xs >>= pipes::override(results, &std::pair<int const, std::string>::second);
    
    REQUIRE(results == expected);
}

TEST_CASE("Override can send results to a setter in the output collection")
{
    struct P
    {
        int x = 0;
        int y = 0;
        
        void setX(int aX){ x = aX; }
        
        bool operator==(P const& other) const{ return x == other.x && y == other.y; }
    };
    
    auto const expected = std::vector<P>{ {1,0}, {2,0}, {3,0}, {4,0}, {5,0} };

    auto const xs = std::vector<int>{1, 2, 3, 4, 5};
    auto results = std::vector<P>(5);
    
    xs >>= pipes::override(results, &P::setX);
    
    REQUIRE(results == expected);
}
