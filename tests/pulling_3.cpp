#include "pulling.h"


namespace graph_mutator::tests::pulling::d3 {

using Pull_3 = Test;


// =============================================================================
// ===== Linear Linear Linear ==================================================
// =============================================================================

/// Tests degree 3 pulling a single linear chain connected to two linear chains.
/// Driver is at end A, source edge is at end B of the same chain.
/// Applies single step, so that the source chain survives.
TEST_F(Pull_3, DrLiA_SrLiB_SameCnDS_Survive)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a single linear chain connected to two ",
            "linear chains.\n",
            "The path driver is at chain end A.\n",
            "Source edge is at end B of the same chain.\n",
            "Applies single step, so that the source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 2};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w1;  // the source belongs to the same chain as the driver
    constexpr auto eS = Ends::opp(eD);
    const auto v1 = w0;
    const auto v2 = w2;
    const auto v3 = w3;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {1};
    pu3f(pp, n);  // Pull single step to preserve source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 1);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v3].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].length(), n);
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[wD].length() - n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[v2].g[0].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[v1].g[0].ind, gr0.cn[v1].g[0].ind);
    ASSERT_EQ(gr.cn[v1].g[1].ind, gr0.cn[v1].g[1].ind);
    ASSERT_EQ(gr.cn[v3].g[0].ind, gr0.cn[v2].g[0].ind);
    ASSERT_EQ(gr.cn[v3].g[1].ind, gr0.cn[v2].g[1].ind);
    ASSERT_EQ(gr.cn[v3].g[2].ind, gr0.cn[v2].g[2].ind);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
}  // end DrLiA_SrLiB_SameCnDS_Survive

/* DrLiA_SrLiB_SameCnDS_Survive

TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 4 w 2 c 0
            [1] > ind 3 indc 5 w 2 c 0
            [2] > ind 4 indc 6 w 2 c 0

driver (egEnd A) 0:   [0] > ind 5 indc 0 w 1 c 0
source (end B)   1:   [1] > ind 6 indc 1 w 1 c 0

TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [0]  len 1 > ( 5 )
           [0] > ind 5 indc 0 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [1]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 1 w 0 c 0
           [1] > ind 1 indc 2 w 0 c 0
TEST_AFTER 2 {0 B} {1 B} {3 A}  **  0 [2]  len 1 > ( 6 )
           [0] > ind 6 indc 3 w 2 c 0
TEST_AFTER 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 4 w 3 c 0
           [1] > ind 3 indc 5 w 3 c 0
           [2] > ind 4 indc 6 w 3 c 0

*/


/// Tests degree 3 pulling a single linear chain connected to two linear chains.
/// Driver is at end A, source edge is at end B of the same chain.
/// Applies enough steps to engulf the whole source chain.
TEST_F(Pull_3, DrLiA_SrLiB_SameCnDS_Consume)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a single linear chain connected to two ",
            "linear chains.\n",
            "Driver is at end A, source edge is at end B of the same chain.\n",
            "Applies enough steps to engulf the whole source chain."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 2};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});  // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w1;  // the source belongs to the same chain as the driver
    constexpr auto eS = Ends::opp(eD);
    const auto v1 = w0;
    const auto v2 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {2};
    pu3f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length());
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[v1].g[0].ind, gr0.cn[v1].g[0].ind);
    ASSERT_EQ(gr.cn[v1].g[1].ind, gr0.cn[v1].g[1].ind);
    ASSERT_EQ(gr.cn[v2].g[0].ind, gr0.cn[v2].g[0].ind);
    ASSERT_EQ(gr.cn[v2].g[1].ind, gr0.cn[v2].g[1].ind);
    ASSERT_EQ(gr.cn[v2].g[2].ind, gr0.cn[v2].g[2].ind);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_EQ(gr.cn[wD].ngs[eD].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[eS].num(), 2);
    ASSERT_EQ(gr.cn[wD].ngs[eS][0].w, gr0.cn[wD].ngs[eD][0].w);
    ASSERT_EQ(gr.cn[wD].ngs[eS][1].w, gr0.cn[wD].ngs[eD][1].w);
    ASSERT_EQ(gr.cn[wD].ngs[eS][0].e, gr0.cn[wD].ngs[eD][0].e);
    ASSERT_EQ(gr.cn[wD].ngs[eS][1].e, gr0.cn[wD].ngs[eD][1].e);

}  // DrLiA_SrLiB_SameCnDS_Consume

