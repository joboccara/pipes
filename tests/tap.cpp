#include "catch.hpp"
#include "pipes/operator.hpp"
#include "pipes/push_back.hpp"
#include "pipes/tap.hpp"
#include "pipes/transform.hpp"

#include <algorithm>
#include <vector>
#include <list>

typedef unsigned int uint;

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

TEST_CASE("tap operator=")
{
    using range_type = std::list<int>;

    class FrontInserter
    {
    public:
        FrontInserter(range_type& range) : range_(range) {}

        void operator()(range_type::value_type value) const 
        {
            range_.push_front(value);
        } 
    private:
        range_type& range_;
    };

    range_type results1, results2, results3, results4;
    
    auto tap1 = pipes::tap(FrontInserter(results1)) >>= pipes::push_back(results2);
    auto tap2 = pipes::tap(FrontInserter(results3)) >>= pipes::push_back(results4);
    
    tap2 = tap1;
    pipes::send(0, tap2);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}