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