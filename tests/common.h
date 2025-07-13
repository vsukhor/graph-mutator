/* =============================================================================

Copyright (c) 2021-2025 Valerii Sukhorukov <vsukhorukov@yahoo.com>
All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

================================================================================
*/

#ifndef GRAPH_MUTATOR_TESTS_COMMON_H
#define GRAPH_MUTATOR_TESTS_COMMON_H

#include "gtest/gtest.h"

#include "graph-mutator/definitions.h"

namespace graph_mutator::tests {

using namespace std::string_literals;


class Test
    : public testing::Test {

protected:

    static constexpr bool withMaxVerbosity {true};
    static constexpr const char* tagBefore {"TEST_BEFORE"};
    static constexpr const char* tagAfter {"TEST_AFTER"};

    static unsigned int testCount;


    void SetUp() override
    {
        msgr = new Msgr {&std::cout, nullptr, 6};
    }

    void TearDown() override
    {
        if (msgr)
            delete msgr;
    }

    template<typename...Args>
    void print_description(Args... args)
    {
        log_('\n',
             colorcodes::GREEN, testCount, " : ",
             colorcodes::YELLOW, args..., colorcodes::RESET,
             '\n');
    }

};

inline unsigned int Test::testCount {};

}  // namespace graph_mutator::tests

#endif  // GRAPH_MUTATOR_TESTS_COMMON_H