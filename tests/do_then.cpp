#include <algorithm>
#include <vector>
#include "catch.hpp"

#include "pipes/for_each.hpp"
#include "pipes/do_then.hpp"

TEST_CASE("several operations in for_each")
{
    auto const inputs = std::vector<int>{1, 2, 3, 4, 5};
    auto const expected1 = std::vector<int>{2, 4, 6, 8, 10};
    auto const expected2 = std::vector<int>{2, 3, 4, 5, 6};
    auto const expected3 = std::vector<int>{-1, -2, -3, -4, -5};
    
    auto results1 = std::vector<int>{};
    auto results2 = std::vector<int>{};
    auto results3 = std::vector<int>{};
    
    inputs >>= pipes::for_each(pipes::do_([&](int i){ results1.push_back(i*2);}).
                             then_([&](int i){ results2.push_back(i+1);}).
                             then_([&](int i){ results3.push_back(-i);}));
    
    REQUIRE((results1 == expected1));
    REQUIRE((results2 == expected2));
    REQUIRE((results3 == expected3));
}

TEST_CASE("several operations in std::for_each")
{
    auto const inputs = std::vector<int>{1, 2, 3, 4, 5};
    auto const expected1 = std::vector<int>{2, 4, 6, 8, 10};
    auto const expected2 = std::vector<int>{2, 3, 4, 5, 6};
    auto const expected3 = std::vector<int>{-1, -2, -3, -4, -5};
    
    auto results1 = std::vector<int>{};
    auto results2 = std::vector<int>{};
    auto results3 = std::vector<int>{};
    
    std::for_each(begin(inputs), end(inputs), pipes::do_([&](int i){ results1.push_back(i*2);}).
                                                     then_([&](int i){ results2.push_back(i+1);}).
                                                     then_([&](int i){ results3.push_back(-i);}));
    
    REQUIRE((results1 == expected1));
    REQUIRE((results2 == expected2));
    REQUIRE((results3 == expected3));
}

TEST_CASE("several mutating operations in std::for_each")
{
    auto results = std::vector<int>{1, 2, 3, 4, 5};
    auto const expected = std::vector<int>{-3, -5, -7, -9, -11};
    
    std::for_each(begin(results), end(results), pipes::do_([&](int& i){ i *= 2;}).
                                                       then_([&](int& i){ i += 1;}).
                                                       then_([&](int& i){ i*= -1;}));
    
    REQUIRE((results == expected));
}
