#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>

#include "pulling.h"


namespace graph_mutator::tests::pulling::d1 {

using Pull_1 = Test;

// =============================================================================
// ===== Single Chain ==========================================================
// =============================================================================


/// Tests pulling degree 1 of a compartment consisting of a single linear chain.
/// The chain is driven at its end A. This is a trivial case of a standalone
/// chain where no change is induced.
TEST_F(Pull_1, SingleLinChain_DrA)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests pulling degree 1 of a compartment consisting of a single ",
            "linear chain.\nThe chain is driven at its end A. \nThis is a ",
            "trivial case of a standalone chain where no change is induced."
        );

    // Create initial graph.

    constexpr EgId len {5};
    constexpr ChId w {0};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w].tail(), eA};
    constexpr Source src {w, eB};
    Path pp {&gr.ct[gr.cn[w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};
    constexpr int n {2};
    pu1f(pp, n);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    ASSERT_EQ(gr.cn[w], gr0.cn[w]);
    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w].length(), len);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
    ASSERT_EQ(gr.cn[w].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w].ngs[eB].num(), 0);

}  //  end SingleLinChain_DrA


/// Tests pulling degree 1 of a compartment consisting of a single linear chain.
/// The path is driven at the chain end B.
/// This is a trivial case of a standalone chain where no change is induced.
TEST_F(Pull_1, SingleLinChain_DrB)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests pulling degree 1 of a compartment consisting of a single ",
            "linear chain. \nThe path is driven at chain end B. \nThis is ",
            "a trivial case of a standalone chain where no change is induced."
        );

    // Create initial graph.

    constexpr EgId len {5};
    constexpr ChId w {0};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w].head(), eB};
    constexpr Source src {w, eA};
    Path pp {&gr.ct[gr.cn[w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};
    constexpr int n {2};
    pu1f(pp, n);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    ASSERT_EQ(gr.cn[w], gr0.cn[w]);
    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w].length(), len);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
    ASSERT_EQ(gr.cn[w].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w].ngs[eB].num(), 0);

}  //  end SingleLinChain_DrB


// =============================================================================
// ===== Vertex Degree 3: Linear Source ========================================
// =============================================================================

/// Tests degree 1 pulling over a path traversing a 3-way junction.
/// The path is driven at its end A.
/// The source chain is linear, its free end A survives.
TEST_F(Pull_1, J3_DrA_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path is driven at end A.\n",
            "The source chain is linear, its free end B survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (5) -> (2, 3)
    merge12(ESlot{w1, eB},
            BSlot{w0, 2});     // splits w0 to create w2

    const auto gr0 = gr;  // copy the graph to save its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;
    const auto v1 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J3_DrA_SrLiA_Srv

/*  J3_DrA_SrLiA_Srv

TEST_BEFORE 1  ** {0 B} {2 A}  0 [0]  len 3 > > > ( 5 6 7 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
            [2] > ind 7 indc 2 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
TEST_BEFORE 2 {1 B} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 5 w 2 c 0
            [1] > ind 3 indc 6 w 2 c 0
            [2] > ind 4 indc 7 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 7 indc 2 w 1 c 0
                 3:   [1] > ind 6 indc 1 w 1 c 0
source (end A)   4:   [0] > ind 5 indc 0 w 1 c 0

TEST_AFTER 1  ** {0 B} {2 A}  0 [0]  len 1 > ( 5 )
           [0] > ind 5 indc 0 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A}  0 [1]  len 4 > > > > ( 0 1 7 6 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
           [2] > ind 7 indc 2 w 0 c 0
           [3] > ind 6 indc 1 w 0 c 0
TEST_AFTER 2 {1 B} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
           [0] > ind 2 indc 5 w 2 c 0
           [1] > ind 3 indc 6 w 2 c 0
           [2] > ind 4 indc 7 w 2 c 0
*/

/// Tests degree 1 pulling over a path traversing a 3-way junction.
/// The path is driven at its end A.
/// The source chain is linear, its free end B survives.
TEST_F(Pull_1, J3_DrA_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path is driven at end A.\n",
            "The source chain is linear, its free end B survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 1};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // splits w0 to create w2

    const auto gr0 = gr;  // copy the graph to save its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto v1 = w1;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& eg: gr.cn[wD].g) {
        ASSERT_EQ(eg.indw, i++);
        ASSERT_EQ(eg.w, wD);
        ASSERT_EQ(eg.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J3_DrA_SrLiB_Srv

/*  J3_DrA_SrLiB_Srv

TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 1 > ( 5 )
            [0] > ind 5 indc 0 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 1 w 0 c 0
            [1] > ind 1 indc 2 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 3 w 2 c 0
            [1] > ind 3 indc 4 w 2 c 0
            [2] > ind 4 indc 5 w 2 c 0

driver (egEnd A) 0:   [0] > ind 0 indc 1 w 0 c 0
                 1:   [1] > ind 1 indc 2 w 0 c 0
                 2:   [0] > ind 2 indc 3 w 2 c 0
                 3:   [1] > ind 3 indc 4 w 2 c 0
source (end B)   4:   [2] > ind 4 indc 5 w 2 c 0

TEST_AFTER 1 {0 B} {2 A}  **  0 [0]  len 1 > ( 5 )
           [0] > ind 5 indc 0 w 1 c 0
TEST_AFTER 0  ** {1 A} {2 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
           [0] > ind 0 indc 1 w 0 c 0
           [1] > ind 1 indc 2 w 0 c 0
           [2] > ind 2 indc 3 w 0 c 0
           [3] > ind 3 indc 4 w 0 c 0
TEST_AFTER 2 {1 A} {0 B}  **  0 [2]  len 1 > ( 4 )
           [0] > ind 4 indc 5 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 3-way junction.
/// The path driver is at chain end B.
/// The source chain is linear, its free end A survives.
TEST_F(Pull_1, J3_DrB_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path driver is at chain end B.\n",
            "The source chain is linear, its free end A survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 1};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 3});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w0;
    constexpr auto eS = eA;
    const auto v1 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& eg: gr.cn[wD].g) {
        ASSERT_EQ(eg.indw, i++);
        ASSERT_EQ(eg.w, wD);
        ASSERT_EQ(eg.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J3_DrB_SrLiA_Srv

/*  J3_DrB_SrLiA_Srv

TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 1 > ( 5 )
            [0] > ind 5 indc 0 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 1 w 0 c 0
            [1] > ind 1 indc 2 w 0 c 0
            [2] > ind 2 indc 3 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 4 w 2 c 0
            [1] > ind 4 indc 5 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [2] > ind 2 indc 3 w 0 c 0
                 2:   [1] > ind 1 indc 2 w 0 c 0
source (end A)   3:   [0] > ind 0 indc 1 w 0 c 0

TEST_AFTER 1 {0 B} {2 A}  **  0 [0]  len 3 > > > ( 1 2 5 )
           [0] > ind 1 indc 2 w 1 c 0
           [1] > ind 2 indc 3 w 1 c 0
           [2] > ind 5 indc 0 w 1 c 0
TEST_AFTER 0  ** {1 A} {2 A}  0 [1]  len 1 > ( 0 )
           [0] > ind 0 indc 1 w 0 c 0
TEST_AFTER 2 {1 A} {0 B}  **  0 [2]  len 2 > > ( 3 4 )
           [0] > ind 3 indc 4 w 2 c 0
           [1] > ind 4 indc 5 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 3-way junction.
/// The path driver is at chain end B.
/// The source chain is linear, its free end B survives.
TEST_F(Pull_1, J3_DrB_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path driver is at chain end B.\n",
            "The source chain is linear, its free end B survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 1};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto v1 = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[0].indc, gr0.cn[wS].g[1].indc);
    ASSERT_EQ(gr.cn[wD].g[1].indc, gr0.cn[wS].g[0].indc);
    for (EgId i {}; const auto& eg: gr.cn[wD].g) {
        ASSERT_EQ(eg.indw, i++);
        ASSERT_EQ(eg.w, wD);
        ASSERT_EQ(eg.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J3_DrB_SrLiB_Srv

/*
TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 1 > ( 5 )
            [0] > ind 5 indc 0 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 1 w 0 c 0
            [1] > ind 1 indc 2 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 3 w 2 c 0
            [1] > ind 3 indc 4 w 2 c 0
            [2] > ind 4 indc 5 w 2 c 0

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [0] > ind 2 indc 3 w 2 c 0
                 2:   [1] > ind 3 indc 4 w 2 c 0
source (end B)   3:   [2] > ind 4 indc 5 w 2 c 0

TEST_AFTER 1 {0 B} {2 A}  **  0 [0]  len 3 > > > ( 3 2 5 )
           [0] > ind 3 indc 4 w 1 c 0
           [1] > ind 2 indc 3 w 1 c 0
           [2] > ind 5 indc 0 w 1 c 0
TEST_AFTER 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 1 w 0 c 0
           [1] > ind 1 indc 2 w 0 c 0
TEST_AFTER 2 {1 A} {0 B}  **  0 [2]  len 1 > ( 4 )
           [0] > ind 4 indc 5 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 3-way junction,
/// The path is driven at a chain end A.
/// The source chain is linear with free end A. It is engulfed completely.
TEST_F(Pull_1, J3_DrA_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction. ",
            "The path is driven at a chain end A.\n",
            "The source chain is linear with free end A. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {3, 2};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};

    merge12(ESlot{w1, eB},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w0;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct.size(), 1);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), 1);
    ASSERT_EQ(gr.cn[w0].length(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.cn[w0].idw, 0);
    ASSERT_EQ(gr.cn[w0].idc, 0);
    ASSERT_EQ(gr.cn[w0].c, 0);
    constexpr std::array<EgId, 5> inds {3, 4, 1, 0, 2};
    for (EgId i {}; const auto& eg: gr.cn[w0].g) {
        ASSERT_EQ(eg.ind, inds[i]);
        ASSERT_EQ(eg.indc, i);
        ASSERT_EQ(eg.indw, i);
        ASSERT_EQ(eg.c, 0);
        ASSERT_EQ(eg.w, w0);
        ++i;
    }

}  //  end J3_DrA_SrLiA_Rmv

/*
TEST_BEFORE 1  ** {0 B} {2 A}  0 [0]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 0 w 1 c 0
            [1] > ind 4 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
TEST_BEFORE 2 {1 B} {0 B}  **  0 [2]  len 1 > ( 2 )
            [0] > ind 2 indc 4 w 2 c 0

driver (egEnd A) 0:   [0] > ind 3 indc 0 w 1 c 0
                 1:   [1] > ind 4 indc 1 w 1 c 0
                 2:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   3:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 0  **  0 [0]  len 5 > > > > > ( 3 4 1 0 2 )
           [0] > ind 3 indc 0 w 0 c 0
           [1] > ind 4 indc 1 w 0 c 0
           [2] > ind 1 indc 2 w 0 c 0
           [3] > ind 0 indc 3 w 0 c 0
           [4] > ind 2 indc 4 w 0 c 0
*/


/// Tests degree 1 pulling over a path traversing a 3-way junction.
/// The path is driven at a chain end A.
/// The source chain is linear with free end B. It is engulfed completely.
TEST_F(Pull_1, J3_DrA_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path is driven at a chain end A.\n",
            "The source chain is linear with free end B. ",
            "It is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 1};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w2;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct.size(), 1);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), 1);
    ASSERT_EQ(gr.cn[w0].length(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.cn[w0].idw, 0);
    ASSERT_EQ(gr.cn[w0].idc, 0);
    ASSERT_EQ(gr.cn[w0].c, 0);
    constexpr std::array<EgId, 6> inds {0, 1, 2, 3, 4, 5};
    for (EgId i {}; const auto& eg: gr.cn[w0].g) {
        ASSERT_EQ(eg.ind, inds[i]);
        ASSERT_EQ(eg.indc, i);
        ASSERT_EQ(eg.indw, i);
        ASSERT_EQ(eg.c, 0);
        ASSERT_EQ(eg.w, w0);
        ++i;
    }

}  // end J3_DrA_SrLiB_Rmv

/*
TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 1 > ( 5 )
            [0] > ind 5 indc 0 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 1 w 0 c 0
            [1] > ind 1 indc 2 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 3 w 2 c 0
            [1] > ind 3 indc 4 w 2 c 0
            [2] > ind 4 indc 5 w 2 c 0

driver (egEnd A) 0:   [0] > ind 0 indc 1 w 0 c 0
                 1:   [1] > ind 1 indc 2 w 0 c 0
                 2:   [0] > ind 2 indc 3 w 2 c 0
                 3:   [1] > ind 3 indc 4 w 2 c 0
source (end B)   4:   [2] > ind 4 indc 5 w 2 c 0

TEST_AFTER 0  **  0 [0]  len 6 > > > > > > ( 0 1 2 3 4 5 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 3 indc 3 w 0 c 0
           [4] > ind 4 indc 4 w 0 c 0
           [5] > ind 5 indc 5 w 0 c 0
*/


/// Tests degree 1 pulling over a path traversing a 3-way junction.
/// The path is driven at a chain end B.
/// The source chain is linear with free end B. It is engulfed completely.
TEST_F(Pull_1, J3_DrB_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path is driven at a chain end B.\n",
            "The source chain is linear with free end B. ",
            "It is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 1};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (5) -> (2, 3)
    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct.size(), 1);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), 1);
    ASSERT_EQ(gr.cn[w0].length(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.cn[w0].idw, 0);
    ASSERT_EQ(gr.cn[w0].idc, 0);
    ASSERT_EQ(gr.cn[w0].c, 0);
    constexpr std::array<EgId, 6> inds {0, 1, 4, 3, 2, 5};
    for (EgId i {}; const auto& eg: gr.cn[w0].g) {
        ASSERT_EQ(eg.ind, inds[i]);
        ASSERT_EQ(eg.indc, i);
        ASSERT_EQ(eg.indw, i);
        ASSERT_EQ(eg.c, 0);
        ASSERT_EQ(eg.w, w0);
        ++i;
    }

}  //  end J3_DrB_SrLiB_Rmv

/*
TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 1 > ( 5 )
            [0] > ind 5 indc 0 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 1 w 0 c 0
            [1] > ind 1 indc 2 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 3 > > > ( 2 3 4 )
            [0] > ind 2 indc 3 w 2 c 0
            [1] > ind 3 indc 4 w 2 c 0
            [2] > ind 4 indc 5 w 2 c 0

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [0] > ind 2 indc 3 w 2 c 0
                 2:   [1] > ind 3 indc 4 w 2 c 0
source (end B)   3:   [2] > ind 4 indc 5 w 2 c 0

TEST_AFTER 0  **  0 [0]  len 6 > > > > > > ( 0 1 4 3 2 5 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 4 indc 2 w 0 c 0
           [3] > ind 3 indc 3 w 0 c 0
           [4] > ind 2 indc 4 w 0 c 0
           [5] > ind 5 indc 5 w 0 c 0
*/


/// Tests degree 1 pulling over a path traversing a 3-way junction,
/// The path is driven at a chain end B.
/// The source chain is linear with free end A. It is engulfed completely.
TEST_F(Pull_1, J3_DrB_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way junction.\n",
            "The path is driven at a chain end B.\n",
            "The source chain is linear with free end A. ",
            "It is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {3, 2};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};

    merge12(ESlot{w1, eA},
            BSlot{w0, 2});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w2;
    constexpr auto eD = eB;
    const auto wS = w0;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct.size(), 1);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), 1);
    ASSERT_EQ(gr.cn[w0].length(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.cn[w0].idw, 0);
    ASSERT_EQ(gr.cn[w0].idc, 0);
    ASSERT_EQ(gr.cn[w0].c, 0);
    constexpr std::array<EgId, 5> inds {4, 3, 0, 1, 2};
    for (EgId i {}; const auto& eg: gr.cn[w0].g) {
        ASSERT_EQ(eg.ind, inds[i]);
        ASSERT_EQ(eg.indc, i);
        ASSERT_EQ(eg.indw, i);
        ASSERT_EQ(eg.c, 0);
        ASSERT_EQ(eg.w, w0);
        ++i;
    }

}  //  end J3_DrB_SrLiA_Rmv

