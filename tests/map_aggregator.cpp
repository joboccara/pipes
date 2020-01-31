#include <map>
#include <utility>
#include <vector>

#include "catch.hpp"
#include "pipes/map_aggregator.hpp"

std::string concatenateStrings(std::string const& s1, std::string const& s2)
{
    return s1 + s2;
}

TEST_CASE("map_aggregator")
{
    std::map<int, std::string> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    std::map<int, std::string> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
    
    std::map<int, std::string> expected = { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
    std::map<int, std::string> results;
    
    std::copy(entries.begin(), entries.end(), pipes::map_aggregator(results, concatenateStrings));
    std::copy(entries2.begin(), entries2.end(), pipes::map_aggregator(results, concatenateStrings));
    
    REQUIRE((results == expected));
}

TEST_CASE("map_aggregator with operator>>=")
{
    std::map<int, std::string> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    std::map<int, std::string> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
    
    std::map<int, std::string> expected = { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
    std::map<int, std::string> results;
    
    entries >>= pipes::map_aggregator(results, std::plus<>{});
    
    entries2 >>= pipes::map_aggregator(results, std::plus<>{});
    
    REQUIRE((results == expected));
}

TEST_CASE("map_aggregator from vector of pairs")
{
    std::vector<std::pair<int, std::string>> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    std::vector<std::pair<int, std::string>> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
    
    std::map<int, std::string> expected = { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
    std::map<int, std::string> results;
    
    std::copy(entries.begin(), entries.end(), pipes::map_aggregator(results, concatenateStrings));
    std::copy(entries2.begin(), entries2.end(), pipes::map_aggregator(results, concatenateStrings));
    
    REQUIRE((results == expected));
}

TEST_CASE("map_aggregator's iterator category should be std::output_iterator_tag")
{
    std::map<int, std::string> results;
    static_assert(std::is_same<decltype(pipes::map_aggregator(results, concatenateStrings))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("map_aggregator::operator=")
{
    auto results1 = std::map<int, std::string>{};
    auto map_aggregator1 = pipes::map_aggregator(results1, concatenateStrings);
    auto results2 = std::map<int, std::string>{};
    auto map_aggregator2 = pipes::map_aggregator(results2, concatenateStrings);
    
    map_aggregator2 = map_aggregator1;
    pipes::send(std::make_pair(0, std::string{"zero"}), map_aggregator2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
