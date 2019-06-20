#include "catch.hpp"
#include "filter.hpp"
#include "funnel.hpp"
#include "tee.hpp"
#include "transform.hpp"

TEST_CASE("tee outuputs to the next pipe as well as the one it takes in argument")
{
    auto const inputs = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto const expectedIntermediaryResults = std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    auto const expectedResults = std::vector<int>{12, 14, 16, 18, 20};
    
    auto intermediaryResults = std::vector<int>{};
    auto results = std::vector<int>{};
    
    inputs >>= pipes::funnel
    >>= pipes::transform([](int i){ return i * 2; })
    >>= pipes::tee(back_inserter(intermediaryResults))
    >>= pipes::filter([](int i){ return i > 10; })
    >>= back_inserter(results);
    
    REQUIRE(results == expectedResults);
    REQUIRE(intermediaryResults == expectedIntermediaryResults);
}

TEST_CASE("tee operator=")
{
    std::vector<int> results1, results2, results3, results4;
    
    auto tee1 = pipes::tee(back_inserter(results1)) >>= back_inserter(results2);
    auto tee2 = pipes::tee(back_inserter(results3)) >>= back_inserter(results4);
    
    tee2 = tee1;
    pipes::send(tee2, 0);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
