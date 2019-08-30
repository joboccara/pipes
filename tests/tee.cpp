#include "catch.hpp"
#include "pipes/filter.hpp"
#include "pipes/tee.hpp"
#include "pipes/transform.hpp"
#include "pipes/push_back.hpp"

TEST_CASE("tee outuputs to the next pipe as well as the one it takes in argument")
{
    auto const inputs = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto const expectedIntermediaryResults = std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    auto const expectedResults = std::vector<int>{12, 14, 16, 18, 20};
    
    auto intermediaryResults = std::vector<int>{};
    auto results = std::vector<int>{};
    
    inputs >>= pipes::transform([](int i){ return i * 2; })
           >>= pipes::tee(pipes::push_back(intermediaryResults))
           >>= pipes::filter([](int i){ return i > 10; })
           >>= pipes::push_back(results);
    
    REQUIRE(results == expectedResults);
    REQUIRE(intermediaryResults == expectedIntermediaryResults);
}

TEST_CASE("tee operator=")
{
    std::vector<int> results1, results2, results3, results4;
    
    auto tee1 = pipes::tee(pipes::push_back(results1)) >>= pipes::push_back(results2);
    auto tee2 = pipes::tee(pipes::push_back(results3)) >>= pipes::push_back(results4);
    
    tee2 = tee1;
    pipes::send(0, tee2);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
