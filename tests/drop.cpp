#include "catch.hpp"
#include "pipes/pipes.hpp"

TEST_CASE("drop ignores the first N elements and passes on the elements after")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 6, 7, 8, 9, 10 };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::drop(5)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("drop ignores the first N elements coming from a pipe and passes on the elements after")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 6, 8, 10 };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0; })
    >>= pipes::drop(2)
    >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("drop ignores everything if N is greater than the size of the incoming input")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0; })
    >>= pipes::drop(10)
    >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("drop drops elements coming from several collections")
{
    auto const input1 = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{ 10, 20, 30, 40, 50};

    auto const expected = std::vector<int>{ 44, 55 };
    
    auto result = std::vector<int>{};
    
    pipes::mux(input1, input2)
        >>= pipes::drop(3)
        >>= pipes::transform([](int a, int b){ return a + b; })
        >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("drop doesn't send anything on if the input is empty")
{
    auto const input = std::vector<int>{ };
    auto const expected = std::vector<int>{ };
    
    auto result = std::vector<int>{};
    
    input >>= pipes::drop(10)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("drop operator=")
{
    auto const expected1 = std::vector<int>{ 3 };
    auto const expected2 = std::vector<int>{ };
    
    auto result1 = std::vector<int>{};
    auto result2 = std::vector<int>{};
    
    auto pipeline1 = pipes::drop(2) >>= pipes::push_back(result1);
    auto pipeline2 = pipes::drop(3) >>= pipes::push_back(result2);
    
    pipeline2 = pipeline1;
    
    pipes::send(1, pipeline2);
    pipes::send(2, pipeline2);
    pipes::send(3, pipeline2);

    REQUIRE(result1 == expected1);
    REQUIRE(result2 == expected2);
}