/* DrLiA_SrLiB_SameCnDS_Consume

TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 4 w 2 c 0
            [1] > ind 3 indc 5 w 2 c 0
            [2] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 3+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 5 indc 0 w 1 c 0
source (end B)   1:   [1] > ind 6 indc 1 w 1 c 0

TEST_AFTER 1  ** {0 B} {2 A}  0 [0]  len 2 > > ( 5 6 )
           [0] > ind 5 indc 0 w 1 c 0
           [1] > ind 6 indc 1 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A}  0 [1]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 2 w 0 c 0
           [1] > ind 1 indc 3 w 0 c 0
TEST_AFTER 2 {0 B} {1 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 4 w 2 c 0
           [1] > ind 3 indc 5 w 2 c 0
           [2] > ind 4 indc 6 w 2 c 0
*/


/// Tests degree 3 pulling a single linear chain connected to two linear chains.
/// Driver is at end B, source edge is at end A of the same chain.
/// Applies single step, so that the source chain survives.
TEST_F(Pull_3, DrLiB_SrLiA_SameCnDS_Survive)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a single linear chain connected to two ",
            "linear chains.\n",
            "Driver is at end B, source edge is at end A of the same chain.\n",
            "Applies single step, so that the source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 2};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eB;
    const auto wS = w0;  // the source belongs to the same chain as the driver
    constexpr auto eS = Ends::opp(eD);
    const auto v1 = w1;
    const auto v2 = w2;
    const auto v3 = w3;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {1};
    pu3f(pp, n);  // Pull single step to preserve source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 1);
    ASSERT_EQ(gr.cn[wD].length(), n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[wD].length() - n);
    ASSERT_EQ(gr.cn[v3].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[v1].g[0].ind, gr0.cn[v1].g[1].ind);
    ASSERT_EQ(gr.cn[v1].g[1].ind, gr0.cn[v1].g[0].ind);
    ASSERT_EQ(gr.cn[v2].g[0].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[v3].g[0].ind, gr0.cn[v2].g[0].ind);
    ASSERT_EQ(gr.cn[v3].g[1].ind, gr0.cn[v2].g[1].ind);
    ASSERT_EQ(gr.cn[v3].g[2].ind, gr0.cn[v2].g[2].ind);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
}  // DrLiB_SrLiA_SameCnDS_Survive

/*  DrLiB_SrLiA_SameCnDS_Survive

TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 4 w 2 c 0
            [1] > ind 3 indc 5 w 2 c 0
            [2] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 3+ :: 1 step over path:

driver (egEnd B) 0:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   1:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [1]  len 2 < < ( 6 5 )
           [0] < ind 6 indc 1 w 1 c 0
           [1] < ind 5 indc 2 w 1 c 0
TEST_AFTER 2 {1 B} {0 B} {3 A}  **  0 [2]  len 1 > ( 1 )
           [0] > ind 1 indc 3 w 2 c 0
TEST_AFTER 3 {1 B} {2 A} {0 B}  **  0 [3]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 4 w 3 c 0
           [1] > ind 3 indc 5 w 3 c 0
           [2] > ind 4 indc 6 w 3 c 0
*/


