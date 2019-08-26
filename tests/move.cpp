#include "catch.hpp"

#include "pipes/pipes.hpp"

TEST_CASE("push back move only types")
{
    auto input = std::vector<std::unique_ptr<int>>{};
    input.push_back(std::make_unique<int>(0));
    input.push_back(std::make_unique<int>(1));
    input.push_back(std::make_unique<int>(2));
    
    auto results = std::vector<std::unique_ptr<int>>{};
    
    input >>= pipes::move
          >>= pipes::push_back(results);
    
    REQUIRE(input.size() == 3);
    REQUIRE(input[0] == nullptr);
    REQUIRE(input[1] == nullptr);
    REQUIRE(input[2] == nullptr);
    
    REQUIRE(results.size() == 3);
    REQUIRE(*results[0] == 0);
    REQUIRE(*results[1] == 1);
    REQUIRE(*results[2] == 2);
}

TEST_CASE("transform move only types")
{
    auto input = std::vector<std::unique_ptr<int>>{};
    input.push_back(std::make_unique<int>(0));
    input.push_back(std::make_unique<int>(1));
    input.push_back(std::make_unique<int>(2));
    
    auto results = std::vector<std::unique_ptr<int>>{};
    
    input >>= pipes::move
          >>= pipes::transform([](std::unique_ptr<int> pointer){ return pointer; })
          >>= pipes::push_back(results);
    
    REQUIRE(input.size() == 3);
    REQUIRE(input[0] == nullptr);
    REQUIRE(input[1] == nullptr);
    REQUIRE(input[2] == nullptr);
    
    REQUIRE(results.size() == 3);
    REQUIRE(*results[0] == 0);
    REQUIRE(*results[1] == 1);
    REQUIRE(*results[2] == 2);
}