/*
TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 0 w 1 c 0
            [1] > ind 4 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 1 > ( 2 )
            [0] > ind 2 indc 4 w 2 c 0

driver (egEnd B) 0:   [0] > ind 2 indc 4 w 2 c 0
                 1:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   2:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 0  **  0 [0]  len 5 < < > > > ( 4 3 0 1 2 )
           [0] < ind 4 indc 0 w 0 c 0
           [1] < ind 3 indc 1 w 0 c 0
           [2] > ind 0 indc 2 w 0 c 0
           [3] > ind 1 indc 3 w 0 c 0
           [4] > ind 2 indc 4 w 0 c 0
*/


// =============================================================================
// ===== Vertex Degree 3: Cycle Source =========================================
// =============================================================================

/// Tests degree 1 pulling through a 3-way junction joining a linear and a
/// cycle chains. Driver edge is at the free end A of the linear chain while
/// source is at the end A of the cycle chain. Preserves the component topology.
TEST_F(Pull_1, J3_DrA_SrCyA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling through a 3-way junction joining a ",
            "linear and a cycle chains. ",
            "Driver edge is at the free end A of the linear chain while the ",
            "source is at the end A of the cycle chain. ",
            "Preserves the component topology."
        );

    // Create initial graph.

    constexpr EgId len {6};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // create a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eB},
            BSlot{w0, 2});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eA;
    const auto wD = w0;
    constexpr auto eS = eA;
    const auto wS = w1;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to get min length of the source cycle chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
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
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[wD].ngs[eB][0].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eB][0].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eB][1].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eB][1].e, eB);

}  //  J3_DrA_SrCyA_Srv

/*
TEST_BEFORE 0  ** {1 A} {1 B}  0 [0]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
TEST_BEFORE 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 4 > > > > ( 2 3 4 5 )
            [0] > ind 2 indc 2 w 1 c 0
            [1] > ind 3 indc 3 w 1 c 0
            [2] > ind 4 indc 4 w 1 c 0
            [3] > ind 5 indc 5 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:
driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [3] > ind 5 indc 5 w 1 c 0
                 3:   [2] > ind 4 indc 4 w 1 c 0
                 4:   [1] > ind 3 indc 3 w 1 c 0
source (end A)   5:   [0] > ind 2 indc 2 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 4 > > > > ( 0 1 5 4 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 5 indc 5 w 0 c 0
           [3] > ind 4 indc 4 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 2 > > ( 2 3 )
           [0] > ind 2 indc 2 w 1 c 0
           [1] > ind 3 indc 3 w 1 c 0
*/


/// Tests degree 1 pulling through a 3-way junction joining a linear and a
/// cycle chains. Driver edge is at the free end A of the linear chain while
/// source is at the end B of the cycle chain. Preserves the component topology.
TEST_F(Pull_1, J3_DrA_SrCyB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling through a 3-way junction joining a ",
            "linear and a cycle chains. ",
            "Driver edge is at the free end A of the linear chain while the ",
            "source is at the end B of the cycle chain. ",
            "Preserves the component topology."
        );

    // Create initial graph.

    constexpr EgId len {6};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // create a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eB},
            BSlot{w0, 2});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eA;
    const auto wD = w0;
    constexpr auto eS = eB;
    const auto wS = w1;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to get min length of the source cycle chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
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
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[3].ind);
    ASSERT_EQ(gr.cn[wS].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[wD].ngs[eB][0].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eB][0].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eB][1].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eB][1].e, eB);

}  //  J3_DrA_SrCyB_Srv

/*
TEST_BEFORE 0  ** {1 A} {1 B}  0 [0]  len 2 > > ( 0 1 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
TEST_BEFORE 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 4 > > > > ( 2 3 4 5 )
            [0] > ind 2 indc 2 w 1 c 0
            [1] > ind 3 indc 3 w 1 c 0
            [2] > ind 4 indc 4 w 1 c 0
            [3] > ind 5 indc 5 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:
driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [0] > ind 2 indc 2 w 1 c 0
                 3:   [1] > ind 3 indc 3 w 1 c 0
                 4:   [2] > ind 4 indc 4 w 1 c 0
source (end B)   5:   [3] > ind 5 indc 5 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 4 > > > > ( 0 1 2 3 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 3 indc 3 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 2 > > ( 4 5 )
           [0] > ind 4 indc 4 w 1 c 0
           [1] > ind 5 indc 5 w 1 c 0
*/

/// Tests degree 1 pulling through a 3-way junction joining a linear and a
/// cycle chains. Driver edge is at the free end B of the linear chain while
/// source is at the end A of the cycle chain. Preserves the component topology.
TEST_F(Pull_1, J3_DrB_SrCyA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling through a 3-way junction joining a ",
            "linear and a cycle chains. ",
            "Driver edge is at the free end B of the linear chain while the ",
            "source is at the end A of the cycle chain. ",
            "Preserves the component topology."
        );

    // Create initial graph.

    constexpr EgId len {6};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // create a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    const auto wD = w1;
    constexpr auto eS = eB;
    const auto wS = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to get min length of the source cycle chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
     for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_TRUE(gr.cn[wS].is_connected_cycle());
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wS].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].w, w1);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].e, eA);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].w, w0);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].e, eA);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].w, w1);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[wD].ngs[eB].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[eA][0].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eA][0].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eA][1].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eA][1].e, eB);

}  //  J3_DrB_SrCyA_Srv

/*
TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
            [3] > ind 3 indc 3 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 4 5 )
            [0] > ind 4 indc 4 w 1 c 0
            [1] > ind 5 indc 5 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [1] > ind 5 indc 5 w 1 c 0
                 1:   [0] > ind 4 indc 4 w 1 c 0
                 2:   [0] > ind 0 indc 0 w 0 c 0
                 3:   [1] > ind 1 indc 1 w 0 c 0
                 4:   [2] > ind 2 indc 2 w 0 c 0
source (end B)   5:   [3] > ind 3 indc 3 w 0 c 0

TEST_AFTER 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 2 > > ( 2 3 )
           [0] > ind 2 indc 2 w 0 c 0
           [1] > ind 3 indc 3 w 0 c 0
TEST_AFTER 1 {0 A} {0 B}  **  0 [1]  len 4 > > > > ( 1 0 4 5 )
           [0] > ind 1 indc 1 w 1 c 0
           [1] > ind 0 indc 0 w 1 c 0
           [2] > ind 4 indc 4 w 1 c 0
           [3] > ind 5 indc 5 w 1 c 0
*/


