#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <string>
#include <vector>

TEST_CASE("intersperse inserts a delimiter between the consecutive elements coming its way")
{
    auto const input = std::string{"abcdef"};
    auto const expected = std::string{"a,b,c,d,e,f"};
    
    auto result = std::string{};
    
    input >>= pipes::intersperse(',')
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("intersperse doesn't insert a delimiter if there is only one input")
{
    auto const input = std::string{"a"};
    auto const expected = std::string{"a"};
    
    auto result = std::string{};
    
    input >>= pipes::intersperse(',')
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("intersperse doesn't insert anything if there is no input")
{
    auto const input = std::string{};
    auto const expected = std::string{};
    
    auto result = std::string{};
    
    input >>= pipes::intersperse(',')
    >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("intersperse can read from another pipe")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{2, 99, 4, 99, 6, 99, 8, 99, 10};
    
    auto result = std::vector<int>{};
    
    input >>= pipes::filter([](int i){ return i % 2 == 0;})
          >>= pipes::intersperse(99)
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("intersperse can read from an STL algorithm")
{
    auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const expected = std::vector<int>{2, 99, 4, 99, 6, 99, 8, 99, 10};

    auto result = std::vector<int>{};

    std::copy_if(begin(input), end(input),
                 pipes::intersperse(99) >>= pipes::push_back(result),
                 [](int i){ return i % 2 == 0;});
    
    REQUIRE(result == expected);
}

struct ToString
{
    std::string const& operator()(std::string const& string) { return string; }
    
    template<typename T>
    std::string operator()(T&& value){ return std::to_string(value); }
};

TEST_CASE("intersperse can have a delimiter of another type than the data it receives")
{
    auto const input = std::vector<std::string>{"word1", "word2", "word3"};
    auto const expected = std::vector<std::string>{"word1", "99", "word2", "99", "word3"};
    
    auto result = std::vector<std::string>{};
    
    input >>= pipes::intersperse(99)
          >>= pipes::transform(ToString{})
          >>= pipes::push_back(result);
    
    REQUIRE(result == expected);
}

TEST_CASE("intersperse operator=")
{
    std::string result1, result2;
    
    auto const expected1 = std::string{"a,b,c"};
    auto const expected2 = std::string{};
    
    auto pipeline1 = pipes::intersperse(',') >>= pipes::push_back(result1);
    auto pipeline2 = pipes::intersperse(',') >>= pipes::push_back(result2);
    
    pipeline2 = pipeline1;
    
    send('a', pipeline2);
    send('b', pipeline2);
    send('c', pipeline2);
    
    REQUIRE(result1 == expected1);
    REQUIRE(result2 == expected2);
}
