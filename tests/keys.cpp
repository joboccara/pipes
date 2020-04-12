/* Copyright (C) 2020 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "pipes/keys.hpp"
#include "catch.hpp"
#include "pipes/push_back.hpp"
#include "pipes/tee.hpp"
#include "pipes/transform.hpp"

#include <iostream>
#include <map>
#include <string>

TEST_CASE("keys sends the keys of a map into the next pipe")
{
    auto const inputs = std::map<int, std::string>{{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}};

    auto const expectedIntermediaryResults = std::vector<int>{1, 2, 3, 4};
    auto const expectedResults             = std::vector<std::string>{"1", "2", "3", "4"};

    auto intermediaryResults = std::vector<int>{};
    auto results             = std::vector<std::string>{};

    inputs >>= pipes::keys{} >>= pipes::tee(pipes::push_back(intermediaryResults))
        >>= pipes::transform([](auto&& val) { return std::to_string(val); }) >>= pipes::push_back(results);

    REQUIRE(intermediaryResults == expectedIntermediaryResults);
    REQUIRE(results == expectedResults);
}
