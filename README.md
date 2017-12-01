This library provides output iterators that enrich and complement the ones of the STL, such as `std::back_inserter`.

#sorted_inserter

In the majority of cases where it is used in algoritms, `std::inserter` forces its user to provide a position. It makes sense for un-sorted containers such as `std::vector`, but for sorted containers such as `std::set` we end up choosing begin or end by defult, which doesn't make sense:

```cpp
std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
std::set<int> results;
std::copy(begin(v), end(v), std::inserter(results, end(results)));
```
`sorted_inserter` removes this constraint by making the position optional. If no hint is passed, the containers is left to determine the correct position to insert:

```cpp
std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
std::set<int> results;
std::copy(begin(v), end(v), sorted_inserter(results));

//results contains { -4, 1, 2, 3, 7, 8, 10 }
```
Read the [full story](https://www.fluentcpp.com/2017/03/17/smart-iterators-for-inserting-into-sorted-container/) about `sorted_inserter`.

#map_aggregator

`map_aggregator` provides the possibility to embark an aggregator function in the inserter iterator, so that new elements whose **key is already present in the map** can be merged with the existent (e.g. have their values added together).

```cpp
std::vector<std::pair<int, std::string>> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
std::vector<std::pair<int, std::string>> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
std::map<int, std::string> results;

std::copy(entries.begin(), entries.end(), map_aggregator(results, concatenateStrings));
std::copy(entries2.begin(), entries2.end(), map_aggregator(results, concatenateStrings));

// results contains { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
```

`set_aggreagator` provides a similar functionality for aggregating elements into sets.

Read the [full story](https://www.fluentcpp.com/2017/03/21/smart-iterator-aggregating-new-elements-existing-ones-map-set/) about `map_aggregator` and `set_aggregator`.

#output_transformer

`output_transformer` is an output iterator that wraps around another output iterator. It embarks a function `f` and, when it receives a value, applies `f` on it and sends the result to the output iterator that it is wrapping.

The helper function `make_output_transformer` takes a function and returns an `output_transformer` embarking (a copy of) that function.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5};

auto const times2 = make_output_transformer([](int i) { return i*2; });

std::vector<int> results;
std::copy(begin(input), end(input), times2(std::back_inserter(results)));

// results contains {2, 4, 6, 8, 10}
```
Read the [full story](https://www.fluentcpp.com/2017/11/28/output-iterator-adaptors-symmetry-range-adaptors/) about smart output iterators.

#dead_end_iterator

`dead_end_iterator` is an iterator that doesn't do anything with the value it receives. It is useful for selecting only some data coming out of an algorithm that has several outputs.
An example of such algorithm is [`set_segregate`](https://github.com/joboccara/sets).
