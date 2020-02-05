#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

std::string toUpper(std::string const& s)
{
    std::string result(s.size(), '_');
    std::transform(begin(s), end(s), begin(result), [](char c){ return static_cast<char>(std::toupper(c)); });
    return result;
}

TEST_CASE("streaming strings in")
{
    auto const input = std::string{"word1 word2 word3"};
    auto const expected = std::vector<std::string>{"WORD1", "WORD2", "WORD3"};
    auto results = std::vector<std::string>{};
    
    std::istringstream(input) >>= pipes::read_in_stream<std::string>{}
                              >>= pipes::transform(toUpper)
                              >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("streaming doubles in")
{
    auto const input = std::string{"1.1 2.2 3.3"};
    auto const expected = std::vector<double>{11, 22, 33};
    auto results = std::vector<double>{};
    
    std::istringstream(input) >>= pipes::read_in_stream<double>{}
                              >>= pipes::transform([](double d){ return d * 10; })
                              >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("empty stream in")
{
    auto const input = std::string{};
    auto const expected = std::vector<std::string>{};
    auto results = std::vector<std::string>{};
    
    std::istringstream(input) >>= pipes::read_in_stream<std::string>{}
                              >>= pipes::transform(toUpper)
                              >>= pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("streaming out")
{
    auto const input = std::vector<std::string>{"word1", "word2", "word3"};
    auto const expected = std::string{"WORD1WORD2WORD3"};
    auto resultStream = std::ostringstream{};
    
    input >>= pipes::transform(toUpper)
          >>= pipes::to_out_stream(resultStream);
    
    auto const results = resultStream.str();
    
    REQUIRE(results == expected);
}
