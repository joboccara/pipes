#include "catch.hpp"
#include "pipes/pipes.hpp"

TEST_CASE("combinations genreates each couple of elements in a range once")
{
   auto const inputs = std::vector<int>{ 1, 2, 3, 4, 5 };
   auto const expected = std::vector<std::pair<int, int>>{
      { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 },
                { 2, 3 }, { 2, 4 }, { 2, 5 },
                          { 3, 4 }, { 3, 5 },
                                    { 4, 5 }
   };

   auto results = std::vector<std::pair<int, int>>{};
   
    pipes::combinations(inputs)
        >>= pipes::transform([](int i, int j){ return std::make_pair(i, j); })
        >>= pipes::push_back(results);
   REQUIRE(results == expected);
}

TEST_CASE("combinations genreates nothing for an empty input")
{
   auto const inputs = std::vector<int>{};
   auto const expected = std::vector<std::pair<int, int>>{};

   auto results = std::vector<std::pair<int, int>>{};
   
   pipes::combinations(inputs)
       >>= pipes::transform([](int i, int j){ return std::make_pair(i, j); })
       >>= pipes::push_back(results);
    REQUIRE(results == expected);
}
