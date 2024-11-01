#include "catch.hpp"
#include "pipes/pipes.hpp"

#include <algorithm>
#include <vector>

TEST_CASE("to_vector")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = input;

	REQUIRE(pipes::detail::IsAReturningPipeline< pipes::to_<std::vector<int>> >{true});

    auto results=(input>>=pipes::to_<std::vector<int>>());
    
    REQUIRE(results == expected);
}

TEST_CASE("filter_to_vector")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = {4, 8};

	REQUIRE(pipes::detail::IsAReturningPipeline< pipes::to_<std::vector<int>> >{true});

    auto results=(input
        >>=pipes::filter([](int i){return i%4 == 0;})
		>>=pipes::to_<std::vector<int>>()
	);
    
    REQUIRE(results == expected);
}
