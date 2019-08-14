This library provides output iterators that enrich and complement the ones of the STL, such as `std::back_inserter`.

All components are located in the namespace `pipes`.

## `transform`

`transform` is a pipe that takes a function `f` and, when it receives a value, applies `f` on it and sends the result on to the next pipe.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5};
std::vector<int> results;

input >>= pipes::funnel
>>= pipes::transform([](int i) { return i*2; })
>>= back_inserter(results);

// results contains {2, 4, 6, 8, 10}
```

## `filter`

`filter` is a pipe that takes a predicate `p` and, when it receives a value `x`, sends the result on to the next pipe iif `p(x)` is `true`.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> results;

input >>= pipes::funnel
>>= pipes::filter([](int i){ return i % 2 == 0; })
>>= back_inserter(results);

// results contains {2, 4, 6, 8, 10}
```

## `demux`

`demux` is a pipe that takes any number of pipes, and sends a copy of the values it receives to each of those pipes.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5};
std::vector<int> results1;
std::vector<int> results2;
std::vector<int> results3;

input >>= pipes::funnel
>>= pipes::demux(back_inserter(results1),
back_inserter(results2),
back_inserter(results3));

// results1 contains {1, 2, 3, 4, 5}
// results2 contains {1, 2, 3, 4, 5}
// results3 contains {1, 2, 3, 4, 5}
```

## `partition`

`partition` is a pipe that takes a predicate `p` and two other pipes. When it receives a value `x`, sends the result on to the first pipe iif `p(x)` is `true`, and to the second pipe if `p(x)` is `false`.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> evens;
std::vector<int> odds;

input >>= pipes::funnel
>>= pipes::partition([](int n){ return n % 2 == 0; },
back_inserter(evens),
back_inserter(odds));

// evens contains {2, 4, 6, 8, 10}
// odds contains {1, 3, 5, 7, 9}
```

## `switch`

`switch_` is a pipe that takes several `case_` branches. Each branch contains a predicate and a pipe. When it receives a value, it tries it successively on the predicates of each branch, and sends the value on to the pipe of the first branch where the predicate returns `true`.
The `default_` branch is equivalent to one that takes a predicate that returns always `true`. Having a `default_` branch is not mandatory.

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> multiplesOf4;
std::vector<int> multiplesOf3;
std::vector<int> rest;

numbers >>= pipes::funnel
>>= pipes::switch_(pipes::case_([](int n){ return n % 4 == 0; }) >>= back_inserter(multiplesOf4),
pipes::case_([](int n){ return n % 3 == 0; }) >>= back_inserter(multiplesOf3),
pipes::default_ >>= back_inserter(rest) ));

// multiplesOf4 contains {4, 8};
// multiplesOf3 contains {3, 6, 9};
// rest contains {1, 2, 5, 7, 10};
```

## `tee`

`tee` is a pipe that takes one other pipe, and sends a copy of the values it receives to of this pipes before sending in on to the next pipe.
Like the `tee` command on UNIX, this pipe is useful to take a peek at intermediary results.

```cpp
auto const inputs = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
auto intermediaryResults = std::vector<int>{};
auto results = std::vector<int>{};

inputs >>= pipes::funnel
>>= pipes::tee(back_inserter(intermediaryResults))
>>= back_inserter(results);

// intermediaryResults contains {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}
// results contains {12, 14, 16, 18, 20}
```

## `unzip`

`unzip` is a pipe that takes N other pipes. When it receives a `std::pair` or `std::tuple` of size N (for `std::pair` N is 2), it sends each of its components to the corresponding output pipe:

```cpp
std::map<int, std::string> entries = { {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"} };
std::vector<int> keys;
std::vector<std::string> values;

entries >>= pipes::funnel
>>= pipes::unzip(back_inserter(keys),
back_inserter(values)));

// keys contains {1, 2, 3, 4, 5};
// values contains {"one", "two", "three", "four", "five"};
```

## `dev_null`

`dev_null` is a pipe that doesn't do anything with the value it receives. It is useful for selecting only some data coming out of an algorithm that has several outputs.
An example of such algorithm is [`set_segregate`](https://github.com/joboccara/sets):

```cpp
std::set<int> setA = {1, 2, 3, 4, 5};
std::set<int> setB =       {3, 4, 5, 6, 7};

std::vector<int> inAOnly;
std::vector<int> inBoth;

sets::set_seggregate(setA, setB,
back_inserter(inAOnly),
back_inserter(inBoth),
dev_null{});

// inAOnly contains {1, 2}
// inBoth contains {3, 4, 5}
```

## `sorted_inserter`

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

## `custom_inserter`

`custom_inserter` takes a function (or function object) that inserts into an arbitrary output container. The purpose of this iterator is to give legacy code that does not use STL containers access to STL algorithms:

```cpp
std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};

void legacyInsert(int number, DarkLegacyStructure const& thing); // this function inserts into the old non-STL container

DarkLegacyStructure legacyStructure = // ...

std::copy(begin(input), end(input), custom_inserter([&legacyStructure](int number){ legacyInsert(number, legacyStructure); });
```

Read the [full story](https://www.fluentcpp.com/2017/11/24/how-to-use-the-stl-in-legacy-code/) about making legacy code compatible with the STL.

Note that `custom_inserter` goes along with a helper function object, `do_`, that allows to perfom several actions sequentially on the output of the algorithm:
 
 ```cpp
 std::copy(begin(input), end(input), pipes::custom_inserter(pipes::do_([&](int i){ results1.push_back(i*2);}).
                                                                   then_([&](int i){ results2.push_back(i+1);}).
                                                                   then_([&](int i){ results3.push_back(-i);})));

 ```

## `map_aggregator`

`map_aggregator` provides the possibility to embark an aggregator function in the inserter iterator, so that new elements whose **key is already present in the map** can be merged with the existent (e.g. have their values added together).

```cpp
std::vector<std::pair<int, std::string>> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
std::vector<std::pair<int, std::string>> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
std::map<int, std::string> results;

std::copy(entries.begin(), entries.end(), map_aggregator(results, concatenateStrings));
std::copy(entries2.begin(), entries2.end(), map_aggregator(results, concatenateStrings));

// results contains { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} }
```

`set_aggreagator` provides a similar functionality for aggregating elements into sets.

Read the [full story](https://www.fluentcpp.com/2017/03/21/smart-iterator-aggregating-new-elements-existing-ones-map-set/) about `map_aggregator` and `set_aggregator`.