/// Tests degree 1 pulling through a 3-way junction joining a linear and a cycle
/// chains. Driver edge is at the free end B of the linear chain while source
/// is at the end B of the cycle chain. Preserves the component topology.
TEST_F(Pull_1, J3_DrB_SrCyB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling through a 3-way junction joining a linear",
            "and a cycle chains. ",
            "Driver edge is at the free end B of the linear chain while the ",
            "source is at the end B of the cycle chain. ",
            "Preserves the component topology."
        );

    // Create initial graph.

    constexpr EgId len {6};
    const auto [w0, w1] = create_array<ChId, 2>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0 : length (5) -> (2, 3)
    // create a linear chain w1 connected at end A to a cycle chain w0
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    const auto wD = w1;
    constexpr auto eS = eA;
    const auto wS = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wD].end_edge(Ends::opp(eD)), Ends::opp(eD)},
        Driver {&gr.cn[wS].end_edge(Ends::opp(eS)), Ends::opp(eS)}
    };
    Path pp {&gr.ct[gr.cn[wS].c],
             drv,
             src,
             internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to get min length of the source cycle chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
     for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        const auto& eg = gr.cn[h.w].g[h.a];
        ASSERT_EQ(eg.w, h.w);
        ASSERT_EQ(eg.indw, h.a);
        ASSERT_EQ(eg.ind, h.i);
        ASSERT_EQ(eg.c, ic);
        ASSERT_EQ(eg.indc, i++);
    }
    ASSERT_TRUE(gr.cn[wS].is_connected_cycle());
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wS].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eA][0].e, eB);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eA][1].e, eA);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].w, wS);
    ASSERT_EQ(gr.cn[wS].ngs[eB][0].e, eA);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].w, wD);
    ASSERT_EQ(gr.cn[wS].ngs[eB][1].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[wD].ngs[eB].num(), 0);
    ASSERT_EQ(gr.cn[wD].ngs[eA][0].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eA][0].e, eA);
    ASSERT_EQ(gr.cn[wD].ngs[eA][1].w, wS);
    ASSERT_EQ(gr.cn[wD].ngs[eA][1].e, eB);

}  //  end J3_DrB_SrCyB_Srv

/*
TEST_BEFORE 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
            [3] > ind 3 indc 3 w 0 c 0
TEST_BEFORE 1 {0 A} {0 B}  **  0 [1]  len 2 > > ( 4 5 )
            [0] > ind 4 indc 4 w 1 c 0
            [1] > ind 5 indc 5 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [1] > ind 5 indc 5 w 1 c 0
                 1:   [0] > ind 4 indc 4 w 1 c 0
                 2:   [3] > ind 3 indc 3 w 0 c 0
                 3:   [2] > ind 2 indc 2 w 0 c 0
                 4:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
TEST_AFTER 1 {0 A} {0 B}  **  0 [1]  len 4 > > > > ( 2 3 4 5 )
           [0] > ind 2 indc 2 w 1 c 0
           [1] > ind 3 indc 3 w 1 c 0
           [2] > ind 4 indc 4 w 1 c 0
           [3] > ind 5 indc 5 w 1 c 0
*/


// =============================================================================
// ===== Vertex Degree 4: Lin Lin Lin Lin ======================================
// =============================================================================

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end A,
/// source edge is at tne end A of a linear chain.
/// The source chain survives.
TEST_F(Pull_1, J4_LinLin_DrA_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end A, ",
            "source edge is at tne end A of a linear chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w3 : length (5) -> (2, 3)
    // w1, w2 : length (4) -> (1, 3)
    merge22(BSlot{w1, 3},
            BSlot{w0, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w0;
    constexpr auto eS = eA;
    const auto v1 = w2;
    const auto v2 = w3;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
    }

}  //  end J4_LinLin_DrA_SrLiA_Srv

/*  J4_LinLin_DrA_SrLiA_Srv

TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 0 w 1 c 0
            [1] > ind 7 indc 1 w 1 c 0
            [2] > ind 8 indc 2 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {1 B} {3 A}  **  0 [2]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 6 w 2 c 0
            [1] > ind 10 indc 7 w 2 c 0
            [2] > ind 11 indc 8 w 2 c 0
TEST_BEFORE 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 9 w 3 c 0
            [1] > ind 4 indc 10 w 3 c 0
            [2] > ind 5 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 6 indc 0 w 1 c 0
                 1:   [1] > ind 7 indc 1 w 1 c 0
                 2:   [2] > ind 8 indc 2 w 1 c 0
                 3:   [2] > ind 2 indc 5 w 0 c 0
                 4:   [1] > ind 1 indc 4 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 3 w 0 c 0

TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [0]  len 5 > > > > > ( 6 7 8 2 1 )
           [0] > ind 6 indc 0 w 1 c 0
           [1] > ind 7 indc 1 w 1 c 0
           [2] > ind 8 indc 2 w 1 c 0
           [3] > ind 2 indc 5 w 1 c 0
           [4] > ind 1 indc 4 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [1]  len 1 > ( 0 )
           [0] > ind 0 indc 3 w 0 c 0
TEST_AFTER 2 {0 B} {1 B} {3 A}  **  0 [2]  len 3 > > > ( 9 10 11 )
           [0] > ind 9 indc 6 w 2 c 0
           [1] > ind 10 indc 7 w 2 c 0
           [2] > ind 11 indc 8 w 2 c 0
TEST_AFTER 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 3 4 5 )
           [0] > ind 3 indc 9 w 3 c 0
           [1] > ind 4 indc 10 w 3 c 0
           [2] > ind 5 indc 11 w 3 c 0
*/

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end A,
/// source edge is at tne end B of a linear chain.
/// The source chain survives.
TEST_F(Pull_1, J4_LinLin_DrA_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end A, ",
            "source edge is at tne end B of a linear chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w3 : length (5) -> (2, 3)
    // w1, w2 : length (4) -> (1, 3)
    merge22(BSlot{w1, 3},
            BSlot{w0, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto v1 = w0;
    const auto v2 = w3;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
    }

}  //  end J4_LinLin_DrA_SrLiB_Srv

/*  J4_LinLin_DrA_SrLiB_Srv

TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 0 w 1 c 0
            [1] > ind 7 indc 1 w 1 c 0
            [2] > ind 8 indc 2 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {1 B} {3 A}  **  0 [2]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 6 w 2 c 0
            [1] > ind 10 indc 7 w 2 c 0
            [2] > ind 11 indc 8 w 2 c 0
TEST_BEFORE 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 9 w 3 c 0
            [1] > ind 4 indc 10 w 3 c 0
            [2] > ind 5 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 6 indc 0 w 1 c 0
                 1:   [1] > ind 7 indc 1 w 1 c 0
                 2:   [2] > ind 8 indc 2 w 1 c 0
                 3:   [0] > ind 9 indc 6 w 2 c 0
                 4:   [1] > ind 10 indc 7 w 2 c 0
source (end B)   5:   [2] > ind 11 indc 8 w 2 c 0

TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [0]  len 5 > > > > > ( 6 7 8 9 10 )
           [0] > ind 6 indc 0 w 1 c 0
           [1] > ind 7 indc 1 w 1 c 0
           [2] > ind 8 indc 2 w 1 c 0
           [3] > ind 9 indc 6 w 1 c 0
           [4] > ind 10 indc 7 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 0 1 2 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
           [2] > ind 2 indc 5 w 0 c 0
TEST_AFTER 2 {0 B} {1 B} {3 A}  **  0 [2]  len 1 > ( 11 )
           [0] > ind 11 indc 8 w 2 c 0
TEST_AFTER 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 3 4 5 )
           [0] > ind 3 indc 9 w 3 c 0
           [1] > ind 4 indc 10 w 3 c 0
           [2] > ind 5 indc 11 w 3 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end B,
/// source edge is at tne end B of a linear chain.
/// The source chain survives.
TEST_F(Pull_1, J4_LinLin_DrB_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end B, ",
            "source edge is at tne end B of a linear chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w2 : length (6) -> (3, 3)
    // w1, w3 : length (6) -> (3, 3)
    merge22(BSlot{w0, 3},
            BSlot{w1, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w2;
    constexpr auto eD = eB;
    const auto wS = w3;
    constexpr auto eS = eB;
    const auto v1 = w0;
    const auto v2 = w1;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
    }

}  //  end J4_LinLin_DrB_SrLiB_Srv

/*  J4_LinLin_DrB_SrLiB_Srv

TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 3 w 1 c 0
            [1] > ind 7 indc 4 w 1 c 0
            [2] > ind 8 indc 5 w 1 c 0
TEST_BEFORE 2 {1 B} {0 B} {3 A}  **  0 [2]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
TEST_BEFORE 3 {1 B} {2 A} {0 B}  **  0 [3]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 9 w 3 c 0
            [1] > ind 10 indc 10 w 3 c 0
            [2] > ind 11 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 5 indc 8 w 2 c 0
                 1:   [1] > ind 4 indc 7 w 2 c 0
                 2:   [0] > ind 3 indc 6 w 2 c 0
                 3:   [0] > ind 9 indc 9 w 3 c 0
                 4:   [1] > ind 10 indc 10 w 3 c 0
source (end B)   5:   [2] > ind 11 indc 11 w 3 c 0

TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 0 1 2 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 6 7 8 )
           [0] > ind 6 indc 3 w 1 c 0
           [1] > ind 7 indc 4 w 1 c 0
           [2] > ind 8 indc 5 w 1 c 0
TEST_AFTER 2 {1 B} {0 B} {3 A}  **  0 [2]  len 5 > > > > > ( 10 9 3 4 5 )
           [0] > ind 10 indc 10 w 2 c 0
           [1] > ind 9 indc 9 w 2 c 0
           [2] > ind 3 indc 6 w 2 c 0
           [3] > ind 4 indc 7 w 2 c 0
           [4] > ind 5 indc 8 w 2 c 0
TEST_AFTER 3 {1 B} {2 A} {0 B}  **  0 [3]  len 1 > ( 11 )
           [0] > ind 11 indc 11 w 3 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end B,
/// source edge is at tne end A of a linear chain.
/// The source chain survives.
TEST_F(Pull_1, J4_LinLin_DrB_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end B, ",
            "source edge is at tne end A of a linear chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w2 : length (6) -> (3, 3)
    // w1, w3 : length (6) -> (3, 3)
    merge22(BSlot{w0, 3},
            BSlot{w1, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w2;
    constexpr auto eD = eB;
    const auto wS = w0;
    constexpr auto eS = eA;
    const auto v1 = w1;
    const auto v2 = w3;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[v2].length(), gr0.cn[v2].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
    }

}  //  end J4_LinLin_DrB_SrLiA_Srv

/*  J4_LinLin_DrB_SrLiA_Srv

TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 3 w 1 c 0
            [1] > ind 7 indc 4 w 1 c 0
            [2] > ind 8 indc 5 w 1 c 0
TEST_BEFORE 2 {1 B} {0 B} {3 A}  **  0 [2]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
TEST_BEFORE 3 {1 B} {2 A} {0 B}  **  0 [3]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 9 w 3 c 0
            [1] > ind 10 indc 10 w 3 c 0
            [2] > ind 11 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 5 indc 8 w 2 c 0
                 1:   [1] > ind 4 indc 7 w 2 c 0
                 2:   [0] > ind 3 indc 6 w 2 c 0
                 3:   [2] > ind 2 indc 2 w 0 c 0
                 4:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 6 7 8 )
           [0] > ind 6 indc 3 w 1 c 0
           [1] > ind 7 indc 4 w 1 c 0
           [2] > ind 8 indc 5 w 1 c 0
TEST_AFTER 2 {1 B} {0 B} {3 A}  **  0 [2]  len 5 > > > > > ( 1 2 3 4 5 )
           [0] > ind 1 indc 1 w 2 c 0
           [1] > ind 2 indc 2 w 2 c 0
           [2] > ind 3 indc 6 w 2 c 0
           [3] > ind 4 indc 7 w 2 c 0
           [4] > ind 5 indc 8 w 2 c 0
TEST_AFTER 3 {1 B} {2 A} {0 B}  **  0 [3]  len 3 > > > ( 9 10 11 )
           [0] > ind 9 indc 9 w 3 c 0
           [1] > ind 10 indc 10 w 3 c 0
           [2] > ind 11 indc 11 w 3 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end A,
/// source edge is at tne end A of a linear chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_LinLin_DrA_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end A, ",
            "source edge is at tne end A of a linear chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w3 : length (6) -> (3, 3)
    // w1, w2 : length (6) -> (3, 3)
    merge22(BSlot{w1, 3},
            BSlot{w0, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w0;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wS].g[0].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[1].c, ic);
    }

}  //  end J4_LinLin_DrA_SrLiA_Rmv

/* J4_LinLin_DrA_SrLiA_Rmv

TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 0 w 1 c 0
            [1] > ind 7 indc 1 w 1 c 0
            [2] > ind 8 indc 2 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {1 B} {3 A}  **  0 [2]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 6 w 2 c 0
            [1] > ind 10 indc 7 w 2 c 0
            [2] > ind 11 indc 8 w 2 c 0
TEST_BEFORE 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 9 w 3 c 0
            [1] > ind 4 indc 10 w 3 c 0
            [2] > ind 5 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 6 indc 0 w 1 c 0
                 1:   [1] > ind 7 indc 1 w 1 c 0
                 2:   [2] > ind 8 indc 2 w 1 c 0
                 3:   [2] > ind 2 indc 5 w 0 c 0
                 4:   [1] > ind 1 indc 4 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 3 w 0 c 0

TEST_AFTER 1  ** {2 A} {0 A}  0 [0]  len 6 > > > > > > ( 6 7 8 2 1 0 )
           [0] > ind 6 indc 0 w 1 c 0
           [1] > ind 7 indc 1 w 1 c 0
           [2] > ind 8 indc 2 w 1 c 0
           [3] > ind 2 indc 3 w 1 c 0
           [4] > ind 1 indc 4 w 1 c 0
           [5] > ind 0 indc 5 w 1 c 0
TEST_AFTER 2 {1 B} {0 A}  **  0 [1]  len 3 > > > ( 9 10 11 )
           [0] > ind 9 indc 6 w 2 c 0
           [1] > ind 10 indc 7 w 2 c 0
           [2] > ind 11 indc 8 w 2 c 0
TEST_AFTER 0 {2 A} {1 B}  **  0 [2]  len 3 > > > ( 3 4 5 )
           [0] > ind 3 indc 9 w 0 c 0
           [1] > ind 4 indc 10 w 0 c 0
           [2] > ind 5 indc 11 w 0 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end A,
/// source edge is at tne end B of a linear chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_LinLin_DrA_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end A, ",
            "source edge is at tne end B of a linear chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w3 : length (6) -> (3, 3)
    // w1, w2 : length (6) -> (3, 3)
    merge22(BSlot{w1, 3},
            BSlot{w0, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w2;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wS].g[2].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[1].c, ic);
    }

}  //  end J4_LinLin_DrA_SrLiB_Rmv

/* J4_LinLin_DrA_SrLiB_Rmv

TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 0 w 1 c 0
            [1] > ind 7 indc 1 w 1 c 0
            [2] > ind 8 indc 2 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {1 B} {3 A}  **  0 [2]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 6 w 2 c 0
            [1] > ind 10 indc 7 w 2 c 0
            [2] > ind 11 indc 8 w 2 c 0
TEST_BEFORE 3 {0 B} {2 A} {1 B}  **  0 [3]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 9 w 3 c 0
            [1] > ind 4 indc 10 w 3 c 0
            [2] > ind 5 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 6 indc 0 w 1 c 0
                 1:   [1] > ind 7 indc 1 w 1 c 0
                 2:   [2] > ind 8 indc 2 w 1 c 0
                 3:   [0] > ind 9 indc 6 w 2 c 0
                 4:   [1] > ind 10 indc 7 w 2 c 0
source (end B)   5:   [2] > ind 11 indc 8 w 2 c 0

TEST_AFTER 1  ** {0 B} {2 A}  0 [0]  len 6 > > > > > > ( 6 7 8 9 10 11 )
           [0] > ind 6 indc 0 w 1 c 0
           [1] > ind 7 indc 1 w 1 c 0
           [2] > ind 8 indc 2 w 1 c 0
           [3] > ind 9 indc 3 w 1 c 0
           [4] > ind 10 indc 4 w 1 c 0
           [5] > ind 11 indc 5 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
           [0] > ind 0 indc 6 w 0 c 0
           [1] > ind 1 indc 7 w 0 c 0
           [2] > ind 2 indc 8 w 0 c 0
TEST_AFTER 2 {0 B} {1 B}  **  0 [2]  len 3 > > > ( 3 4 5 )
           [0] > ind 3 indc 9 w 2 c 0
           [1] > ind 4 indc 10 w 2 c 0
           [2] > ind 5 indc 11 w 2 c 0

*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end B,
/// source edge is at tne end B of a linear chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_LinLin_DrB_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end B, ",
            "source edge is at tne end B of a linear chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w2 : length (6) -> (3, 3)
    // w1, w3 : length (6) -> (3, 3)
    merge22(BSlot{w0, 3},
            BSlot{w1, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w2;
    constexpr auto eD = eB;
    const auto wS = w3;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
    }

}  //  end J4_LinLin_DrB_SrLiB_Rmv

/* J4_LinLin_DrB_SrLiB_Rmv

TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 3 w 1 c 0
            [1] > ind 7 indc 4 w 1 c 0
            [2] > ind 8 indc 5 w 1 c 0
TEST_BEFORE 2 {1 B} {0 B} {3 A}  **  0 [2]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
TEST_BEFORE 3 {1 B} {2 A} {0 B}  **  0 [3]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 9 w 3 c 0
            [1] > ind 10 indc 10 w 3 c 0
            [2] > ind 11 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd B) 0:   [2] > ind 5 indc 8 w 2 c 0
                 1:   [1] > ind 4 indc 7 w 2 c 0
                 2:   [0] > ind 3 indc 6 w 2 c 0
                 3:   [0] > ind 9 indc 9 w 3 c 0
                 4:   [1] > ind 10 indc 10 w 3 c 0
source (end B)   5:   [2] > ind 11 indc 11 w 3 c 0

TEST_AFTER 0  ** {1 B} {2 A}  0 [0]  len 3 > > > ( 0 1 2 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
TEST_AFTER 1  ** {0 B} {2 A}  0 [1]  len 3 > > > ( 6 7 8 )
           [0] > ind 6 indc 3 w 1 c 0
           [1] > ind 7 indc 4 w 1 c 0
           [2] > ind 8 indc 5 w 1 c 0
TEST_AFTER 2 {1 B} {0 B}  **  0 [2]  len 6 > > > > > > ( 11 10 9 3 4 5 )
           [0] > ind 11 indc 6 w 2 c 0
           [1] > ind 10 indc 7 w 2 c 0
           [2] > ind 9 indc 8 w 2 c 0
           [3] > ind 3 indc 9 w 2 c 0
           [4] > ind 4 indc 10 w 2 c 0
           [5] > ind 5 indc 11 w 2 c 0

*/
/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting four linear chains. Driving edge is at the end B,
/// source edge is at tne end A of a linear chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_LinLin_DrB_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting four linear chains. Driving edge is at the end B, ",
            "source edge is at tne end A of a linear chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {6, 6};
    const auto [w0, w1, w2, w3] = create_array<ChId, 4>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w2 : length (6) -> (3, 3)
    // w1, w3 : length (6) -> (3, 3)
    merge22(BSlot{w0, 3},
            BSlot{w1, 3});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w2;
    constexpr auto eD = eB;
    const auto wS = w0;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
    }

}  //  end J4_LinLin_DrB_SrLiA_Rmv

