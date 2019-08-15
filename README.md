 <p><img src="https://github.com/joboccara/pipes/blob/readme/docs/cpp_pipes.png"/></p>
 
<a href="https://www.patreon.com/join/fluentcpp?"><img alt="become a patron" src="https://c5.patreon.com/external/logo/become_a_patron_button.png" height="35px"></a>

Pipes are small components for writing expressive code when working on collections. Pipes chain together into a pipeline that receives data from a source, operates on that data, and send the results to a destination.

This is a header-only library, implemented in C++14.

# Contents

* [A First Example](#a-first-example)
* [A Second Example](#a-second-example)
* [Doesn't it look like ranges?](#doesn-t-it-look-like-ranges-)
* [End pipes](#end-pipes)
* [Easy integration with STL algorithms](#easy-integration-with-stl-algorithms)
* [List of available pipes](#list-of-available-pipes)

# A First Example

Here is a simple example of a pipeline made of two pipes: `transform` and `filter`:

```cpp
auto const source = std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
auto destination = std::vector<int>{};

source >>= pipes::funnel
       >>= pipes::filter([](int i){ return i % 2 == 0; }
       >>= pipes::transform([](int i){ return i * 2; })
       >>= std::back_inserter(destination);

// destination contains {0, 4, 8, 12, 16};
```
### What's going on here:
* `funnel` traverses the `source` and sends each elements to the next pipe (here, `filter`).
* Every time `filter` receives a piece of data, it sends its to the next pipe (here, `transform`) only if that piece of data satisfies `filter`'s' predicate.
* `transform` then applies its function on the data its gets and sends the result to the next pipe (here, `std::back_inserter`).
* `std::back_inserter` is a standard component that `push_back`s the data it receives to its `vector` (here, `destination`).

# A Second Example

Here is a more elaborate example with a pipeline that branches out in several directions:

```cpp
A >>= pipes::funnel
  >>= pipes::transform(f)
  >>= pipes::filter(p)
  >>= pipes::unzip(back_inserter(B),
                   pipes::demux(back_inserter(C),
                                pipes::filter(q) >>= back_inserter(D),
                                pipes::filter(r) >>= back_inserter(E));
```

Here, `unzip` takes the `std::pair`s or `std::tuple`s it receives and breaks them down into individual elements. It sends each element to the pipes it takes (here `back_inserter` and `demux`).

`demux` takes any number of pipes and sends the data it receives to each of them.

Since data circulates through pipes, real life pipes and plumbing provide a nice analogy (which gave its names to the library). For example, the above pipeline can be graphically represented like this:
 
 <p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/pipeline.png"/></p>

# Doesn't it look like ranges?

Pipes sort of look like [ranges](https://github.com/ericniebler/range-v3) adaptors from afar, but those two libraries have very different designs.

Range views are about adapting ranges with view layers, and reading through those layers in lazy mode.
Pipes are about sending pieces of data as they come along in a collection through a pipeline, and let them land in a destination.

Ranges and pipes have overlapping components such as `transform` and `filter`. But pipes do things like ranges can't do, such as `pipes::demux` and ranges do things that pipes can't do, like `ranges::view::zip`.

It is possible to use ranges and pipes in the same expression though:

```cpp
ranges::view::zip(dadChromosome, momChromosome)
    >>= pipes::funnel
    >>= pipes::transform(crossover)
    >>= pipes::unzip(back_inserter(gameteChromosome1),
                     back_inserter(gameteChromosome2));
```

# End pipes

This library also provides end pipes, which are components that send data to a collection in an elaborate way. For example, the `map_aggregate`  pipe receives `std::pair<Key, Value>`s and adds them to a map with the following rule:
* if its key is not already in the map, insert the incoming pair in the map,
* otherwise, aggregate the value of the incoming pair with the existing one in the map.

Example:

```cpp
std::map<int, std::string> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
std::map<int, std::string> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
std::map<int, std::string> results;

// results is empty

entries >>= pipes::funnel
        >>= pipes::map_aggregator(results, concatenateStrings);

// the elements of entries have been inserted into results

entries2 >>= pipes::funnel
         >>= pipes::map_aggregator(results, concatenateStrings);

// the new elements of entries2 have been inserter into results, the existing ones have been concatenated with the new values 
// results contains { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} }
```

All components are located in the namespace `pipes`.

# Easy integration with STL algorithms

All pipes can be used as output iterators of STL algorithms:

```cpp
std::set_difference(begin(setA), end(setA),
                    begin(setB), end(setB),
                    transform(f) >>= filter(p) >>= map_aggregator(results, addValues));
```

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/pipes-STL-algos.png"/></p>

# List of available pipes

* [General pipes](#general-pipes)
    * [`demux`](#-demux-)
    * [`dev_null`](#-dev-null-)
    * [`filter`](#-filter-)
    * [`partition`](#-partition-)
    * [`switch`](#-switch-)
    * [`tee`](#-tee-)
    * [`transform`](#-transform-)
    * [`unzip`](#-unzip-)
* [End pipes](#end-pipes-1)
    * [`custom`](#-custom-)
    * [`map_aggregator`](#-map-aggregator-)
    * [`set_aggregator`](#-set-aggregator-)
    * [`sorted_inserter`](#-sorted-inserter-)

## General pipes

### `demux`

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/demux_pipe.png"/></p>

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

### `dev_null`

`dev_null` is a pipe that doesn't do anything with the value it receives. It is useful for selecting only some data coming out of an algorithm that has several outputs.
An example of such algorithm is [`set_segregate`](https://github.com/joboccara/sets):

```cpp
std::set<int> setA = {1, 2, 3, 4, 5};
std::set<int> setB = {3, 4, 5, 6, 7};

std::vector<int> inAOnly;
std::vector<int> inBoth;

sets::set_seggregate(setA, setB,
                     back_inserter(inAOnly),
                     back_inserter(inBoth),
                     dev_null{});

// inAOnly contains {1, 2}
// inBoth contains {3, 4, 5}
```

### `filter`

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/filter_pipe.png"/></p>

`filter` is a pipe that takes a predicate `p` and, when it receives a value `x`, sends the result on to the next pipe iif `p(x)` is `true`.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> results;

input >>= pipes::funnel
      >>= pipes::filter([](int i){ return i % 2 == 0; })
      >>= back_inserter(results);

// results contains {2, 4, 6, 8, 10}
```

### `partition`

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/partition_pipe.png"/></p>

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

### `switch`

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

### `tee`

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/tee_pipe.png"/></p>

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

### `transform`

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/transform_pipe.png"/></p>

`transform` is a pipe that takes a function `f` and, when it receives a value, applies `f` on it and sends the result on to the next pipe.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5};
std::vector<int> results;

input >>= pipes::funnel
      >>= pipes::transform([](int i) { return i*2; })
      >>= back_inserter(results);

// results contains {2, 4, 6, 8, 10}
```

### `unzip`

<p align="center"><img src="https://github.com/joboccara/pipes/blob/readme/docs/unzip_pipe.png"/></p>

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

## End pipes

### `custom`

`custom` takes a function (or function object) that sends to the data it receives to that function. One of its usages is to give legacy code that does not use STL containers access to STL algorithms:

```cpp
std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};

void legacyInsert(int number, DarkLegacyStructure const& thing); // this function inserts into the old non-STL container

DarkLegacyStructure legacyStructure = // ...

std::copy(begin(input), end(input), custom([&legacyStructure](int number){ legacyInsert(number, legacyStructure); });
```

Read the [full story](https://www.fluentcpp.com/2017/11/24/how-to-use-the-stl-in-legacy-code/) about making legacy code compatible with the STL.

Note that `custom` goes along with a helper function object, `do_`, that allows to perfom several actions sequentially on the output of the algorithm:

```cpp
std::copy(begin(input), end(input), pipes::custom(pipes::do_([&](int i){ results1.push_back(i*2);}).
                                                         then_([&](int i){ results2.push_back(i+1);}).
                                                         then_([&](int i){ results3.push_back(-i);})));

```

### `map_aggregator`

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

### `set_aggregator`

Like `map_aggregator`, but inserting/aggregating into `std::set`s. Since `std::set` values are `const`, this pipe erases the element and re-inserts the aggregated value into the `std::set`.

```cpp
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
    return { value1.i, value1.s + value2.s };
}

int main()
{
    std::vector<Value> entries = { Value{1, "a"}, Value{2, "b"}, Value{3, "c"}, Value{4, "d"} };
    std::vector<Value> entries2 = { Value{2, "b"}, Value{3, "c"}, Value{4, "d"}, Value{5, "e"} };
    std::set<Value> results;

    std::copy(entries.begin(), entries.end(), pipes::set_aggregator(results, concatenateValues));
    std::copy(entries2.begin(), entries2.end(), pipes::set_aggregator(results, concatenateValues));

    // results contain { Value{1, "a"}, Value{2, "bb"}, Value{3, "cc"}, Value{4, "dd"}, Value{5, "e"} }
}
```

### `sorted_inserter`

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
