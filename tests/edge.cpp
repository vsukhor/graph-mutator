#include "gtest/gtest.h"

#include "graph-mutator/definitions.h"

#include "graph-mutator/structure/edge.h"
#include "graph-mutator/structure/ends.h"


namespace graph_mutator::tests::edge {


using Edge = structure::Edge<structure::vertices::maxDegree>;
using Ends = Edge::Ends;

TEST(EdgeTest, Constructor)
{
    Edge e {3, 4, 5, 6, 7};

    ASSERT_EQ(3, e.ind);
    ASSERT_EQ(4, e.indc);
    ASSERT_EQ(5, e.indw);
    ASSERT_EQ(6, e.w);
    ASSERT_EQ(7, e.c);
}


TEST(EdgeTest, Reflect)
{
    Edge e {3, 4, 5, 6, 7};

    ASSERT_EQ(e.orientation(), 1);

    e.reverse();

    ASSERT_EQ(e.orientation(), -1);
}


TEST(EdgeTest, OppEnd)
{
    Edge e {3, 4, 5, 6, 7};

    ASSERT_EQ(e.template opp_end<Ends::A>(), Ends::B);
    ASSERT_EQ(e.template opp_end<Ends::B>(), Ends::A);
    ASSERT_EQ(e.opp_end(Ends::A), Ends::B);
    ASSERT_EQ(e.opp_end(Ends::B), Ends::A);
}


TEST(EdgeTest, SetCmp)
{
    Edge e {3, 4, 5, 6, 7};

    e.set_cmp(70, 40);

    ASSERT_EQ(3, e.ind);
    ASSERT_EQ(40, e.indc);
    ASSERT_EQ(5, e.indw);
    ASSERT_EQ(6, e.w);
    ASSERT_EQ(70, e.c);
}


}  // namespace graph_mutator::tests::edge