/* J4_LinLin_DrB_SrLiA_Rmv
TEST_BEFORE 0  ** {1 B} {2 A} {3 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 1  ** {0 B} {2 A} {3 A}  0 [1]  len 3 > > > ( 6 7 8 )
            [0] > ind 6 indc 3 w 1 c 0
            [1] > ind 7 indc 4 w 1 c 0
            [2] > ind 8 indc 5 w 1 c 0
TEST_BEFORE 2 {1 B} {0 B} {3 A}  **  0 [2]  len 3 > > > ( 3 4 5 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
TEST_BEFORE 3 {1 B} {2 A} {0 B}  **  0 [3]  len 3 > > > ( 9 10 11 )
            [0] > ind 9 indc 9 w 3 c 0
            [1] > ind 10 indc 10 w 3 c 0
            [2] > ind 11 indc 11 w 3 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd B) 0:   [2] > ind 5 indc 8 w 2 c 0
                 1:   [1] > ind 4 indc 7 w 2 c 0
                 2:   [0] > ind 3 indc 6 w 2 c 0
                 3:   [2] > ind 2 indc 2 w 0 c 0
                 4:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 1  ** {2 A} {0 A}  0 [0]  len 3 > > > ( 6 7 8 )
           [0] > ind 6 indc 0 w 1 c 0
           [1] > ind 7 indc 1 w 1 c 0
           [2] > ind 8 indc 2 w 1 c 0
TEST_AFTER 2 {1 B} {0 A}  **  0 [1]  len 6 > > > > > > ( 0 1 2 3 4 5 )
           [0] > ind 0 indc 3 w 2 c 0
           [1] > ind 1 indc 4 w 2 c 0
           [2] > ind 2 indc 5 w 2 c 0
           [3] > ind 3 indc 6 w 2 c 0
           [4] > ind 4 indc 7 w 2 c 0
           [5] > ind 5 indc 8 w 2 c 0
TEST_AFTER 0 {1 B} {2 A}  **  0 [2]  len 3 > > > ( 9 10 11 )
           [0] > ind 9 indc 9 w 0 c 0
           [1] > ind 10 indc 10 w 0 c 0
           [2] > ind 11 indc 11 w 0 c 0

*/

// =============================================================================
// ===== Vertex Degree 4: Lin OutCyc Lin =======================================
// =============================================================================

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end A of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// The source chain survives.
TEST_F(Pull_1, J4_DrA_OutCyc_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end A, source edge is at tne end A of a linear chain. ",
            "Pulled sequence omits the cycle chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eB},
            BSlot{w0, 3});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eB},
            ESlot{w0, eB});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;
    const auto v1 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrA_OutCyc_SrLiA_Srv

