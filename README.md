This projects aims at enriching STL output iterators such as `std::inserter`.

`sorted_inserter` provides a lighter version of `std::inserter` by making the position to insert optional. Indeed in the majority of cases where it is used in algoritms, the user find himself compelled to provide some position, and usually chooses begin or end by defult, which don't make sense.

Usage example:

```cpp
std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
std::set<int> results;
std::copy(begin(v), end(v), sorted_inserter(results)); // note the position is left to the set to determine

//results contains { -4, 1, 2, 3, 7, 8, 10 }
```

`map_aggregator` provides the possibility to embark an aggregator function in the inserter iterator, so that new elements whose key is already present in the map can be merged with the existent (e.g. have their values added together).

```cpp
std::vector<std::pair<int, std::string>> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
std::vector<std::pair<int, std::string>> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
std::map<int, std::string> results;

std::copy(entries.begin(), entries.end(), map_aggregator(results, concatenateStrings));
std::copy(entries2.begin(), entries2.end(), map_aggregator(results, concatenateStrings));

// results contains { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
```

`set_aggreagator` provides a similar functionality for aggregating elements into sets.
