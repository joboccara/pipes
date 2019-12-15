#include "catch.hpp"
#include "pipes/pipes.hpp"

TEST_CASE("mux can send info from several ranges to transform")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const expected = std::vector<int>{11, 22, 33, 44, 55};
    
    auto results = std::vector<int>{};
    
    pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("mux stops when the shortest collection is empty")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const expected = std::vector<int>{11, 22, 33, 44};
    
    auto results = std::vector<int>{};
    
    pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("mux doesn't do anything with empty ranges")
{
    SECTION("empty-full")
    {
        auto const input1 = std::vector<int>{};
        auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
        auto const expected = std::vector<int>{};
        
        auto results = std::vector<int>{};
        
        pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
    
    SECTION("full-empty")
    {
        auto const input1 = std::vector<int>{1, 2, 3, 4};
        auto const input2 = std::vector<int>{};
        auto const expected = std::vector<int>{};
        
        auto results = std::vector<int>{};
        
        pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
    
    SECTION("empty-empty")
    {
        auto const input1 = std::vector<int>{};
        auto const input2 = std::vector<int>{};
        auto const expected = std::vector<int>{};
        
        auto results = std::vector<int>{};
        
        pipes::mux(input1, input2) >>= pipes::transform([](int a, int b) { return a + b; }) >>= pipes::push_back(results);
        
        REQUIRE(results == expected);
    }
}

TEST_CASE("mux filter transform")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const expected = std::vector<int>{10, 40, 90};

    auto results = std::vector<int>{};
    
    pipes::mux(input1, input2)
    >>= pipes::filter([](int a, int b){ return a + b < 41; })
    >>= pipes::transform([](int a, int b) { return a * b; })
    >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("mux partition")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const expected1 = std::vector<int>{10, 40, 90};
    auto const expected2 = std::vector<int>{36, 45};

    auto results1 = std::vector<int>{};
    auto results2 = std::vector<int>{};

    pipes::mux(input1, input2)
    >>= pipes::partition([](int a, int b){ return a + b < 41; },
                         pipes::transform(std::multiplies<>{}) >>= pipes::push_back(results1),
                         pipes::transform([](int a, int b){ return b - a;}) >>= pipes::push_back(results2));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
}

TEST_CASE("mux allows to apply a transform to 3 collections")
{
    auto const v1 = std::vector<int>{1, 2, 3, 4, 5};
    auto const v2 = std::vector<int>{10, 20, 30, 40, 50};
    auto const v3 = std::vector<int>{100, 200, 300, 400, 500};

    auto const expected = std::vector<int>{111, 222, 333, 444, 555};
    
    auto results = std::vector<int>{};
    
    pipes::mux(v1, v2, v3)
    >>= pipes::transform([](int a, int b, int c){ return a + b + c;})
    >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}