/*  J4_DrA_OutCyc_SrLiA_Srv

TEST_BEFORE 1  ** {0 B} {2 A} {2 B}  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0  ** {2 A} {2 B} {1 B}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B} 0 [2] len 4 >>>> (3 4 5 6)
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
            [3] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 2 indc 5 w 0 c 0
                 3:   [2] > ind 9 indc 2 w 1 c 0
                 4:   [1] > ind 8 indc 1 w 1 c 0
source (end A)   5:   [0] > ind 7 indc 0 w 1 c 0

TEST_AFTER 1  ** {0 B} {2 A} {2 B}  0 [0]  len 1 > ( 7 )
           [0] > ind 7 indc 0 w 1 c 0
TEST_AFTER 0  ** {2 A} {2 B} {1 B}  0 [1]  len 5 > > > > > ( 0 1 2 9 8 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
           [2] > ind 2 indc 5 w 0 c 0
           [3] > ind 9 indc 2 w 0 c 0
           [4] > ind 8 indc 1 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 6 w 2 c 0
           [1] > ind 4 indc 7 w 2 c 0
           [2] > ind 5 indc 8 w 2 c 0
           [3] > ind 6 indc 9 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end B of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// The source chain survives.
TEST_F(Pull_1, J4_DrA_OutCyc_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end A, source edge is at tne end B of a linear chain. ",
            "Pulled sequence omits the cycle chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eB;
    const auto v1 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrA_OutCyc_SrLiB_Srv

/*  J4_DrA_OutCyc_SrLiB_Srv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [0] > ind 7 indc 7 w 1 c 0
                 4:   [1] > ind 8 indc 8 w 1 c 0
source (end B)   5:   [2] > ind 9 indc 9 w 1 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 5 > > > > > ( 0 1 2 7 8 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 7 indc 7 w 0 c 0
           [4] > ind 8 indc 8 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 3 w 2 c 0
           [1] > ind 4 indc 4 w 2 c 0
           [2] > ind 5 indc 5 w 2 c 0
           [3] > ind 6 indc 6 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 1 > ( 9 )
           [0] > ind 9 indc 9 w 1 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end B of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// The source chain survives.
TEST_F(Pull_1, J4_DrB_OutCyc_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end B, source edge is at tne end B of a linear chain. ",
            "Pulled sequence omits the cycle chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eA},
            ESlot{w2, eA});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto v1 = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrB_OutCyc_SrLiB_Srv

/*  J4_DrB_OutCyc_SrLiB_Srv

TEST_BEFORE 1 {2 A} {0 A} {0 B}  **  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
            [3] > ind 3 indc 6 w 0 c 0
TEST_BEFORE 2 {0 A} {0 B} {1 A}  **  0 [2]  len 3 > > > ( 4 5 6 )
            [0] > ind 4 indc 7 w 2 c 0
            [1] > ind 5 indc 8 w 2 c 0
            [2] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 2 w 1 c 0
                 1:   [1] > ind 8 indc 1 w 1 c 0
                 2:   [0] > ind 7 indc 0 w 1 c 0
                 3:   [0] > ind 4 indc 7 w 2 c 0
                 4:   [1] > ind 5 indc 8 w 2 c 0
source (end B)   5:   [2] > ind 6 indc 9 w 2 c 0

TEST_AFTER 1 {2 A} {0 A} {0 B}  **  0 [0]  len 5 > > > > > ( 5 4 7 8 9 )
           [0] > ind 5 indc 8 w 1 c 0
           [1] > ind 4 indc 7 w 1 c 0
           [2] > ind 7 indc 0 w 1 c 0
           [3] > ind 8 indc 1 w 1 c 0
           [4] > ind 9 indc 2 w 1 c 0
TEST_AFTER 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
           [2] > ind 2 indc 5 w 0 c 0
           [3] > ind 3 indc 6 w 0 c 0
TEST_AFTER 2 {0 A} {0 B} {1 A}  **  0 [2]  len 1 > ( 6 )
           [0] > ind 6 indc 9 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end A of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// The source chain survives.
TEST_F(Pull_1, J4_DrB_OutCyc_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end B, source edge is at tne end A of a linear chain. ",
            "Pulled sequence omits the cycle chain. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w0;
    constexpr auto eS = eA;
    const auto v1 = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[v1].length(), gr0.cn[v1].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& eg: gr.cn[wD].g) {
        ASSERT_EQ(eg.indw, i++);
        ASSERT_EQ(eg.w, wD);
        ASSERT_EQ(eg.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrB_OutCyc_SrLiA_Srv

/*  J4_DrB_OutCyc_SrLiA_Srv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 9 w 1 c 0
                 1:   [1] > ind 8 indc 8 w 1 c 0
                 2:   [0] > ind 7 indc 7 w 1 c 0
                 3:   [2] > ind 2 indc 2 w 0 c 0
                 4:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 3 w 2 c 0
           [1] > ind 4 indc 4 w 2 c 0
           [2] > ind 5 indc 5 w 2 c 0
           [3] > ind 6 indc 6 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 5 > > > > > ( 1 2 7 8 9 )
           [0] > ind 1 indc 1 w 1 c 0
           [1] > ind 2 indc 2 w 1 c 0
           [2] > ind 7 indc 7 w 1 c 0
           [3] > ind 8 indc 8 w 1 c 0
           [4] > ind 9 indc 9 w 1 c 0
*/



/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end A of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrA_OutCyc_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end A, source edge is at tne end A of a linear chain. ",
            "Pulled sequence omits the cycle chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eB},
            BSlot{w0, 3});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eB},
            ESlot{w0, eB});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    // Pull three steps to consume the source chain completely.
    constexpr int n {3};
    pu1f(pp, n);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrA_OutCyc_SrLiA_Rmv

/*  J4_DrA_OutCyc_SrLiA_Rmv

TEST_BEFORE 1  ** {0 B} {2 A} {2 B}  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0  ** {2 A} {2 B} {1 B}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B} 0 [2] len 4 >>>> (3 4 5 6)
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
            [3] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 2 indc 5 w 0 c 0
                 3:   [2] > ind 9 indc 2 w 1 c 0
                 4:   [1] > ind 8 indc 1 w 1 c 0
source (end A)   5:   [0] > ind 7 indc 0 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 6 > > > > > > ( 0 1 2 9 8 7 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 9 indc 3 w 0 c 0
           [4] > ind 8 indc 4 w 0 c 0
           [5] > ind 7 indc 5 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 6 w 1 c 0
           [1] > ind 4 indc 7 w 1 c 0
           [2] > ind 5 indc 8 w 1 c 0
           [3] > ind 6 indc 9 w 1 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at the end B of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrA_OutCyc_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end A, source edge is at the end B of a linear chain. ",
            "Pulled edge sequence omits the cycle chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

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
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    // Pull three steps to consume the source chain completely.
    constexpr int n {3};
    pu1f(pp, n);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrA_OutCyc_SrLiB_Rmv

/*  J4_DrA_OutCyc_SrLiB_Rmv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [0] > ind 7 indc 7 w 1 c 0
                 4:   [1] > ind 8 indc 8 w 1 c 0
source (end B)   5:   [2] > ind 9 indc 9 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 6 > > > > > > ( 0 1 2 7 8 9 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 7 indc 3 w 0 c 0
           [4] > ind 8 indc 4 w 0 c 0
           [5] > ind 9 indc 5 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {1 A} {0 B}  0 [1]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 6 w 1 c 0
           [1] > ind 4 indc 7 w 1 c 0
           [2] > ind 5 indc 8 w 1 c 0
           [3] > ind 6 indc 9 w 1 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end B of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrB_OutCyc_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end B, source edge is at tne end B of a linear chain. ",
            "Pulled sequence omits the cycle chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eA},
            ESlot{w2, eA});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    // Pull three steps to consume the source chain completely.
    constexpr int n {3};
    pu1f(pp, n);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrB_OutCyc_SrLiB_Rmv

/*  J4_DrB_OutCyc_SrLiB_Rmv

TEST_BEFORE 1 {2 A} {0 A} {0 B}  **  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
            [3] > ind 3 indc 6 w 0 c 0
TEST_BEFORE 2 {0 A} {0 B} {1 A}  **  0 [2]  len 3 > > > ( 4 5 6 )
            [0] > ind 4 indc 7 w 2 c 0
            [1] > ind 5 indc 8 w 2 c 0
            [2] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 2 w 1 c 0
                 1:   [1] > ind 8 indc 1 w 1 c 0
                 2:   [0] > ind 7 indc 0 w 1 c 0
                 3:   [0] > ind 4 indc 7 w 2 c 0
                 4:   [1] > ind 5 indc 8 w 2 c 0
source (end B)   5:   [2] > ind 6 indc 9 w 2 c 0

TEST_AFTER 1 {0 A} {0 B}  **  0 [0]  len 6 > > > > > > ( 6 5 4 7 8 9 )
           [0] > ind 6 indc 0 w 1 c 0
           [1] > ind 5 indc 1 w 1 c 0
           [2] > ind 4 indc 2 w 1 c 0
           [3] > ind 7 indc 3 w 1 c 0
           [4] > ind 8 indc 4 w 1 c 0
           [5] > ind 9 indc 5 w 1 c 0
TEST_AFTER 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
           [0] > ind 0 indc 6 w 0 c 0
           [1] > ind 1 indc 7 w 0 c 0
           [2] > ind 2 indc 8 w 0 c 0
           [3] > ind 3 indc 9 w 0 c 0
*/

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at the end A of a linear chain.
/// Pulled edge sequence omits the cycle chain.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrB_OutCyc_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at ",
            "the end B, source edge is at the end A of a linear chain. ",
            "Pulled edge sequence omits the cycle chain. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w0;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    // Pull three steps to consume the source chain completely.
    constexpr int n {3};
    pu1f(pp, n);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrB_OutCyc_SrLiA_Rmv

/*  J4_DrB_OutCyc_SrLiA_Rmv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 9 w 1 c 0
                 1:   [1] > ind 8 indc 8 w 1 c 0
                 2:   [0] > ind 7 indc 7 w 1 c 0
                 3:   [2] > ind 2 indc 2 w 0 c 0
                 4:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   5:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [0]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 0 w 0 c 0
           [1] > ind 4 indc 1 w 0 c 0
           [2] > ind 5 indc 2 w 0 c 0
           [3] > ind 6 indc 3 w 0 c 0
TEST_AFTER 1 {0 A} {0 B}  **  0 [1]  len 6 > > > > > > ( 0 1 2 7 8 9 )
           [0] > ind 0 indc 4 w 1 c 0
           [1] > ind 1 indc 5 w 1 c 0
           [2] > ind 2 indc 6 w 1 c 0
           [3] > ind 7 indc 7 w 1 c 0
           [4] > ind 8 indc 8 w 1 c 0
           [5] > ind 9 indc 9 w 1 c 0
*/


// =============================================================================
// ===== Vertex Degree 4: Lin InCyc Lin ========================================
// =============================================================================

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// The source chain survives.
TEST_F(Pull_1, J4_DrA_InCyAB_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end A, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eB},
            BSlot{w0, 3});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eB},
            ESlot{w0, eB});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrA_InCyAB_SrLiA_Srv

