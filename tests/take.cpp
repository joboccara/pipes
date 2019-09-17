#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <vector>

TEST_CASE("take takes the first N elements coming from a range")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{1, 2, 3, 4, 5, 6};
    
    auto result = std::vector<int>{};
    
    input >>= pipes::take(6)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("take takes the first N elements coming from another pipe")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{2, 4};
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0; })
          >>= pipes::take(2)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("take(N) takes all the elements if there are less than N")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{2, 4, 6, 8, 10};
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0; })
          >>= pipes::take(6)
          >>= pipes::push_back(result);
                            
    REQUIRE(result == expected);
}

TEST_CASE("take handles empty input")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{};

    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int){ return false; })
          >>= pipes::take(6)
          >>= pipes::push_back(result);
                            
    REQUIRE(result == expected);
}

TEST_CASE("takes can handle multiple values")
{
    auto const input1 = std::vector<int>{1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{10, 20, 30, 40, 50};
    
    auto const expected = std::vector<int>{11, 22, 33};
    
    auto result = std::vector<int>{};
    
    pipes::mux(input1, input2)
        >>= pipes::take(3)
        >>= pipes::transform([](int a, int b){ return a + b; })
        >>= pipes::push_back(result);
                            
    REQUIRE(result == expected);
}

TEST_CASE("take operator=")
{
    std::vector<int> results1, results2;
    auto pipeline1 = pipes::take(1) >>= pipes::push_back(results1);
    auto pipeline2 = pipes::take(2) >>= pipes::push_back(results2);
    
    pipeline2 = pipeline1;
    send(1, pipeline2);
    send(2, pipeline2);
    send(3, pipeline2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