/// Tests degree 3 pulling a single linear chain connected to two linear chains.
/// Driver is at end B, source edge is at end A of the same chain.
/// Applies enough steps to engulf the whole source chain.
TEST_F(Pull_3, DrLiB_SrLiA_SameCnDS_Consume)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a single linear chain connected to two ",
            "linear chains.\n",
            "Driver is at end B, source edge is at end A of the same chain.\n",
            "Applies enough steps to engulf the whole source chain."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 2};
    const auto [w0, w1, w2, w3, w4] = create_array<ChId, 5>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eB;
    const auto wS = w0;  // the source belongs to the same chain as the driver
    constexpr auto eS = Ends::opp(eD);
    const auto v1 = w1;
    const auto v2 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {2};
    pu3f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[wD].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[v2].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[v2].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[v2].g[2].ind);
    ASSERT_EQ(gr.cn[v1].g[0].ind, gr0.cn[v1].g[1].ind);
    ASSERT_EQ(gr.cn[v1].g[1].ind, gr0.cn[v1].g[0].ind);
    ASSERT_EQ(gr.cn[v2].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[v2].g[1].ind, gr0.cn[wD].g[1].ind);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    const auto v = gr.ct[ic].num_chains() - 1;
    ASSERT_EQ(gr.cn[v].ngs[eD].num(), 0);
    ASSERT_EQ(gr.cn[v].ngs[eS].num(), 2);
    ASSERT_EQ(gr.cn[v].ngs[eS][0].w, gr0.cn[wD].ngs[eD][0].w);
    ASSERT_EQ(gr.cn[v].ngs[eS][1].w, wD);
    ASSERT_EQ(gr.cn[v].ngs[eS][0].e, Ends::opp(gr0.cn[wD].ngs[eD][0].e));
    ASSERT_EQ(gr.cn[v].ngs[eS][1].e, gr0.cn[wD].ngs[eD][1].e);

}  // end DrLiB_SrLiA_SameCnDS_Consume

/* DrLiB_SrLiA_SameCnDS_Consume

TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 4 w 2 c 0
            [1] > ind 3 indc 5 w 2 c 0
            [2] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 3+ :: 2 steps over path:

driver (egEnd B) 0:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   1:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 1  ** {2 A} {0 A}  0 [0]  len 2 < < ( 6 5 )
           [0] < ind 6 indc 0 w 1 c 0
           [1] < ind 5 indc 1 w 1 c 0
TEST_AFTER 2 {1 B} {0 A}  **  0 [1]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 2 w 2 c 0
           [1] > ind 1 indc 3 w 2 c 0
TEST_AFTER 0 {1 B} {2 A}  **  0 [2]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 4 w 0 c 0
           [1] > ind 3 indc 5 w 0 c 0
           [2] > ind 4 indc 6 w 0 c 0
*/


// =============================================================================
// ===== Linear Cycle Out ======================================================
// =============================================================================

/// Tests degree 3 pulling a linear chain connected to a cycle chain.
/// Driver is at end A of the linear chain, source is at its end B.
/// Path does not include the cycle chain.
/// Applies single step, so that the source chain survives.
TEST_F(Pull_3, DrLiA_SrLiB_OutCyc_SameCnDS_Survive)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a linear chain connected to a cycle chain.\n",
            "Driver is at end A of the linear chain, source is at its end B.\n",
            "Path does not include the cycle chain.\n",
            "Applies single step, so that the source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 3});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = Ends::opp(eD);
    const auto v1 = w0;
    const auto v2 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {1};
    pu3f(pp, n);  // Pull single step to preserve source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 1);
    ASSERT_EQ(gr.cn[wD].length(), n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[wD].length() - n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[v1].g[0].ind, gr0.cn[v1].g[0].ind);
    ASSERT_EQ(gr.cn[v1].g[1].ind, gr0.cn[v1].g[1].ind);
    ASSERT_EQ(gr.cn[v1].g[2].ind, gr0.cn[v1].g[2].ind);
    ASSERT_EQ(gr.cn[v2].g[0].ind, gr0.cn[wD].g[1].ind);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
}  // end DrLiA_SrLiB_OutCyc_SameCnDS_Survive

/*  DrLiA_SrLiB_OutCyc_SameCnDS_Survive

TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 3 w 1 c 0
            [1] > ind 4 indc 4 w 1 c 0

Pulling from Vertex Deg 3+ :: 1 step over path:

driver (egEnd A) 0:   [0] > ind 3 indc 3 w 1 c 0
source (end B)   1:   [1] > ind 4 indc 4 w 1 c 0

TEST_AFTER 1  ** {0 B} {0 A} {2 A}  0 [0]  len 1 > ( 3 )
           [0] > ind 3 indc 0 w 1 c 0
TEST_AFTER 0 {0 B} {1 B} {2 A}  ** {1 B} {0 A} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
           [0] > ind 0 indc 1 w 0 c 0
           [1] > ind 1 indc 2 w 0 c 0
           [2] > ind 2 indc 3 w 0 c 0
TEST_AFTER 2 {0 B} {0 A} {1 B}  **  0 [2]  len 1 > ( 4 )
           [0] > ind 4 indc 4 w 2 c 0
*/