/*  J4_DrA_InCyAB_SrLiA_Srv

TEST_BEFORE 1  ** {0 B} {2 A} {2 B}  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0  ** {2 A} {2 B} {1 B}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
            [3] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 2 indc 5 w 0 c 0
                 3:   [0] > ind 3 indc 6 w 2 c 0
                 4:   [1] > ind 4 indc 7 w 2 c 0
                 5:   [2] > ind 5 indc 8 w 2 c 0
                 6:   [3] > ind 6 indc 9 w 2 c 0
                 7:   [2] > ind 9 indc 2 w 1 c 0
                 8:   [1] > ind 8 indc 1 w 1 c 0
source (end A)   9:   [0] > ind 7 indc 0 w 1 c 0

TEST_AFTER 1  ** {0 B} {2 A} {2 B}  0 [0]  len 1 > ( 7 )
           [0] > ind 7 indc 0 w 1 c 0
TEST_AFTER 0  ** {2 A} {2 B} {1 B}  0 [1]  len 5 > > > > > ( 0 1 2 3 4 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
           [2] > ind 2 indc 5 w 0 c 0
           [3] > ind 3 indc 6 w 0 c 0
           [4] > ind 4 indc 7 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 5 6 9 8 )
           [0] > ind 5 indc 8 w 2 c 0
           [1] > ind 6 indc 9 w 2 c 0
           [2] > ind 9 indc 2 w 2 c 0
           [3] > ind 8 indc 1 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// The source chain survives.
TEST_F(Pull_1, J4_DrA_InCyBA_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end A, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eB},
            BSlot{w0, 3});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eB},
            ESlot{w0, eB});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrA_InCyBA_SrLiA_Srv

/* J4_DrA_InCyBA_SrLiA_Srv

TEST_BEFORE 1  ** {0 B} {2 A} {2 B}  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0  ** {2 A} {2 B} {1 B}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
            [3] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 2 indc 5 w 0 c 0
                 3:   [3] > ind 6 indc 9 w 2 c 0
                 4:   [2] > ind 5 indc 8 w 2 c 0
                 5:   [1] > ind 4 indc 7 w 2 c 0
                 6:   [0] > ind 3 indc 6 w 2 c 0
                 7:   [2] > ind 9 indc 2 w 1 c 0
                 8:   [1] > ind 8 indc 1 w 1 c 0
source (end A)   9:   [0] > ind 7 indc 0 w 1 c 0

TEST_AFTER 1  ** {0 B} {2 A} {2 B}  0 [0]  len 1 > ( 7 )
           [0] > ind 7 indc 0 w 1 c 0
TEST_AFTER 0  ** {2 A} {2 B} {1 B}  0 [1]  len 5 > > > > > ( 0 1 2 6 5 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
           [2] > ind 2 indc 5 w 0 c 0
           [3] > ind 6 indc 9 w 0 c 0
           [4] > ind 5 indc 8 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 8 9 3 4 )
           [0] > ind 8 indc 1 w 2 c 0
           [1] > ind 9 indc 2 w 2 c 0
           [2] > ind 3 indc 6 w 2 c 0
           [3] > ind 4 indc 7 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// The source chain survives.
TEST_F(Pull_1, J4_DrA_InCyAB_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eA;
    const auto wD = w0;
    constexpr auto eS = eB;
    const auto wS = w1;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrA_InCyAB_SrLiB_Srv

/* J4_DrA_InCyAB_SrLiB_Srv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [0] > ind 3 indc 3 w 2 c 0
                 4:   [1] > ind 4 indc 4 w 2 c 0
                 5:   [2] > ind 5 indc 5 w 2 c 0
                 6:   [3] > ind 6 indc 6 w 2 c 0
                 7:   [0] > ind 7 indc 7 w 1 c 0
                 8:   [1] > ind 8 indc 8 w 1 c 0
source (end B)   9:   [2] > ind 9 indc 9 w 1 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 5 > > > > > ( 0 1 2 3 4 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 3 indc 3 w 0 c 0
           [4] > ind 4 indc 4 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 5 6 7 8 )
           [0] > ind 5 indc 5 w 2 c 0
           [1] > ind 6 indc 6 w 2 c 0
           [2] > ind 7 indc 7 w 2 c 0
           [3] > ind 8 indc 8 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 1 > ( 9 )
           [0] > ind 9 indc 9 w 1 c 0

*/

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// The source chain survives.
TEST_F(Pull_1, J4_DrA_InCyBA_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eA;
    const auto wD = w0;
    constexpr auto eS = eB;
    const auto wS = w1;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrA_InCyBA_SrLiB_Srv

/* J4_DrA_InCyBA_SrLiB_Srv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [3] > ind 6 indc 6 w 2 c 0
                 4:   [2] > ind 5 indc 5 w 2 c 0
                 5:   [1] > ind 4 indc 4 w 2 c 0
                 6:   [0] > ind 3 indc 3 w 2 c 0
                 7:   [0] > ind 7 indc 7 w 1 c 0
                 8:   [1] > ind 8 indc 8 w 1 c 0
source (end B)   9:   [2] > ind 9 indc 9 w 1 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 5 > > > > > ( 0 1 2 6 5 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 6 indc 6 w 0 c 0
           [4] > ind 5 indc 5 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 8 7 3 4 )
           [0] > ind 8 indc 8 w 2 c 0
           [1] > ind 7 indc 7 w 2 c 0
           [2] > ind 3 indc 3 w 2 c 0
           [3] > ind 4 indc 4 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 1 > ( 9 )
           [0] > ind 9 indc 9 w 1 c 0
*/

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// The source chain survives.
TEST_F(Pull_1, J4_DrB_InCyAB_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end B of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eA},
            ESlot{w2, eA});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto wI = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrB_InCyAB_SrLiB_Srv

