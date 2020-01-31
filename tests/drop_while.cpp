#include "catch.hpp"
#include "pipes/pipes.hpp"

TEST_CASE("drop_while ignores the first N elements and passes on the elements after")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 6, 7, 8, 9, 10 };

    auto result = std::vector<int>{};

    input >>= pipes::drop_while([](int i){ return i != 6; })
          >>= pipes::push_back(result);

    REQUIRE(result == expected);
}

TEST_CASE("drop_while ignores the first elements coming from a pipe and passes on the elements after")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ 6, 8, 10 };

    auto result = std::vector<int>{};

    input >>= pipes::filter([](int i){ return i % 2 == 0; })
          >>= pipes::drop_while([](int i){ return i < 6; })
          >>= pipes::push_back(result);

    REQUIRE(result == expected);
}

TEST_CASE("drop_while ignores everything if the predicate is always false")
{
    auto const input = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{ };

    auto result = std::vector<int>{};
  
    input >>= pipes::filter([](int){ return false; })
          >>= pipes::drop_while([](int i){ return i < 6; })
          >>= pipes::push_back(result);

    REQUIRE(result == expected);
}

TEST_CASE("drop_while drops elements coming from several collections")
{
    auto const input1 = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const input2 = std::vector<int>{ 10, 20, 30, 40, 50};

    auto const expected = std::vector<int>{ 33, 44, 55 };

    auto result = std::vector<int>{};

    pipes::mux(input1, input2)
        >>= pipes::drop_while([](int a, int b){ return a < 3 && b < 40; })
        >>= pipes::transform([](int a, int b){ return a + b; })
        >>= pipes::push_back(result);

    REQUIRE(result == expected);
}

TEST_CASE("drop_while doesn't send anything on if the input is empty")
{
    auto const input = std::vector<int>{ };
    auto const expected = std::vector<int>{ };

    auto result = std::vector<int>{};

    input >>= pipes::drop_while([](int i){ return i < 6; })
          >>= pipes::push_back(result);

    REQUIRE(result == expected);
}

TEST_CASE("drop_while operator=")
{
    auto const expected1 = std::vector<int>{ 2, 3 };
    auto const expected2 = std::vector<int>{ };

    auto result1 = std::vector<int>{};
    auto result2 = std::vector<int>{};

    auto predicate = [](int i){ return i < 2; };

    auto pipeline1 = pipes::drop_while(predicate) >>= pipes::push_back(result1);
    auto pipeline2 = pipes::drop_while(predicate) >>= pipes::push_back(result2);

    pipeline2 = pipeline1;

    pipes::send(1, pipeline2);
    pipes::send(2, pipeline2);
    pipes::send(3, pipeline2);

    REQUIRE(result1 == expected1);
    REQUIRE(result2 == expected2);
}