/// Tests degree 3 pulling a linear chain connected to a cycle chain.
/// Driver is at end A of the linear chain, source is at its end B.
/// Path does not include the cycle chain.
/// Applies enough steps to engulf the whole source chain.
TEST_F(Pull_3, DrLiA_SrLiB_OutCyc_SameCnDS_Consume)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a linear chain connected to a cycle chain.\n",
            "Driver is at end A of the linear chain, source is at its end B.\n",
            "Path does not include the cycle chain.\n",
            "Applies enough steps to engulf the whole source chain."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 3});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w1;  // the same chain for the driver and the source
    constexpr auto eS = Ends::opp(eD);
    const auto v = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {2};
    pu3f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length());
    ASSERT_EQ(gr.cn[v].length(), gr0.cn[v].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[v].g[0].ind, gr0.cn[v].g[0].ind);
    ASSERT_EQ(gr.cn[v].g[1].ind, gr0.cn[v].g[1].ind);
    ASSERT_EQ(gr.cn[v].g[2].ind, gr0.cn[v].g[2].ind);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_EQ(gr.cn[wD].ngs[eD].num(), 0);
    ASSERT_EQ(gr.cn[wS].ngs[eS].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eS][0].w, gr0.cn[wD].ngs[eD][0].w);
    ASSERT_EQ(gr.cn[wS].ngs[eS][1].w, gr0.cn[wD].ngs[eD][1].w);
    ASSERT_EQ(gr.cn[wS].ngs[eS][0].e, Ends::opp(gr0.cn[wD].ngs[eD][0].e));
    ASSERT_EQ(gr.cn[wS].ngs[eS][1].e, Ends::opp(gr0.cn[wD].ngs[eD][1].e));
    ASSERT_EQ(gr.cn[v].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[v].ngs[eA][0].w, v);
    ASSERT_EQ(gr.cn[v].ngs[eA][1].w, wS);
    ASSERT_EQ(gr.cn[v].ngs[eA][0].e, eB);
    ASSERT_EQ(gr.cn[v].ngs[eA][1].e, eS);
    ASSERT_EQ(gr.cn[v].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[v].ngs[eB][0].w, wS);
    ASSERT_EQ(gr.cn[v].ngs[eB][1].w, v);
    ASSERT_EQ(gr.cn[v].ngs[eB][0].e, eS);
    ASSERT_EQ(gr.cn[v].ngs[eB][1].e, eA);

}  // end DrLiA_SrLiB_OutCyc_SameCnDS_Consume

/*  DrLiA_SrLiB_OutCyc_SameCnDS_Consume

TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 3 w 1 c 0
            [1] > ind 4 indc 4 w 1 c 0

Pulling from Vertex Deg 3+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 3 indc 3 w 1 c 0
source (end B)   1:   [1] > ind 4 indc 4 w 1 c 0

TEST_AFTER 1  ** {0 B} {0 A}  0 [0]  len 2 > > ( 3 4 )
           [0] > ind 3 indc 0 w 1 c 0
           [1] > ind 4 indc 1 w 1 c 0
TEST_AFTER 0 {0 B} {1 B}  ** {1 B} {0 A}  0 [1]  len 3 > > > ( 0 1 2 )
           [0] > ind 0 indc 2 w 0 c 0
           [1] > ind 1 indc 3 w 0 c 0
           [2] > ind 2 indc 4 w 0 c 0
*/


