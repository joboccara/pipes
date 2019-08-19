#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

std::string toUpper(std::string const& s)
{
    std::string result(s.size(), '_');
    std::transform(begin(s), end(s), begin(result), [](char c){ return std::toupper(c); });
    return result;
}

TEST_CASE("streaming in")
{
    auto const input = std::string{"word1 word2 word3"};
    auto const expected = std::vector<std::string>{"WORD1", "WORD2", "WORD3"};
    auto results = std::vector<std::string>{};
    
    std::istringstream(input) >>= pipes::transform(toUpper)
                              >>= back_inserter(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("empty stream in")
{
    auto const input = std::string{};
    auto const expected = std::vector<std::string>{};
    auto results = std::vector<std::string>{};
    
    std::istringstream(input) >>= pipes::transform(toUpper)
                              >>= back_inserter(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("streaming out")
{
    auto const input = std::vector<std::string>{"word1", "word2", "word3"};
    auto const expected = std::string{"WORD1WORD2WORD3"};
    auto resultStream = std::ostringstream{};
    
    input >>= pipes::transform(toUpper)
          >>= std::ostream_iterator<std::string>(resultStream);
    
    auto const results = resultStream.str();
    
    REQUIRE(results == expected);
}
