#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "map_aggregator.hpp"
#include "set_aggregator.hpp"
#include "sorted_inserter.hpp"

static bool testSortedInserter()
{
    std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
    std::set<int> expected = { -4, 1, 2, 3, 7, 8, 10 };
    std::set<int> results;
    std::copy(begin(v), end(v), sorted_inserter(results));

    return results == expected;
}

static std::string concatenateStrings(std::string const& s1, std::string const& s2)
{
    return s1 + s2;
}

bool testMapAggregator()
{
    std::vector<std::pair<int, std::string>> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    std::vector<std::pair<int, std::string>> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
    std::map<int, std::string> expected = { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
    std::map<int, std::string> results;

    std::copy(entries.begin(), entries.end(), map_aggregator(results, concatenateStrings));
    std::copy(entries2.begin(), entries2.end(), map_aggregator(results, concatenateStrings));

    return results == expected;
}

struct Value
{
    int i;
    std::string s;
};

bool operator==(Value const& value1, Value const& value2)
{
    return value1.i == value2.i && value1.s == value2.s;
}

bool operator<(Value const& value1, Value const& value2)
{
    if (value1.i < value2.i) return true;
    if (value2.i < value1.i) return false;
    return value1.s < value2.s;
}

Value concatenateValues(Value const& value1, Value const& value2)
{
    if (value1.i != value2.i) throw std::runtime_error("Incompatible values");
    return { value1.i, concatenateStrings(value1.s, value2.s) };
}

template <typename OStream>
OStream& operator<<(OStream& os, Value const& value)
{
    os << value.i << '-' << value.s;
    return os;
}

bool testSetAggregator()
{
    std::vector<Value> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    std::vector<Value> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
    std::set<Value> expected = { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
    std::set<Value> results;

    std::copy(entries.begin(), entries.end(), set_aggregator(results, concatenateValues));
    std::copy(entries2.begin(), entries2.end(), set_aggregator(results, concatenateValues));    

    return results == expected;
}

template <typename TestFunction>
static void launchTest(std::string const& testName, TestFunction testFunction)
{
    std::cout << "Test - " << testName << ": " << (testFunction() ? "OK" : "FAILED") << '\n';
}

static void launchTests()
{
    std::cout << "=== TESTS ===" << std::endl;
    launchTest("sorted_insert", testSortedInserter);
    launchTest("map_aggregator", testMapAggregator);
    launchTest("set_aggregator", testSetAggregator);
}

int main()
{
    launchTests();
}