/// Tests degree 3 pulling a linear chain connected to a cycle chain.
/// Driver is at end B of the linear chain, source is at its end A.
/// Path does not include the cycle chain.
/// Applies single step, so that the source chain survives.
TEST_F(Pull_3, DrLiB_SrLiA_OutCyc_SameCnDS_Survive)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a linear chain connected to a cycle chain.\n",
            "Driver is at end B of the linear chain, source is at its end A.\n",
            "Path does not include the cycle chain.\n",
            "Applies single step, so that the source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    // a linear chain w0 connected at end B to a cycle chain w1
    merge12(ESlot{w0, eB},
            BSlot{w0, 2});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eB;
    const auto wS = w0;  // the same chain for the driver and the source
    constexpr auto eS = Ends::opp(eD);
    const auto v1 = w1;
    const auto v2 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {1};
    pu3f(pp, n);  // Pull single step to preserve source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), n);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
}  // end DrLiB_SrLiA_OutCyc_SameCnDS_Survive

/* DrLiB_SrLiA_OutCyc_SameCnDS_Survive

TEST_BEFORE 0  ** {1 A} {1 B}  0 [0]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
TEST_BEFORE 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 2 w 1 c 0
            [1] > ind 3 indc 3 w 1 c 0
            [2] > ind 4 indc 4 w 1 c 0

Pulling from Vertex Deg 3+ :: 1 step over path:

driver (egEnd B) 0:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   1:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0  ** {1 B} {1 A} {2 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 1 {1 B} {0 B} {2 A}  ** {0 B} {1 A} {2 A}  0 [1]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 1 w 1 c 0
           [1] > ind 3 indc 2 w 1 c 0
           [2] > ind 4 indc 3 w 1 c 0
TEST_AFTER 2 {1 B} {1 A} {0 B}  **  0 [2]  len 1 > ( 1 )
           [0] > ind 1 indc 4 w 2 c 0
*/


/// Tests degree 3 pulling a linear chain connected a cycle chain.\n
/// source and driver edges belong to the same chain;
/// Applies enough steps to engulf the whole source chain.
TEST_F(Pull_3, eB_OutCyc_Consume)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a linear chain connected a cycle chain.\n",
            "source and driver edges belong to the same chain.\n",
            "Applies enough steps to engulf the whole source chain."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    // a linear chain w0 connected at end B to a cycle chain w1
    merge12(ESlot{w0, eB},
            BSlot{w0, 2});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    constexpr auto eS = Ends::opp(eD);
    const auto w = w0;  // the same chain for the driver and the source

    const Driver drv {&gr.cn[w].end_edge(eD), eD};
    const Source src {w, Ends::opp(eD)};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {2};
    pu3f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_EQ(gr.cn[w0].ngs[eD].num(), 0);
    ASSERT_EQ(gr.cn[w0].ngs[eS].num(), 2);
    ASSERT_EQ(gr.cn[w0].ngs[eS][0].w, w1);
    ASSERT_EQ(gr.cn[w0].ngs[eS][1].w, w1);
    ASSERT_EQ(gr.cn[w0].ngs[eS][0].e, eB);
    ASSERT_EQ(gr.cn[w0].ngs[eS][1].e, eA);
}


// =============================================================================
// ===== Cycle Linear In =======================================================
// =============================================================================

/// Tests degree 3 pulling a cycle chain connected a linear chain.
/// Driver is at end A of the cycle, source is at end B of the linear chain.
/// Applies single step, so that the source chain survives.
TEST_F(Pull_3, DrCyA_SrLiB_Survive)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a cycle chain connected a linear chain.\n",
            "Driver is at end A of the cycle, source is at end B of the linear ",
            "chain.\nApplies single step, so that the source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 3});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eB;
    const auto v = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {1};
    pu3f(pp, n);  // Pull single step.


    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 1);
    ASSERT_EQ(gr.cn[wD].length(), n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v].length(), gr0.cn[wD].length());
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_TRUE(gr.cn[v].is_connected_cycle());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[v].g[0].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[v].g[1].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[v].g[3].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].ngs[eD].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)].num(), 3);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][0].w, v);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][0].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][1].w, v);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][1].e, eB);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][2].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][2].e, Ends::opp(eS));
    ASSERT_EQ(gr.cn[wS].ngs[eS].num(), 0);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)].num(), 3);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][0].w, v);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][0].e, eA);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][1].w, v);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][1].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][2].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][2].e, Ends::opp(eD));

}  // DrCyA_SrLiB_Survive

