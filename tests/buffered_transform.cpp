#include "catch.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/buffered_transform.hpp"
#include "pipes/push_back.hpp"
#include "pipes/override.hpp"

#include <algorithm>
#include <memory>
#include <vector>

TEST_CASE("buffered_transform")
{
    auto add = 1;
    std::vector<int> input = { 1, 2, 3, 4, 5 };
    std::vector<int> expected = { add + 2, add + 4, add + 6, add + 8, add + 10 };
    std::vector<int> results;

    SECTION("input from range")
    {
        input >>= pipes::buffered_transform(add, [](int i, int buf) { return buf + i * 2; }) >>= pipes::push_back(results);
        REQUIRE(results == expected);
    }

    SECTION("input from STL algorithm")
    {
        std::copy(begin(input), end(input), pipes::buffered_transform(add, [](int i, int buf) { return buf + i * 2; }) >>= pipes::push_back(results));
        REQUIRE(results == expected);
    }
}
TEST_CASE("buffered_transform can use pointer to member functions")
{
    struct S
    {
        int get_42(int i) const { return i + 41; }
    };

    auto const input = std::vector<S>(10);
    auto const expected = std::vector<int>(10, 42);
    std::vector<int> results;

    input >>= pipes::buffered_transform(1, &S::get_42)
        >>= pipes::push_back(results);

    REQUIRE(expected == results);
}

TEST_CASE("buffered_transform's iterator category should be std::output_iterator_tag")
{
    auto const times2 = pipes::buffered_transform(1, [](int i, int buf) { return buf + i * 2; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(times2 >>= pipes::push_back(output))::iterator_category,
        std::output_iterator_tag>::value,
        "iterator category should be std::output_iterator_tag");
}

TEST_CASE("buffered_transform can override existing contents")
{
    auto add = 1;
    std::vector<int> input = { 1, 2, 3, 4, 5 };
    std::vector<int> expected = { add + 2, add + 4, add + 6, add + 8, add + 10 };

    auto const times2 = pipes::buffered_transform(add, [](int i, int buf) { return i * 2 + buf; });

    std::vector<int> results = { 0, 0, 0, 0, 0 };
    std::copy(begin(input), end(input), times2 >>= pipes::override(results));

    REQUIRE(results == expected);
}

TEST_CASE("buffered_transform operator=")
{
    std::vector<int> results1, results2;
    auto func = [](int i, int buf) { return buf + i * 2; };
    auto pipeline1 = pipes::buffered_transform(1, func) >>= pipes::push_back(results1);
    auto pipeline2 = pipes::buffered_transform(1, func) >>= pipes::push_back(results2);

    pipeline2 = pipeline1;
    send(1, pipeline2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}

TEST_CASE("buffered_transform move_only types")
{
    // Can't use initializer list since it needs to copy
    std::vector<std::unique_ptr<int>> input;
    input.push_back(std::make_unique<int>(1));
    input.push_back(std::make_unique<int>(2));

    std::vector<std::unique_ptr<int>> result;

    std::move(input) >>= pipes::buffered_transform(1, [](auto&& ptr, int) -> decltype(auto) { return std::move(ptr); })
        >>= pipes::push_back(result);

    // unique_ptr op == compares ptr not value
    REQUIRE(result.size() == 2);
    REQUIRE(*result[0] == 1);
    REQUIRE(*result[1] == 2);

    // input elements were moved from
    REQUIRE(input.size() == 2);
    REQUIRE(input[0] == nullptr);
    REQUIRE(input[1] == nullptr);
}

TEST_CASE("buffered_transform move into buffer from r-value")
{
    struct Buf
    {
        bool Moved{}, Copied{}, MovedFrom{};
        Buf() = default;
        Buf(const Buf&) noexcept : Copied(true)
        {
        }
        Buf(Buf&& b) noexcept: Moved(true)
        {
            b.MovedFrom = true;
        }
    };

    std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = { 2, 4, 6, 8, 10 };

    std::vector<int> result;

    Buf b;

    std::move(input) >>= pipes::buffered_transform(std::move(b), [&](int ele, Buf& buf) mutable -> decltype(auto)
        {
            REQUIRE(buf.MovedFrom == false);
            REQUIRE(buf.Copied == false);
            REQUIRE(buf.Moved == true);
            return ele * 2;
        })
        >>= pipes::push_back(result);

    REQUIRE(b.MovedFrom == true);
    REQUIRE(b.Copied == false);
    REQUIRE(b.Moved == false);
}