/* J4_DrB_InCyAB_SrLiB_Srv

TEST_BEFORE 1 {2 A} {0 A} {0 B}  **  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
            [3] > ind 3 indc 6 w 0 c 0
TEST_BEFORE 2 {0 A} {0 B} {1 A}  **  0 [2]  len 3 > > > ( 4 5 6 )
            [0] > ind 4 indc 7 w 2 c 0
            [1] > ind 5 indc 8 w 2 c 0
            [2] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 2 w 1 c 0
                 1:   [1] > ind 8 indc 1 w 1 c 0
                 2:   [0] > ind 7 indc 0 w 1 c 0
                 3:   [0] > ind 0 indc 3 w 0 c 0
                 4:   [1] > ind 1 indc 4 w 0 c 0
                 5:   [2] > ind 2 indc 5 w 0 c 0
                 6:   [3] > ind 3 indc 6 w 0 c 0
                 7:   [0] > ind 4 indc 7 w 2 c 0
                 8:   [1] > ind 5 indc 8 w 2 c 0
source (end B)   9:   [2] > ind 6 indc 9 w 2 c 0

TEST_AFTER 1 {2 A} {0 A} {0 B}  **  0 [0]  len 5 > > > > > ( 1 0 7 8 9 )
           [0] > ind 1 indc 4 w 1 c 0
           [1] > ind 0 indc 3 w 1 c 0
           [2] > ind 7 indc 0 w 1 c 0
           [3] > ind 8 indc 1 w 1 c 0
           [4] > ind 9 indc 2 w 1 c 0
TEST_AFTER 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 2 3 4 5 )
           [0] > ind 2 indc 5 w 0 c 0
           [1] > ind 3 indc 6 w 0 c 0
           [2] > ind 4 indc 7 w 0 c 0
           [3] > ind 5 indc 8 w 0 c 0
TEST_AFTER 2 {0 A} {0 B} {1 A}  **  0 [2]  len 1 > ( 6 )
           [0] > ind 6 indc 9 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// The source chain survives.
TEST_F(Pull_1, J4_DrB_InCyBA_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end B of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eA},
            ESlot{w2, eA});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto wI = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrB_InCyBA_SrLiB_Srv

/* J4_DrB_InCyBA_SrLiB_Srv

TEST_BEFORE 1 {2 A} {0 A} {0 B}  **  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
            [3] > ind 3 indc 6 w 0 c 0
TEST_BEFORE 2 {0 A} {0 B} {1 A}  **  0 [2]  len 3 > > > ( 4 5 6 )
            [0] > ind 4 indc 7 w 2 c 0
            [1] > ind 5 indc 8 w 2 c 0
            [2] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 2 w 1 c 0
                 1:   [1] > ind 8 indc 1 w 1 c 0
                 2:   [0] > ind 7 indc 0 w 1 c 0
                 3:   [3] > ind 3 indc 6 w 0 c 0
                 4:   [2] > ind 2 indc 5 w 0 c 0
                 5:   [1] > ind 1 indc 4 w 0 c 0
                 6:   [0] > ind 0 indc 3 w 0 c 0
                 7:   [0] > ind 4 indc 7 w 2 c 0
                 8:   [1] > ind 5 indc 8 w 2 c 0
source (end B)   9:   [2] > ind 6 indc 9 w 2 c 0

TEST_AFTER 1 {2 A} {0 A} {0 B}  **  0 [0]  len 5 > > > > > ( 2 3 7 8 9 )
           [0] > ind 2 indc 5 w 1 c 0
           [1] > ind 3 indc 6 w 1 c 0
           [2] > ind 7 indc 0 w 1 c 0
           [3] > ind 8 indc 1 w 1 c 0
           [4] > ind 9 indc 2 w 1 c 0
TEST_AFTER 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 5 4 0 1 )
           [0] > ind 5 indc 8 w 0 c 0
           [1] > ind 4 indc 7 w 0 c 0
           [2] > ind 0 indc 3 w 0 c 0
           [3] > ind 1 indc 4 w 0 c 0
TEST_AFTER 2 {0 A} {0 B} {1 A}  **  0 [2]  len 1 > ( 6 )
           [0] > ind 6 indc 9 w 2 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// The source chain survives.
TEST_F(Pull_1, J4_DrB_InCyAB_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    const auto wD = w1;
    constexpr auto eS = eA;
    const auto wS = w0;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrB_InCyAB_SrLiA_Srv

/* J4_DrB_InCyAB_SrLiA_Srv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 9 w 1 c 0
                 1:   [1] > ind 8 indc 8 w 1 c 0
                 2:   [0] > ind 7 indc 7 w 1 c 0
                 3:   [0] > ind 3 indc 3 w 2 c 0
                 4:   [1] > ind 4 indc 4 w 2 c 0
                 5:   [2] > ind 5 indc 5 w 2 c 0
                 6:   [3] > ind 6 indc 6 w 2 c 0
                 7:   [2] > ind 2 indc 2 w 0 c 0
                 8:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   9:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 5 6 2 1 )
           [0] > ind 5 indc 5 w 2 c 0
           [1] > ind 6 indc 6 w 2 c 0
           [2] > ind 2 indc 2 w 2 c 0
           [3] > ind 1 indc 1 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 5 > > > > > ( 4 3 7 8 9 )
           [0] > ind 4 indc 4 w 1 c 0
           [1] > ind 3 indc 3 w 1 c 0
           [2] > ind 7 indc 7 w 1 c 0
           [3] > ind 8 indc 8 w 1 c 0
           [4] > ind 9 indc 9 w 1 c 0

*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// The source chain survives.
TEST_F(Pull_1, J4_DrB_InCyBA_SrLiA_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    const auto wD = w1;
    constexpr auto eS = eA;
    const auto wS = w0;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wS].length() - n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wS].g[1].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }
    ASSERT_EQ(gr.cn[wS].g[0].indw, 0);
    ASSERT_EQ(gr.cn[wS].g[0].w, wS);
    ASSERT_EQ(gr.cn[wS].g[0].c, ic);

}  //  end J4_DrB_InCyBA_SrLiA_Srv

/* J4_DrB_InCyBA_SrLiA_Srv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 2 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 9 w 1 c 0
                 1:   [1] > ind 8 indc 8 w 1 c 0
                 2:   [0] > ind 7 indc 7 w 1 c 0
                 3:   [3] > ind 6 indc 6 w 2 c 0
                 4:   [2] > ind 5 indc 5 w 2 c 0
                 5:   [1] > ind 4 indc 4 w 2 c 0
                 6:   [0] > ind 3 indc 3 w 2 c 0
                 7:   [2] > ind 2 indc 2 w 0 c 0
                 8:   [1] > ind 1 indc 1 w 0 c 0
source (end A)   9:   [0] > ind 0 indc 0 w 0 c 0

TEST_AFTER 0  ** {2 A} {2 B} {1 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 1 2 3 4 )
           [0] > ind 1 indc 1 w 2 c 0
           [1] > ind 2 indc 2 w 2 c 0
           [2] > ind 3 indc 3 w 2 c 0
           [3] > ind 4 indc 4 w 2 c 0
TEST_AFTER 1 {2 A} {2 B} {0 B}  **  0 [2]  len 5 > > > > > ( 5 6 7 8 9 )
           [0] > ind 5 indc 5 w 1 c 0
           [1] > ind 6 indc 6 w 1 c 0
           [2] > ind 7 indc 7 w 1 c 0
           [3] > ind 8 indc 8 w 1 c 0
           [4] > ind 9 indc 9 w 1 c 0

*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrA_InCyAB_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end A, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eB},
            BSlot{w0, 3});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eB},
            ESlot{w0, eB});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[3].ind, gr0.cn[wS].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wS].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wS);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrA_InCyAB_SrLiA_Rmv

/* J4_DrA_InCyAB_SrLiA_Rmv
TEST_BEFORE 1  ** {0 B} {2 A} {2 B}  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0  ** {2 A} {2 B} {1 B}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
            [3] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 2 indc 5 w 0 c 0
                 3:   [0] > ind 3 indc 6 w 2 c 0
                 4:   [1] > ind 4 indc 7 w 2 c 0
                 5:   [2] > ind 5 indc 8 w 2 c 0
                 6:   [3] > ind 6 indc 9 w 2 c 0
                 7:   [2] > ind 9 indc 2 w 1 c 0
                 8:   [1] > ind 8 indc 1 w 1 c 0
source (end A)   9:   [0] > ind 7 indc 0 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 6 > > > > > > ( 0 1 2 3 4 5 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 3 indc 3 w 0 c 0
           [4] > ind 4 indc 4 w 0 c 0
           [5] > ind 5 indc 5 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 4 > > > > ( 6 9 8 7 )
           [0] > ind 6 indc 6 w 1 c 0
           [1] > ind 9 indc 7 w 1 c 0
           [2] > ind 8 indc 8 w 1 c 0
           [3] > ind 7 indc 9 w 1 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrA_InCyBA_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end A, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eB},
            BSlot{w0, 3});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eB},
            ESlot{w0, eB});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    constexpr auto eD = eA;
    const auto wS = w1;
    constexpr auto eS = eA;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[3].ind, gr0.cn[wI].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wS].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wS);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrA_InCyBA_SrLiA_Rmv

/* J4_DrA_InCyBA_SrLiA_Rmv

TEST_BEFORE 1  ** {0 B} {2 A} {2 B}  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0  ** {2 A} {2 B} {1 B}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 B}  ** {0 B} {2 A} {1 B}  0 [2]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 6 w 2 c 0
            [1] > ind 4 indc 7 w 2 c 0
            [2] > ind 5 indc 8 w 2 c 0
            [3] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 3 w 0 c 0
                 1:   [1] > ind 1 indc 4 w 0 c 0
                 2:   [2] > ind 2 indc 5 w 0 c 0
                 3:   [3] > ind 6 indc 9 w 2 c 0
                 4:   [2] > ind 5 indc 8 w 2 c 0
                 5:   [1] > ind 4 indc 7 w 2 c 0
                 6:   [0] > ind 3 indc 6 w 2 c 0
                 7:   [2] > ind 9 indc 2 w 1 c 0
                 8:   [1] > ind 8 indc 1 w 1 c 0
source (end A)   9:   [0] > ind 7 indc 0 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 6 > > > > > > ( 0 1 2 6 5 4 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 6 indc 3 w 0 c 0
           [4] > ind 5 indc 4 w 0 c 0
           [5] > ind 4 indc 9 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {0 B} {1 A}  0 [1]  len 4 > > > > ( 7 8 9 3 )
           [0] > ind 7 indc 5 w 1 c 0
           [1] > ind 8 indc 6 w 1 c 0
           [2] > ind 9 indc 7 w 1 c 0
           [3] > ind 3 indc 8 w 1 c 0

*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrA_InCyAB_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eA;
    const auto wD = w0;
    constexpr auto eS = eB;
    const auto wS = w1;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[3].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wS].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wS);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrA_InCyAB_SrLiB_Rmv

/* J4_DrA_InCyAB_SrLiB_Rmv
TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [0] > ind 3 indc 3 w 2 c 0
                 4:   [1] > ind 4 indc 4 w 2 c 0
                 5:   [2] > ind 5 indc 5 w 2 c 0
                 6:   [3] > ind 6 indc 6 w 2 c 0
                 7:   [0] > ind 7 indc 7 w 1 c 0
                 8:   [1] > ind 8 indc 8 w 1 c 0
source (end B)   9:   [2] > ind 9 indc 9 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 6 > > > > > > ( 0 1 2 3 4 5 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 3 indc 3 w 0 c 0
           [4] > ind 4 indc 4 w 0 c 0
           [5] > ind 5 indc 5 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {1 A} {0 B}  0 [1]  len 4 > > > > ( 6 7 8 9 )
           [0] > ind 6 indc 6 w 1 c 0
           [1] > ind 7 indc 7 w 1 c 0
           [2] > ind 8 indc 8 w 1 c 0
           [3] > ind 9 indc 9 w 1 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end A, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrA_InCyBA_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eA;
    const auto wD = w0;
    constexpr auto eS = eB;
    const auto wS = w1;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wS].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[3].ind, gr0.cn[wI].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wS].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wS);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrA_InCyBA_SrLiB_Rmv

/* J4_DrA_InCyBA_SrLiB_Rmv

TEST_BEFORE 0  ** {2 A} {2 B} {1 A}  0 [0]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 0 w 0 c 0
            [1] > ind 1 indc 1 w 0 c 0
            [2] > ind 2 indc 2 w 0 c 0
TEST_BEFORE 2 {0 B} {2 B} {1 A}  ** {2 A} {0 B} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
            [0] > ind 3 indc 3 w 2 c 0
            [1] > ind 4 indc 4 w 2 c 0
            [2] > ind 5 indc 5 w 2 c 0
            [3] > ind 6 indc 6 w 2 c 0
TEST_BEFORE 1 {2 A} {2 B} {0 B}  **  0 [2]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 7 w 1 c 0
            [1] > ind 8 indc 8 w 1 c 0
            [2] > ind 9 indc 9 w 1 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd A) 0:   [0] > ind 0 indc 0 w 0 c 0
                 1:   [1] > ind 1 indc 1 w 0 c 0
                 2:   [2] > ind 2 indc 2 w 0 c 0
                 3:   [3] > ind 6 indc 6 w 2 c 0
                 4:   [2] > ind 5 indc 5 w 2 c 0
                 5:   [1] > ind 4 indc 4 w 2 c 0
                 6:   [0] > ind 3 indc 3 w 2 c 0
                 7:   [0] > ind 7 indc 7 w 1 c 0
                 8:   [1] > ind 8 indc 8 w 1 c 0
source (end B)   9:   [2] > ind 9 indc 9 w 1 c 0

TEST_AFTER 0  ** {1 A} {1 B}  0 [0]  len 6 > > > > > > ( 0 1 2 6 5 4 )
           [0] > ind 0 indc 0 w 0 c 0
           [1] > ind 1 indc 1 w 0 c 0
           [2] > ind 2 indc 2 w 0 c 0
           [3] > ind 6 indc 3 w 0 c 0
           [4] > ind 5 indc 4 w 0 c 0
           [5] > ind 4 indc 9 w 0 c 0
TEST_AFTER 1 {0 B} {1 B}  ** {1 A} {0 B}  0 [1]  len 4 > > > > ( 9 8 7 3 )
           [0] > ind 9 indc 5 w 1 c 0
           [1] > ind 8 indc 6 w 1 c 0
           [2] > ind 7 indc 7 w 1 c 0
           [3] > ind 3 indc 8 w 1 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrB_InCyAB_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end B of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eA},
            ESlot{w2, eA});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto wI = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wS].g[2].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrB_InCyAB_SrLiB_Rmv

/*
TEST_BEFORE 1 {2 A} {0 A} {0 B}  **  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
            [3] > ind 3 indc 6 w 0 c 0
TEST_BEFORE 2 {0 A} {0 B} {1 A}  **  0 [2]  len 3 > > > ( 4 5 6 )
            [0] > ind 4 indc 7 w 2 c 0
            [1] > ind 5 indc 8 w 2 c 0
            [2] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 2 w 1 c 0
                 1:   [1] > ind 8 indc 1 w 1 c 0
                 2:   [0] > ind 7 indc 0 w 1 c 0
                 3:   [0] > ind 0 indc 3 w 0 c 0
                 4:   [1] > ind 1 indc 4 w 0 c 0
                 5:   [2] > ind 2 indc 5 w 0 c 0
                 6:   [3] > ind 3 indc 6 w 0 c 0
                 7:   [0] > ind 4 indc 7 w 2 c 0
                 8:   [1] > ind 5 indc 8 w 2 c 0
source (end B)   9:   [2] > ind 6 indc 9 w 2 c 0

TEST_AFTER 1 {0 A} {0 B}  **  0 [0]  len 6 > > > > > > ( 2 1 0 7 8 9 )
           [0] > ind 2 indc 5 w 1 c 0
           [1] > ind 1 indc 0 w 1 c 0
           [2] > ind 0 indc 1 w 1 c 0
           [3] > ind 7 indc 2 w 1 c 0
           [4] > ind 8 indc 3 w 1 c 0
           [5] > ind 9 indc 4 w 1 c 0
TEST_AFTER 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [1]  len 4 > > > > ( 3 4 5 6 )
           [0] > ind 3 indc 6 w 0 c 0
           [1] > ind 4 indc 7 w 0 c 0
           [2] > ind 5 indc 8 w 0 c 0
           [3] > ind 6 indc 9 w 0 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end B of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrB_InCyBA_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end B of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {7, 3};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w0, w2 : length (7) -> (3, 4)
    merge12(ESlot{w0, eA},
            BSlot{w0, 4});     // creates w2

    VertexMerger<1, 3, G> merge13 {gr};

    merge13(ESlot{w1, eA},
            ESlot{w2, eA});

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eB;
    const auto wS = w2;
    constexpr auto eS = eB;
    const auto wI = w0;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wD].length(), gr0.cn[wD].length() + n);
    ASSERT_EQ(gr.cn[wI].length(), gr0.cn[wI].length());
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wI].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[0].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wI].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wI].g[2].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wI].g[3].ind, gr0.cn[wI].g[0].ind);
    for (EgId i {}; const auto& g: gr.cn[wD].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wD);
        ASSERT_EQ(g.c, ic);
    }
    for (EgId i {}; const auto& g: gr.cn[wI].g) {
        ASSERT_EQ(g.indw, i++);
        ASSERT_EQ(g.w, wI);
        ASSERT_EQ(g.c, ic);
    }

}  //  end J4_DrB_InCyBA_SrLiB_Rmv

/*
TEST_BEFORE 1 {2 A} {0 A} {0 B}  **  0 [0]  len 3 > > > ( 7 8 9 )
            [0] > ind 7 indc 0 w 1 c 0
            [1] > ind 8 indc 1 w 1 c 0
            [2] > ind 9 indc 2 w 1 c 0
TEST_BEFORE 0 {0 B} {2 A} {1 A}  ** {0 A} {2 A} {1 A}  0 [1]  len 4 > > > > ( 0 1 2 3 )
            [0] > ind 0 indc 3 w 0 c 0
            [1] > ind 1 indc 4 w 0 c 0
            [2] > ind 2 indc 5 w 0 c 0
            [3] > ind 3 indc 6 w 0 c 0
TEST_BEFORE 2 {0 A} {0 B} {1 A}  **  0 [2]  len 3 > > > ( 4 5 6 )
            [0] > ind 4 indc 7 w 2 c 0
            [1] > ind 5 indc 8 w 2 c 0
            [2] > ind 6 indc 9 w 2 c 0

Pulling from Vertex Deg 1+ :: 3 steps over path:

driver (egEnd B) 0:   [2] > ind 9 indc 2 w 1 c 0
                 1:   [1] > ind 8 indc 1 w 1 c 0
                 2:   [0] > ind 7 indc 0 w 1 c 0
                 3:   [3] > ind 3 indc 6 w 0 c 0
                 4:   [2] > ind 2 indc 5 w 0 c 0
                 5:   [1] > ind 1 indc 4 w 0 c 0
                 6:   [0] > ind 0 indc 3 w 0 c 0
                 7:   [0] > ind 4 indc 7 w 2 c 0
                 8:   [1] > ind 5 indc 8 w 2 c 0
source (end B)   9:   [2] > ind 6 indc 9 w 2 c 0

TEST_AFTER 1 {0 A} {0 B}  **  0 [0]  len 6 > > > > > > ( 1 2 3 7 8 9 )
           [0] > ind 1 indc 9 w 1 c 0
           [1] > ind 2 indc 0 w 1 c 0
           [2] > ind 3 indc 1 w 1 c 0
           [3] > ind 7 indc 2 w 1 c 0
           [4] > ind 8 indc 3 w 1 c 0
           [5] > ind 9 indc 4 w 1 c 0
TEST_AFTER 0 {0 B} {1 A}  ** {0 A} {1 A}  0 [1]  len 4 > > > > ( 6 5 4 0 )
           [0] > ind 6 indc 5 w 0 c 0
           [1] > ind 5 indc 6 w 0 c 0
           [2] > ind 4 indc 7 w 0 c 0
           [3] > ind 0 indc 8 w 0 c 0
*/