/* DrCyA_SrLiB_Survive

TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 3 w 1 c 0
            [1] > ind 4 indc 4 w 1 c 0

Pulling from Vertex Deg 3+ :: 1 step over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [0] > ind 3 indc 3 w 1 c 0
source (end B)   4:   [1] > ind 4 indc 4 w 1 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 3 > > > ( 1 2 3 )
           [0] > ind 1 indc 1 w 2 c 0
           [1] > ind 2 indc 2 w 2 c 0
           [2] > ind 3 indc 3 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 1 > ( 4 )
           [0] > ind 4 indc 4 w 1 c 0
*/


/// Tests degree 3 pulling a cycle chain connected a linear chain.
/// Driver is at end A of the cycle, source is at end B of the linear chain.
/// Applies enough steps to engulf the whole source chain.
TEST_F(Pull_3, DrCyA_SrLiB_Consume)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a cycle chain connected a linear chain.\n",
            "Driver is at end A of the cycle, source is at end B of the linear ",
            "chain.\nApplies enough steps to engulf the whole source chain."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 3});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {2};
    pu3f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wS].length());
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wD].length());
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_TRUE(gr.cn[wS].is_connected_cycle());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].ngs[eD].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)].num(), 2);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][0].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][0].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][1].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[Ends::opp(eD)][1].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][0].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][1].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[Ends::opp(eS)][1].e, eS);
    ASSERT_EQ(gr.cn[wS].ngs[eS].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eS][0].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eS][0].e, Ends::opp(eS));
    ASSERT_EQ(gr.cn[wS].ngs[eS][1].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eS][1].e, Ends::opp(eD));

}  // end DrCyA_SrLiB_Consume

/* DrCyA_SrLiB_Consume

TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 3 w 1 c 0
            [1] > ind 4 indc 4 w 1 c 0

Pulling from Vertex Deg 3+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [0] > ind 3 indc 3 w 1 c 0
source (end B)   4:   [1] > ind 4 indc 4 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {1 A} {0 B}  0 [1]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 2 w 1 c 0
           [1] > ind 3 indc 3 w 1 c 0
           [2] > ind 4 indc 4 w 1 c 0
*/


/// Tests degree 3 pulling a cycle chain connected a linear chain.
/// Driver is at end B of the cycle, source is at end B of the linear chain.
/// Applies single step, so that the source chain survives.
TEST_F(Pull_3, DrCyB_SrLiB_Survive)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 3 pulling a cycle chain connected a linear chain.\n",
            "Driver is at end B of the cycle, source is at end B of the linear ",
            "chain.\nApplies single step, so that the source chain survives."
    );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 3});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eB;
    constexpr auto eS = eB;
    const auto wS = w1;
    const auto v = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<3, Orientation::Forwards, G> pu3f {gr};

    constexpr int n {1};
    pu3f(pp, n);  // Pull single step to preserve source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 1);
    ASSERT_EQ(gr.cn[wD].length(), n);
    ASSERT_EQ(gr.cn[wS].length(), 1);
    ASSERT_EQ(gr.cn[v].length(), 3);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_TRUE(gr.cn[v].is_connected_cycle());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g.back().ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[1].ind);

}  // end DrCyB_SrLiB_Survive

/* DrCyB_SrLiB_Survive

TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 3 w 1 c 0
            [1] > ind 4 indc 4 w 1 c 0

Pulling from Vertex Deg 3+ :: 1 step over path:

driver (egEnd B) 0:   [2] > ind 2 indc 2 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [0] > ind 0 indc 0 w 0 c 0
                 3:   [0] > ind 3 indc 3 w 1 c 0
source (end B)   4:   [1] > ind 4 indc 4 w 1 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 1 < ( 2 )
           [0] < ind 2 indc 0 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 3 < < > ( 1 0 3 )
           [0] < ind 1 indc 1 w 2 c 0
           [1] < ind 0 indc 2 w 2 c 0
           [2] > ind 3 indc 3 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 1 > ( 4 )
           [0] > ind 4 indc 4 w 1 c 0
*/


}  // namespace graph_mutator::tests::pulling::d3