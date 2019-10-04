#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <vector>

TEST_CASE("repeat sends n consecutive copies of each element to the next pipe")
{
  auto const input = std::vector<int>{1, 2, 3, 4, 5};
  auto const expected = std::vector<int>{1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5};

  auto results = std::vector<int>{};

  input >>= pipes::repeat(2)
        >>= pipes::push_back(results);

  REQUIRE(results == expected);
}