/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction A -> B.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrB_InCyAB_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction A -> B. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    const auto wD = w1;
    constexpr auto eS = eA;
    const auto wS = w0;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eA), eA},
        Driver {&gr.cn[wI].end_edge(eB), eB}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.cn[wD].g[2].c, ic);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wS].length(), 4);
    ASSERT_EQ(gr.cn[wD].length(), 6);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[wI].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[wI].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[wI].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[3].ind, gr0.cn[wS].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wI].g[3].ind);
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
        ASSERT_EQ(gr.cn[wD].g[i].c, ic);
    }
    for (szt i {}; i<gr.cn[wS].length(); ++i) {
        ASSERT_EQ(gr.cn[wS].g[i].indw, i);
        ASSERT_EQ(gr.cn[wS].g[i].w, wS);
        ASSERT_EQ(gr.cn[wS].g[i].c, ic);
    }

}  //  end J4_DrB_InCyAB_SrLiA_Rmv

/// Tests degree 1 pulling over a path traversing a 4-way junction
/// connecting two linear chains and a cycle.
/// Driving edge is at the end B, source edge is at tne end A of a linear chain.
/// Pulled edge sequence includes the cycle chain in direction B -> A.
/// Source chain is engulfed completely.
TEST_F(Pull_1, J4_DrB_InCyBA_SrLiA_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 4-way junction ",
            "connecting two linear chains and a cycle. Driving edge is at the ",
            "end B, source edge is at tne end A of a linear chain. Pulled edge ",
            "sequence includes the cycle chain in direction B -> A. ",
            "Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr EgId len {10};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w1, w2 : length (10) -> (3, 3, 4)
    merge22(BSlot{w0, 3},
            BSlot{w0, 7});     // creates w1, w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto eD = eB;
    const auto wD = w1;
    constexpr auto eS = eA;
    const auto wS = w0;
    const auto wI = w2;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    const std::vector<Driver> internals {
        Driver {&gr.cn[wI].end_edge(eB), eB},
        Driver {&gr.cn[wI].end_edge(eA), eA}
    };
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src, internals};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.cn[wD].g[1].c, ic);
    ASSERT_EQ(gr.cn[wD].g[2].c, ic);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 1);
    ASSERT_EQ(gr.cn[wS].length(), 4);
    ASSERT_EQ(gr.cn[wD].length(), 6);
    ASSERT_EQ(gr.cn[wD].g[5].ind, gr0.cn[wD].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[4].ind, gr0.cn[wD].g[1].ind);
    ASSERT_EQ(gr.cn[wD].g[3].ind, gr0.cn[wD].g[0].ind);
    ASSERT_EQ(gr.cn[wD].g[2].ind, gr0.cn[w2].g[3].ind);
    ASSERT_EQ(gr.cn[wD].g[1].ind, gr0.cn[w2].g[2].ind);
    ASSERT_EQ(gr.cn[wD].g[0].ind, gr0.cn[w2].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[3].ind, gr0.cn[w2].g[0].ind);
    ASSERT_EQ(gr.cn[wS].g[2].ind, gr0.cn[wS].g[2].ind);
    ASSERT_EQ(gr.cn[wS].g[1].ind, gr0.cn[wS].g[1].ind);
    ASSERT_EQ(gr.cn[wS].g[0].ind, gr0.cn[wS].g[0].ind);
    for (szt i {}; i<gr.cn[wS].length(); ++i) {
        ASSERT_EQ(gr.cn[wS].g[i].indw, i);
        ASSERT_EQ(gr.cn[wS].g[i].w, wS);
    }
    for (szt i {}; i<gr.cn[wD].length(); ++i) {
        ASSERT_EQ(gr.cn[wD].g[i].indw, i);
        ASSERT_EQ(gr.cn[wD].g[i].w, wD);
    }

}  //  end J4_DrB_InCyBA_SrLiA_Rmv


/// Tests degree 1 pulling over a path traversing a 3-way and two 4-way
/// junctions. Pulling is driven at chain end A to source end B.
/// The source chain survives.
TEST_F(Pull_1, J344_DrA_SrLiB_Srv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way and ",
            "two 4-way junctions. Pulling is driven at chain end A to source ",
            "end B. The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 4> len {5, 7, 3, 3};
    const auto [w0, w1, w2, w3, w4, w5, w6, w7, w8] = create_array<ChId, 9>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};

    // w0, w5 : length (5) -> (2, 3)
    // w1, w4 : length (7) -> (1, 6)
    merge22(BSlot{w1, 1},
            BSlot{w0, 2});     // creates w4, w5

    // w4, w6 : length (6) -> (1, 5)
    // w2, w7 : length (3) -> (2, 1)
    merge22(BSlot{w4, 1},
            BSlot{w2, 2});     // creates w6, w7

    VertexMerger<1, 1, G> merge11 {gr};

    // make w2 cycle removing w7
    merge11(ESlot{w2, eA},
            ESlot{w7, eB});

    VertexMerger<1, 2, G> merge12 {gr};

    // w6, w7 : length (5) -> (2, 3)
    merge12(ESlot{w3, eA},
            BSlot{w6, 2});     // creates w7

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w1;
    constexpr auto eD = eA;
    const auto wS = w7;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].end_edge(eD), eD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {2};
    pu1f(pp, n);  // Pull two steps to leave source edge in the original chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    ASSERT_EQ(gr.cn[w6].length(), 2);
    ASSERT_EQ(gr.cn[w7].length(), 1);
    ASSERT_EQ(gr.cn[w1].g[1].ind, gr0.cn[w4].g[0].ind);
    ASSERT_EQ(gr.cn[w1].g[2].ind, gr0.cn[w6].g[0].ind);
    ASSERT_EQ(gr.cn[w4].g[0].ind, gr0.cn[w6].g[1].ind);
    ASSERT_EQ(gr.cn[w6].g[0].ind, gr0.cn[w7].g[0].ind);
    ASSERT_EQ(gr.cn[w6].g[1].ind, gr0.cn[w7].g[1].ind);
    ASSERT_EQ(gr.cn[w1].g[1].indc, gr0.cn[w4].g[0].indc);
    ASSERT_EQ(gr.cn[w1].g[2].indc, gr0.cn[w6].g[0].indc);
    ASSERT_EQ(gr.cn[w4].g[0].indc, gr0.cn[w6].g[1].indc);
    ASSERT_EQ(gr.cn[w6].g[0].indc, gr0.cn[w7].g[0].indc);
    ASSERT_EQ(gr.cn[w6].g[1].indc, gr0.cn[w7].g[1].indc);
    ASSERT_EQ(gr.cn[w1].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w1].g[2].indw, 2);
    ASSERT_EQ(gr.cn[w4].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w6].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w6].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w1].g[1].w, w1);
    ASSERT_EQ(gr.cn[w1].g[2].w, w1);
    ASSERT_EQ(gr.cn[w4].g[0].w, w4);
    ASSERT_EQ(gr.cn[w6].g[0].w, w6);
    ASSERT_EQ(gr.cn[w6].g[1].w, w6);
    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.cn[w1].g[1].c, ic);
    ASSERT_EQ(gr.cn[w1].g[2].c, ic);
    ASSERT_EQ(gr.cn[w4].g[0].c, ic);
    ASSERT_EQ(gr.cn[w6].g[0].c, ic);
    ASSERT_EQ(gr.cn[w6].g[1].c, ic);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.ct[ic].ww, gr0.ct[ic].ww);
    ASSERT_EQ(gr.ct[ic].chis, gr0.ct[ic].chis);

}  //  end J344_DrA_SrLiB_Srv


/// Tests degree 1 pulling over a path traversing a 3-way and
/// two 4-way junctions. Source chain is engulfed completely.
TEST_F(Pull_1, J344_DrA_SrLiB_Rmv)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 1 pulling over a path traversing a 3-way and ",
            "two 4-way junctions. Source chain is engulfed completely."
        );

    // Create initial graph.

    constexpr std::array<EgId, 4> len {5, 7, 3, 3};
    const auto [w0, w1, w2, w3, w4, w5, w6, w7, w8] = create_array<ChId, 9>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};

    // w0, w5 : length (5) -> (2, 3)
    // w1, w4 : length (7) -> (1, 6)
    merge22(BSlot{w1, 1},
            BSlot{w0, 2});     // creates w4, w5

    // w4, w6 : length (6) -> (1, 5)
    // w2, w7 : length (3) -> (2, 1)
    merge22(BSlot{w4, 1},
            BSlot{w2, 2});     // creates w6, w7

    VertexMerger<1, 1, G> merge11 {gr};

    // make w2 cycle removing w7
    merge11(ESlot{w2, eA},
            ESlot{w7, eB});

    VertexMerger<1, 2, G> merge12 {gr};

    // w6, w7 : length (5) -> (2, 3)
    merge12(ESlot{w3, eA},
            BSlot{w6, 2});     // creates w7

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w1].tail(), eA};
    const Source src {w7, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<1, Orientation::Forwards, G> pu1f {gr};

    constexpr int n {3};
    pu1f(pp, n);  // Pull three steps to consume the source chain.

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    ASSERT_EQ(gr.cn[w1].length(), 4);
    ASSERT_EQ(gr.cn[w3].length(), 5);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    ASSERT_EQ(gr.cn[w1].g[0].ind, gr0.cn[w1].g[0].ind);
    ASSERT_EQ(gr.cn[w1].g[1].ind, gr0.cn[w4].g[0].ind);
    ASSERT_EQ(gr.cn[w1].g[2].ind, gr0.cn[w6].g[0].ind);
    ASSERT_EQ(gr.cn[w1].g[3].ind, gr0.cn[w6].g[1].ind);
    ASSERT_EQ(gr.cn[w4].g[0].ind, gr0.cn[w7].g[0].ind);
    ASSERT_EQ(gr.cn[w3].g[0].ind, gr0.cn[w7].g[1].ind);
    ASSERT_EQ(gr.cn[w3].g[1].ind, gr0.cn[w7].g[2].ind);
    ASSERT_EQ(gr.cn[w1].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w1].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w1].g[2].indw, 2);
    ASSERT_EQ(gr.cn[w1].g[3].indw, 3);
    ASSERT_EQ(gr.cn[w4].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w3].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w3].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w1].g[0].w, w1);
    ASSERT_EQ(gr.cn[w1].g[1].w, w1);
    ASSERT_EQ(gr.cn[w1].g[2].w, w1);
    ASSERT_EQ(gr.cn[w1].g[3].w, w1);
    ASSERT_EQ(gr.cn[w4].g[0].w, w4);
    ASSERT_EQ(gr.cn[w3].g[0].w, w3);
    ASSERT_EQ(gr.cn[w3].g[1].w, w3);
    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.cn[w1].g[0].c, ic);
    ASSERT_EQ(gr.cn[w1].g[1].c, ic);
    ASSERT_EQ(gr.cn[w1].g[2].c, ic);
    ASSERT_EQ(gr.cn[w1].g[3].c, ic);
    ASSERT_EQ(gr.cn[w4].g[0].c, ic);
    ASSERT_EQ(gr.cn[w3].g[0].c, ic);
    ASSERT_EQ(gr.cn[w3].g[1].c, ic);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() - 2);

}  //  end J344_DrA_SrLiB_Rmv


}  // namespace graph_mutator::tests::pulling::d1