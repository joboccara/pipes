#include "catch.hpp"
#include "pipes/operator.hpp"
#include "pipes/push_back.hpp"
#include "pipes/tap.hpp"
#include "pipes/transform.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("tap")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    std::vector<int> results;

    input >>= pipes::transform([](auto const& x) { return x * 2; })
          >>= pipes::tap([&expected, i = uint{0}](auto const& x) mutable { REQUIRE(x == expected[i++]); })
          >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}