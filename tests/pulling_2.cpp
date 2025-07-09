#include "pulling.h"


namespace graph_mutator::tests::pulling::d2 {

using Pull_2 = Test;


// =============================================================================
// ===== DEGREE 2 ==============================================================
// =============================================================================

/// Tests single-step pulling degree 2 over a single-edge path at chain end A.
TEST_F(Pull_2, Path1_Step1_SingleLinChain1A)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests single-step pulling degree 2 over a single-edge path ",
            "at chain end A"
        );

    // Create initial graph.

    constexpr EgId len {5};
    constexpr ChId w {};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto wD = w;
    constexpr auto aD = 0;
    constexpr auto egeD = eB;
    constexpr auto wS = w;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].g[aD], egeD};
    constexpr Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull single step.
    pu2f(pp, 1);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.cmpt_num(), 1);
    ASSERT_EQ(ic, 0);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[w].length(), 5);
    ASSERT_EQ(gr.cn[w].g[0].ind, 0);
    ASSERT_EQ(gr.cn[w].g[1].ind, 1);
    ASSERT_EQ(gr.cn[w].g[2].ind, 2);
    ASSERT_EQ(gr.cn[w].g[3].ind, 3);
    ASSERT_EQ(gr.cn[w].g[4].ind, 4);
    ASSERT_EQ(gr.cn[w].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w].g[2].indw, 2);
    ASSERT_EQ(gr.cn[w].g[3].indw, 3);
    ASSERT_EQ(gr.cn[w].g[4].indw, 4);
    ASSERT_EQ(gr.cn[w].g[0].w, w);
    ASSERT_EQ(gr.cn[w].g[1].w, w);
    ASSERT_EQ(gr.cn[w].g[2].w, w);
    ASSERT_EQ(gr.cn[w].g[3].w, w);
    ASSERT_EQ(gr.cn[w].g[4].w, w);
    ASSERT_EQ(gr.cn[w].g[0].c, ic);
    ASSERT_EQ(gr.cn[w].g[1].c, ic);
    ASSERT_EQ(gr.cn[w].g[2].c, ic);
    ASSERT_EQ(gr.cn[w].g[3].c, ic);
    ASSERT_EQ(gr.cn[w].g[4].c, ic);
    ASSERT_FALSE(gr.cn[w].g[0].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[1].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[2].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[3].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[4].points_forwards());
    ASSERT_EQ(gr.cn[w].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w].ngs[eB].num(), 0);

}  //  end Path1_Step1_SingleLinChain1A


/// Tests single-step pulling degree 2 over a single-edge path at chain end B.
TEST_F(Pull_2, Path1_Step1_SingleLinChain1B)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests single-step pulling degree 2 over a single-edge path ",
            "at chain end B"
        );

    // Create initial graph.

    constexpr EgId len {5};
    constexpr ChId w {};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto wD = w;
    constexpr auto aD = 4;
    constexpr auto egeD = eA;
    constexpr auto wS = w;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].g[aD], egeD};
    constexpr Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull single step.
    pu2f(pp, 1);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic =pp.cmp->ind;
    ASSERT_EQ(gr.cmpt_num(), 1);
    ASSERT_EQ(ic, 0);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[w].length(), 5);
    ASSERT_EQ(gr.cn[w].g[0].ind, 0);
    ASSERT_EQ(gr.cn[w].g[1].ind, 1);
    ASSERT_EQ(gr.cn[w].g[2].ind, 2);
    ASSERT_EQ(gr.cn[w].g[3].ind, 3);
    ASSERT_EQ(gr.cn[w].g[4].ind, 4);
    ASSERT_EQ(gr.cn[w].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w].g[2].indw, 2);
    ASSERT_EQ(gr.cn[w].g[3].indw, 3);
    ASSERT_EQ(gr.cn[w].g[4].indw, 4);
    ASSERT_EQ(gr.cn[w].g[0].w, w);
    ASSERT_EQ(gr.cn[w].g[1].w, w);
    ASSERT_EQ(gr.cn[w].g[2].w, w);
    ASSERT_EQ(gr.cn[w].g[3].w, w);
    ASSERT_EQ(gr.cn[w].g[4].w, w);
    ASSERT_EQ(gr.cn[w].g[0].c, ic);
    ASSERT_EQ(gr.cn[w].g[1].c, ic);
    ASSERT_EQ(gr.cn[w].g[2].c, ic);
    ASSERT_EQ(gr.cn[w].g[3].c, ic);
    ASSERT_EQ(gr.cn[w].g[4].c, ic);
    ASSERT_TRUE(gr.cn[w].g[0].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[1].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[2].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[3].points_forwards());
    ASSERT_FALSE(gr.cn[w].g[4].points_forwards());
    ASSERT_EQ(gr.cn[w].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w].ngs[eB].num(), 0);

}  //  end Path1_Step1_SingleLinChain1B


/// Tests single-step pulling degree 2 inside a single linear chain compartment
/// and a source edge at end A.
TEST_F(Pull_2, Step1_PathMult_SingleLinChain_DrvInner_SrcA)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests single-step pulling degree 2 of inside a single linear chain",
            " compartment and a source edge at end A"
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1, w2] = create_array<ChId, 3>();

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w0;
    const auto aD = 2;
    const auto egeD = eB;
    const auto wS = w0;
    const auto eS = eA;

    const Driver drv {&gr.cn[wD].g[aD], egeD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull single step.
    pu2f(pp, 1);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.cmpt_num(), 1);
    ASSERT_EQ(ic, 0);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 2);
    ASSERT_EQ(gr.cn[w2].length(), 1);
    ASSERT_EQ(gr.cn[w0].g[0].ind, 0);
    ASSERT_EQ(gr.cn[w0].g[1].ind, 1);
    ASSERT_EQ(gr.cn[w1].g[0].ind, 3);
    ASSERT_EQ(gr.cn[w1].g[1].ind, 4);
    ASSERT_EQ(gr.cn[w2].g[0].ind, 2);
    ASSERT_EQ(gr.cn[w0].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w0].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w1].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w1].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w2].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w0].g[0].w, w0);
    ASSERT_EQ(gr.cn[w0].g[1].w, w0);
    ASSERT_EQ(gr.cn[w1].g[0].w, w1);
    ASSERT_EQ(gr.cn[w1].g[1].w, w1);
    ASSERT_EQ(gr.cn[w2].g[0].w, w2);
    ASSERT_EQ(gr.cn[w0].g[0].c, ic);
    ASSERT_EQ(gr.cn[w0].g[1].c, ic);
    ASSERT_EQ(gr.cn[w1].g[0].c, ic);
    ASSERT_EQ(gr.cn[w1].g[1].c, ic);
    ASSERT_EQ(gr.cn[w2].g[0].c, ic);
    ASSERT_EQ(gr.cn[w0].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w0].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[w0].ngs[eB][0].w, w1);
    ASSERT_EQ(gr.cn[w0].ngs[eB][0].e, eA);
    ASSERT_EQ(gr.cn[w0].ngs[eB][1].w, w2);
    ASSERT_EQ(gr.cn[w0].ngs[eB][1].e, eA);
    ASSERT_EQ(gr.cn[w1].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[w1].ngs[eB].num(), 0);
    ASSERT_EQ(gr.cn[w1].ngs[eA][0].w, w0);
    ASSERT_EQ(gr.cn[w1].ngs[eA][0].e, eB);
    ASSERT_EQ(gr.cn[w1].ngs[eA][1].w, w2);
    ASSERT_EQ(gr.cn[w1].ngs[eA][1].e, eA);
    ASSERT_EQ(gr.cn[w2].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[w2].ngs[eB].num(), 0);
    ASSERT_EQ(gr.cn[w2].ngs[eA][0].w, w1);
    ASSERT_EQ(gr.cn[w2].ngs[eA][0].e, eA);
    ASSERT_EQ(gr.cn[w2].ngs[eA][1].w, w0);
    ASSERT_EQ(gr.cn[w2].ngs[eA][1].e, eB);

}  //  end Step1_PathMult_SingleLinChain_DrvInner_SrcA


/// Tests pulling degree 2 over a multi-edge path driven at chain intermediate
/// edge and a source edge at end A. Number of steps equals to the path length.
TEST_F(Pull_2, StepFull_SingleLinChain_DrvInner_SrcA)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests pulling degree 2 over a multi-edge path driven at chain ",
            "intermediate edge and a source edge at end A. ",
            "Number of steps equals to the path length."
        );

    // Create initial graph.

    constexpr EgId len {5};
    constexpr ChId w {};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto wD = w;
    constexpr auto aD = 2;
    constexpr auto egeD = eB;
    constexpr auto wS = w;
    constexpr auto eS = eA;

    const Driver drv {&gr.cn[wD].g[aD], egeD};
    constexpr Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic =pp.cmp->ind;
    ASSERT_EQ(gr.cmpt_num(), 1);
    ASSERT_EQ(ic, 0);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[w].length(), 5);
    ASSERT_EQ(gr.cn[w].g[0].ind, 2);
    ASSERT_EQ(gr.cn[w].g[1].ind, 1);
    ASSERT_EQ(gr.cn[w].g[2].ind, 0);
    ASSERT_EQ(gr.cn[w].g[3].ind, 3);
    ASSERT_EQ(gr.cn[w].g[4].ind, 4);
    ASSERT_EQ(gr.cn[w].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w].g[2].indw, 2);
    ASSERT_EQ(gr.cn[w].g[3].indw, 3);
    ASSERT_EQ(gr.cn[w].g[4].indw, 4);
    ASSERT_EQ(gr.cn[w].g[0].w, w);
    ASSERT_EQ(gr.cn[w].g[1].w, w);
    ASSERT_EQ(gr.cn[w].g[2].w, w);
    ASSERT_EQ(gr.cn[w].g[3].w, w);
    ASSERT_EQ(gr.cn[w].g[4].w, w);
    ASSERT_EQ(gr.cn[w].g[0].c, ic);
    ASSERT_EQ(gr.cn[w].g[1].c, ic);
    ASSERT_EQ(gr.cn[w].g[2].c, ic);
    ASSERT_EQ(gr.cn[w].g[3].c, ic);
    ASSERT_EQ(gr.cn[w].g[4].c, ic);
    ASSERT_FALSE(gr.cn[w].g[0].points_forwards());
    ASSERT_FALSE(gr.cn[w].g[1].points_forwards());
    ASSERT_FALSE(gr.cn[w].g[2].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[3].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[4].points_forwards());
    ASSERT_EQ(gr.cn[w].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w].ngs[eB].num(), 0);

}  //  end  StepFull_SingleLinChain_DrvInner_SrcA


/// Tests single-step pulling degree 2 over a multi-edge path driven at chain
/// intermediate edge and a source edge at end B.
TEST_F(Pull_2, Step1_SingleLinChain_DrvInner_SrcB)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests single-step pulling degree 2 over a multi-edge path ",
            "driven at chain intermediate edge and a source edge at end B."
        );

    // Create initial graph.

    constexpr EgId len {5};
    const auto [w0, w1, w2] = create_array<ChId, 3>();
    const auto w {w0};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const auto wD = w;
    constexpr auto aD = 2;
    constexpr auto egeD = eA;
    const auto wS = w;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].g[aD], egeD};
    const Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull single step.
    pu2f(pp, 1);  // creates w1, w2

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic =pp.cmp->ind;
    ASSERT_EQ(gr.cmpt_num(), 1);
    ASSERT_EQ(ic, 0);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 2);
    ASSERT_EQ(gr.cn[w0].g[0].ind, 0);
    ASSERT_EQ(gr.cn[w0].g[1].ind, 1);
    ASSERT_EQ(gr.cn[w1].g[0].ind, 2);
    ASSERT_EQ(gr.cn[w2].g[0].ind, 3);
    ASSERT_EQ(gr.cn[w2].g[1].ind, 4);
    ASSERT_EQ(gr.cn[w0].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w0].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w1].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w2].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w2].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w0].g[0].w, w0);
    ASSERT_EQ(gr.cn[w0].g[1].w, w0);
    ASSERT_EQ(gr.cn[w1].g[0].w, w1);
    ASSERT_EQ(gr.cn[w2].g[0].w, w2);
    ASSERT_EQ(gr.cn[w2].g[1].w, w2);
    ASSERT_EQ(gr.cn[w0].g[0].c, ic);
    ASSERT_EQ(gr.cn[w0].g[1].c, ic);
    ASSERT_EQ(gr.cn[w1].g[0].c, ic);
    ASSERT_EQ(gr.cn[w0].g[0].c, ic);
    ASSERT_EQ(gr.cn[w0].g[1].c, ic);
    ASSERT_TRUE(gr.cn[w0].g[0].points_forwards());
    ASSERT_TRUE(gr.cn[w0].g[1].points_forwards());
    ASSERT_TRUE(gr.cn[w1].g[0].points_forwards());
    ASSERT_TRUE(gr.cn[w2].g[0].points_forwards());
    ASSERT_TRUE(gr.cn[w2].g[1].points_forwards());
    ASSERT_EQ(gr.cn[w0].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w0].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[w1].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w1].ngs[eB].num(), 2);
    ASSERT_EQ(gr.cn[w2].ngs[eA].num(), 2);
    ASSERT_EQ(gr.cn[w2].ngs[eB].num(), 0);

}  //  end  Step1_SingleLinChain_DrvInner_SrcB


/// Tests pulling degree 2 over a multi-edge path driven at chain intermediate
/// edge and a source edge at end B. Number of steps equals to the path length.
TEST_F(Pull_2, StepFull_SingleLinChain_DrvInner_SrcB)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests pulling degree 2 over a multi-edge path driven at chain ",
            "intermediate edge and a source edge at end B. ",
            "Number of steps equals to the path length."
        );

    // Create initial graph.

    constexpr EgId len {5};
    constexpr ChId w {};

    G gr;
    gr.add_single_chain_component(len);

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    constexpr auto wD = w;
    constexpr auto aD = 2;
    constexpr auto egeD = eA;
    constexpr auto wS = w;
    constexpr auto eS = eB;

    const Driver drv {&gr.cn[wD].g[aD], egeD};
    constexpr Source src {wS, eS};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic =pp.cmp->ind;
    ASSERT_EQ(gr.cmpt_num(), 1);
    ASSERT_EQ(ic, 0);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.cn[w].length(), 5);
    ASSERT_EQ(gr.cn[w].g[0].ind, 0);
    ASSERT_EQ(gr.cn[w].g[1].ind, 1);
    ASSERT_EQ(gr.cn[w].g[2].ind, 4);
    ASSERT_EQ(gr.cn[w].g[3].ind, 3);
    ASSERT_EQ(gr.cn[w].g[4].ind, 2);
    ASSERT_EQ(gr.cn[w].g[0].indw, 0);
    ASSERT_EQ(gr.cn[w].g[1].indw, 1);
    ASSERT_EQ(gr.cn[w].g[2].indw, 2);
    ASSERT_EQ(gr.cn[w].g[3].indw, 3);
    ASSERT_EQ(gr.cn[w].g[4].indw, 4);
    ASSERT_EQ(gr.cn[w].g[0].w, w);
    ASSERT_EQ(gr.cn[w].g[1].w, w);
    ASSERT_EQ(gr.cn[w].g[2].w, w);
    ASSERT_EQ(gr.cn[w].g[3].w, w);
    ASSERT_EQ(gr.cn[w].g[4].w, w);
    ASSERT_EQ(gr.cn[w].g[0].c, ic);
    ASSERT_EQ(gr.cn[w].g[1].c, ic);
    ASSERT_EQ(gr.cn[w].g[2].c, ic);
    ASSERT_EQ(gr.cn[w].g[3].c, ic);
    ASSERT_EQ(gr.cn[w].g[4].c, ic);
    ASSERT_TRUE(gr.cn[w].g[0].points_forwards());
    ASSERT_TRUE(gr.cn[w].g[1].points_forwards());
    ASSERT_FALSE(gr.cn[w].g[2].points_forwards());
    ASSERT_FALSE(gr.cn[w].g[3].points_forwards());
    ASSERT_FALSE(gr.cn[w].g[4].points_forwards());
    ASSERT_EQ(gr.cn[w].ngs[eA].num(), 0);
    ASSERT_EQ(gr.cn[w].ngs[eB].num(), 0);

}  //  end  StepFull_SingleLinChain_DrvInner_SrcB


/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end A of a linear chain pulls end A of a linear source chain.
/// The source chain survives.
TEST_F(Pull_2, Junct3_LinDrvA_LinSrcA_SrcSurvives)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end A of a linear chain pulls end A of a linear ",
            "source chain. The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 2};
    const auto [w0, w1, w2, w3, w4] = create_array<ChId, 5>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    merge12(ESlot{w1, eB},
            BSlot{w0, 3});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w1].g[1], eA};
    const Source src {w0, eA};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 2);
    ASSERT_EQ(gr.cn[w3].length(), 2);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvA_LinSrcA_SrcSurvives
/*
TEST_BEFORE 1  ** {0 B} {2 A}  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
            [2] > ind 2 indc 4 w 0 c 0
TEST_BEFORE 2 {1 B} {0 B}  **  0 [2]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 5 w 2 c 0
            [1] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 2+ :: 2 steps over path:

driver (egEnd A) 0:   [1] > ind 6 indc 1 w 1 c 0
                 1:   [2] > ind 2 indc 4 w 0 c 0
                 2:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   3:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 1  ** {3 B} {4 A}  0 [0]  len 1 > ( 5 )
           [0] > ind 5 indc 0 w 1 c 0
TEST_AFTER 3  ** {1 B} {4 A}  0 [1]  len 2 > > ( 6 2 )
           [0] > ind 6 indc 1 w 3 c 0
           [1] > ind 2 indc 6 w 3 c 0
TEST_AFTER 0  ** {4 B} {2 A}  0 [2]  len 1 > ( 0 )
           [0] > ind 0 indc 2 w 0 c 0
TEST_AFTER 2 {4 B} {0 B}  **  0 [3]  len 2 > > ( 3 4 )
           [0] > ind 3 indc 4 w 2 c 0
           [1] > ind 4 indc 5 w 2 c 0
TEST_AFTER 4 {1 B} {3 B}  ** {0 B} {2 A}  0 [4]  len 1 > ( 1 )
           [0] > ind 1 indc 3 w 4 c 0
*/

/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end B of a linear chain pulls end B of a linear source chain.
/// The source chain survives.
TEST_F(Pull_2, Junct3_LinDrvB_LinSrcB_SrcSurvives)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end B of a linear chain pulls end B of a linear ",
            "source chain. The source chain survives."
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

    const Driver drv {&gr.cn[w1].g[0], eB};
    const Source src {w2, eB};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 1);
    ASSERT_EQ(gr.cn[w3].length(), 1);
    ASSERT_EQ(gr.cn[w4].length(), 2);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Pull_2Junct3_LinDrvB_LinSrcB_SrcSurvives

/*
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

Pulling from Vertex Deg 2+ :: 2 steps over path:

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [0] > ind 2 indc 4 w 2 c 0
                 2:   [1] > ind 3 indc 5 w 2 c 0
source (end B)   3:   [2] > ind 4 indc 6 w 2 c 0

TEST_AFTER 3 {1 B} {4 A}  **  0 [0]  len 1 > ( 6 )
           [0] > ind 6 indc 0 w 3 c 0
TEST_AFTER 1 {0 B} {2 A}  ** {3 A} {4 A}  0 [1]  len 1 > ( 3 )
           [0] > ind 3 indc 4 w 1 c 0
TEST_AFTER 0  ** {1 A} {2 A}  0 [2]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 2 w 0 c 0
           [1] > ind 1 indc 3 w 0 c 0
TEST_AFTER 2 {1 A} {0 B}  **  0 [3]  len 1 > ( 4 )
           [0] > ind 4 indc 5 w 2 c 0
TEST_AFTER 4 {3 A} {1 B}  **  0 [4]  len 2 > > ( 2 5 )
           [0] > ind 2 indc 1 w 4 c 0
           [1] > ind 5 indc 6 w 4 c 0
*/

/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end A of a linear chain pulls end B of a linear source chain.
/// The source chain survives.
TEST_F(Pull_2, Junct3_LinDrvA_LinSrcB_SrcSurvives)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end A of a linear chain pulls end B of a linear ",
            "source chain. The source chain survives."
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

    const Driver drv {&gr.cn[w0].g[0], eB};
    const Source src {w2, eB};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 2);
    ASSERT_EQ(gr.cn[w2].length(), 1);
    ASSERT_EQ(gr.cn[w3].length(), 2);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvA_LinSrcB_SrcSurvives

/*
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

Pulling from Vertex Deg 2+ :: 2 steps over path:

driver (egEnd A) 0:   [1] > ind 1 indc 3 w 0 c 0
                 1:   [0] > ind 2 indc 4 w 2 c 0
                 2:   [1] > ind 3 indc 5 w 2 c 0
source (end B)   3:   [2] > ind 4 indc 6 w 2 c 0

TEST_AFTER 0  ** {3 B} {4 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 3  ** {0 B} {4 A}  0 [1]  len 2 > > ( 1 2 )
           [0] > ind 1 indc 1 w 3 c 0
           [1] > ind 2 indc 6 w 3 c 0
TEST_AFTER 1 {4 B} {2 A}  **  0 [2]  len 2 > > ( 5 6 )
           [0] > ind 5 indc 2 w 1 c 0
           [1] > ind 6 indc 3 w 1 c 0
TEST_AFTER 2 {1 A} {4 B}  **  0 [3]  len 1 > ( 4 )
           [0] > ind 4 indc 5 w 2 c 0
TEST_AFTER 4 {0 B} {3 B}  ** {1 A} {2 A}  0 [4]  len 1 > ( 3 )
           [0] > ind 3 indc 4 w 4 c 0
*/

/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end A of a linear chain pulls end B of a linear source chain.
/// The source chain survives.
TEST_F(Pull_2, Junct3_LinDrvB_LinSrcA_SrcSurvives)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end B of a linear chain pulls end A of a linear ",
            "source chain. The source chain survives."
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
            BSlot{w0, 3});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w1].g[0], eB};
    const Source src {w0, eA};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 2);
    ASSERT_EQ(gr.cn[w3].length(), 1);
    ASSERT_EQ(gr.cn[w4].length(), 2);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvB_LinSrcA_SrcSurvives
/*
TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
            [2] > ind 2 indc 4 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 5 w 2 c 0
            [1] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 2+ :: 2 steps over path:

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [2] > ind 2 indc 4 w 0 c 0
                 2:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   3:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 3 {1 B} {4 A}  **  0 [0]  len 1 > ( 6 )
           [0] > ind 6 indc 0 w 3 c 0
TEST_AFTER 1 {0 B} {2 A}  ** {3 A} {4 A}  0 [1]  len 1 > ( 1 )
           [0] > ind 1 indc 3 w 1 c 0
TEST_AFTER 0  ** {1 A} {2 A}  0 [2]  len 1 > ( 0 )
           [0] > ind 0 indc 2 w 0 c 0
TEST_AFTER 2 {1 A} {0 B}  **  0 [3]  len 2 > > ( 3 4 )
           [0] > ind 3 indc 4 w 2 c 0
           [1] > ind 4 indc 5 w 2 c 0
TEST_AFTER 4 {3 A} {1 B}  **  0 [4]  len 2 > > ( 2 5 )
           [0] > ind 2 indc 1 w 4 c 0
           [1] > ind 5 indc 6 w 4 c 0
*/


/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end A of a linear chain pulls end A of a linear source chain.
/// The source chain is consumed.
TEST_F(Pull_2, Junct3_LinDrvA_LinSrcA_SrcConsumed)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end A of a linear chain pulls end A of a linear ",
            "source chain. The source chain is consumed."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 2};
    const auto [w0, w1, w2, w3, w4] = create_array<ChId, 5>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<1, 2, G> merge12 {gr};
    // w1, w0 : length (5) -> (2, 3)
    merge12(ESlot{w1, eB},
            BSlot{w0, 3});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w1].g[1], eA};
    const Source src {w0, eA};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 3);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 3);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvA_LinSrcA_SrcConsumed

/*
TEST_BEFORE 1  ** {0 B} {2 A}  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 B} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
            [2] > ind 2 indc 4 w 0 c 0
TEST_BEFORE 2 {1 B} {0 B}  **  0 [2]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 5 w 2 c 0
            [1] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 2+ :: 3 steps over path:

driver (egEnd A) 0:   [1] > ind 6 indc 1 w 1 c 0
                 1:   [2] > ind 2 indc 4 w 0 c 0
                 2:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   3:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 2 {1 B} {0 B}  **  0 [0]  len 3 > > > ( 0 3 4 )
           [0] > ind 0 indc 1 w 2 c 0
           [1] > ind 3 indc 2 w 2 c 0
           [2] > ind 4 indc 3 w 2 c 0
TEST_AFTER 1  ** {0 B} {2 A}  0 [1]  len 1 > ( 5 )
           [0] > ind 5 indc 4 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A}  0 [2]  len 3 > > > ( 6 2 1 )
           [0] > ind 6 indc 5 w 0 c 0
           [1] > ind 2 indc 6 w 0 c 0
           [2] > ind 1 indc 0 w 0 c 0
*/

/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end B of a linear chain pulls end B of a linear source chain.
/// The source chain is consumed.
TEST_F(Pull_2, Junct3_LinDrvB_LinSrcB_SrcConsumed)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end B of a linear chain pulls end B of a linear ",
            "source chain. The source chain is consumed."
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

    const Driver drv {&gr.cn[w1].g[0], eB};
    const Source src {w2, eB};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 3);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[w2].length(), 1);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvB_LinSrcB_SrcConsumed

/*
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

Pulling from Vertex Deg 2+ :: 3 steps over path:

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [0] > ind 2 indc 4 w 2 c 0
                 2:   [1] > ind 3 indc 5 w 2 c 0
source (end B)   3:   [2] > ind 4 indc 6 w 2 c 0

TEST_AFTER 2 {1 B} {0 A}  **  0 [0]  len 1 > ( 6 )
           [0] > ind 6 indc 0 w 2 c 0
TEST_AFTER 1  ** {2 A} {0 A}  0 [1]  len 3 > > > ( 0 1 4 )
           [0] > ind 0 indc 1 w 1 c 0
           [1] > ind 1 indc 2 w 1 c 0
           [2] > ind 4 indc 3 w 1 c 0
TEST_AFTER 0 {2 A} {1 B}  **  0 [2]  len 3 > > > ( 3 2 5 )
           [0] > ind 3 indc 4 w 0 c 0
           [1] > ind 2 indc 5 w 0 c 0
           [2] > ind 5 indc 6 w 0 c 0
*/

/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end A of a linear chain pulls end B of a linear source chain.
/// The source chain is consumed.
TEST_F(Pull_2, Junct3_LinDrvA_LinSrcB_SrcConsumed)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end A of a linear chain pulls end B of a linear ",
            "source chain. The source chain is consumed."
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

    const Driver drv {&gr.cn[w0].g[0], eB};
    const Source src {w2, eB};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[w2].length(), 3);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvA_LinSrcB_SrcConsumed

/*
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

driver (egEnd A) 0:   [1] > ind 1 indc 3 w 0 c 0
                 1:   [0] > ind 2 indc 4 w 2 c 0
                 2:   [1] > ind 3 indc 5 w 2 c 0
source (end B)   3:   [2] > ind 4 indc 6 w 2 c 0

TEST_AFTER 0  ** {2 B} {1 A}  0 [0]  len 1 > ( 0 )
           [0] > ind 0 indc 0 w 0 c 0
TEST_AFTER 2  ** {0 B} {1 A}  0 [1]  len 3 > > > ( 1 2 3 )
           [0] > ind 1 indc 1 w 2 c 0
           [1] > ind 2 indc 2 w 2 c 0
           [2] > ind 3 indc 3 w 2 c 0
TEST_AFTER 1 {0 B} {2 B}  **  0 [2]  len 3 > > > ( 4 5 6 )
           [0] > ind 4 indc 4 w 1 c 0
           [1] > ind 5 indc 5 w 1 c 0
           [2] > ind 6 indc 6 w 1 c 0
*/

/// Tests degree 2 pulling over a path traversing a 3-way junction.
/// Driver edge end A of a linear chain pulls end B of a linear source chain.
/// The source chain is consumed.
TEST_F(Pull_2, Junct3_LinDrvB_LinSrcA_SrcConsumed)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction. ",
            "Driver edge end B of a linear chain pulls end A of a linear ",
            "source chain. The source chain is consumed."
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
            BSlot{w0, 3});     // creates w2

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w1].g[0], eB};
    const Source src {w0, eA};

    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[w2].length(), 3);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}  //  end Junct3_LinDrvB_LinSrcA_SrcConsumed
/*
TEST_BEFORE 1 {0 B} {2 A}  **  0 [0]  len 2 > > ( 5 6 )
            [0] > ind 5 indc 0 w 1 c 0
            [1] > ind 6 indc 1 w 1 c 0
TEST_BEFORE 0  ** {1 A} {2 A}  0 [1]  len 3 > > > ( 0 1 2 )
            [0] > ind 0 indc 2 w 0 c 0
            [1] > ind 1 indc 3 w 0 c 0
            [2] > ind 2 indc 4 w 0 c 0
TEST_BEFORE 2 {1 A} {0 B}  **  0 [2]  len 2 > > ( 3 4 )
            [0] > ind 3 indc 5 w 2 c 0
            [1] > ind 4 indc 6 w 2 c 0

Pulling from Vertex Deg 2+ :: 3 steps over path:

driver (egEnd B) 0:   [0] > ind 5 indc 0 w 1 c 0
                 1:   [2] > ind 2 indc 4 w 0 c 0
                 2:   [1] > ind 1 indc 3 w 0 c 0
source (end A)   3:   [0] > ind 0 indc 2 w 0 c 0

TEST_AFTER 0 {1 A} {2 A}  **  0 [0]  len 1 > ( 6 )
           [0] > ind 6 indc 0 w 0 c 0
TEST_AFTER 1 {0 A} {2 A}  **  0 [1]  len 3 < > > ( 0 3 4 )
           [0] < ind 0 indc 2 w 1 c 0
           [1] > ind 3 indc 3 w 1 c 0
           [2] > ind 4 indc 4 w 1 c 0
TEST_AFTER 2 {0 A} {1 A}  **  0 [2]  len 3 < > > ( 1 2 5 )
           [0] < ind 1 indc 1 w 2 c 0
           [1] > ind 2 indc 5 w 2 c 0
           [2] > ind 5 indc 6 w 2 c 0
*/

/*
/// Tests degree 2 pulling over a path traversing a 4-way junction at end A.
/// The source chain survives.
TEST_F(Pull_2, Deg4junctionA)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 4-way junction, ",
            "at end A. The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 4};
    const auto [w0, w1, w2, w3, w4, w5] = create_array<ChId, 6>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w3 : length (5) -> (2, 3)
    // w1, w2 : length (4) -> (1, 3)
    merge22(BSlot{w1, 1},
            BSlot{w0, 2});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w2].g[1], eA};
    const Source src {w3, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 1);
    ASSERT_EQ(gr.cn[w3].length(), 1);
    ASSERT_EQ(gr.cn[w4].length(), 2);
    ASSERT_EQ(gr.cn[w5].length(), 2);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}

TEST_AFTER 4 {2 B} {5 A}  **  0 [0]  len 2 > > ( 7 8 )
           [0] > ind 7 indc 0 w 4 c 0
           [1] > ind 8 indc 1 w 4 c 0
TEST_AFTER 1  ** {0 B} {2 A} {3 A}  0 [1]  len 1 > ( 5 )
           [0] > ind 5 indc 2 w 1 c 0
TEST_AFTER 0  ** {1 B} {2 A} {3 A}  0 [2]  len 2 > > ( 0 1 )
           [0] > ind 0 indc 3 w 0 c 0
           [1] > ind 1 indc 4 w 0 c 0
TEST_AFTER 2 {0 B} {1 B} {3 A}  ** {4 A} {5 A}  0 [3]  len 1 > ( 3 )
           [0] > ind 3 indc 6 w 2 c 0
TEST_AFTER 3 {0 B} {2 A} {1 B}  **  0 [4]  len 1 > ( 4 )
           [0] > ind 4 indc 7 w 3 c 0
TEST_AFTER 5 {4 A} {2 B}  **  0 [5]  len 2 > > ( 2 6 )
           [0] > ind 2 indc 5 w 5 c 0
           [1] > ind 6 indc 8 w 5 c 0


/// Tests degree 2 pulling over a path traversing a 4-way junction at end B.
/// The source chain survives.
TEST_F(Pull_2, Deg4junctionB)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 4-way junction, ",
            "at end B. The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 2> len {5, 4};
    const auto [w0, w1, w2, w3, w4, w5] = create_array<ChId, 6>();

    G gr;
    for (const auto u: len)
        gr.add_single_chain_component(u);

    VertexMerger<2, 2, G> merge22 {gr};
    // w0, w3 : length (5) -> (2, 3)
    // w1, w2 : length (4) -> (1, 3)
    merge22(BSlot{w1, 1},
            BSlot{w0, 2});     // creates w2, w3

    const auto gr0 = gr;  // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w0].g[0], eB};
    const Source src {w3, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 3);
    ASSERT_EQ(gr.cn[w3].length(), 1);
    ASSERT_EQ(gr.cn[w4].length(), 2);
    ASSERT_EQ(gr.cn[w5].length(), 1);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}


/// Tests degree 2 pulling over a path traversing a 3-way and
/// two 4-way junctions, with driver chain connected at end A.
/// The source chain survives.
TEST_F(Pull_2, Deg344junctionA)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way and ",
            "two 4-way junctions, with driver chain connected at end A. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 4> len {5, 7, 3, 3};
    const auto [w0, w1, w2, w3, w4, w5, w6, w7, w8, w9]
        = create_array<ChId, 10>();

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

    const auto gr0 = gr;  // copy the graph in its initial state   // copy the graph in its initial state

    if constexpr (withMaxVerbosity)
        gr.print_components(tagBefore);

    // Define a sequence of edges to pull.

    const Driver drv {&gr.cn[w5].g[1], eA};
    const Source src {w7, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 3);
    ASSERT_EQ(gr.cn[w3].length(), 3);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    ASSERT_EQ(gr.cn[w5].length(), 1);
    ASSERT_EQ(gr.cn[w6].length(), 2);
    ASSERT_EQ(gr.cn[w7].length(), 1);
    ASSERT_EQ(gr.cn[w8].length(), 2);
    ASSERT_EQ(gr.cn[w9].length(), 2);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}


/// Tests degree 2 pulling over a path traversing a 3-way and
/// two 4-way junctions, with driver chain connected at end B.
/// The source chain survives.
TEST_F(Pull_2, Deg344junctionB)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way and ",
            "two 4-way junctions, with driver chain connected at end B. ",
            "The source chain survives."
        );

    // Create initial graph.

    constexpr std::array<EgId, 4> len {5, 7, 3, 3};
    const auto [w0, w1, w2, w3, w4, w5, w6, w7, w8, w9]
        = create_array<ChId, 10>();

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

    const Driver drv {&gr.cn[w0].g[0], eB};
    const Source src {w7, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains() + 2);
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 1);
    ASSERT_EQ(gr.cn[w2].length(), 3);
    ASSERT_EQ(gr.cn[w3].length(), 3);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    ASSERT_EQ(gr.cn[w5].length(), 3);
    ASSERT_EQ(gr.cn[w6].length(), 2);
    ASSERT_EQ(gr.cn[w7].length(), 1);
    ASSERT_EQ(gr.cn[w8].length(), 2);
    ASSERT_EQ(gr.cn[w9].length(), 1);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}


/// Tests degree 2 pulling over a path traversing a 3-way junction,
/// source chain is engulfed completely.
TEST_F(Pull_2, Deg3junctionSrcRem)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction, ",
            "source chain is engulfed completely."
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

    const Driver drv {&gr.cn[w2].g[1], eA};
    const Source src {w1, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull single step.
    pu2f(pp, 1);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    const auto ic = pp.cmp->ind;
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
    ASSERT_EQ(gr.ct[ic].num_vertices(), gr0.ct[ic].num_vertices());
    ASSERT_EQ(gr.cn[w0].length(), 2);
    ASSERT_EQ(gr.cn[w1].length(), 3);
    ASSERT_EQ(gr.cn[w2].length(), 1);
    for(EgId i {}; const auto& h : gr.ct[ic].gl) {
        ASSERT_EQ(gr.cn[h.w].g[h.a].w, h.w);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indw, h.a);
        ASSERT_EQ(gr.cn[h.w].g[h.a].ind, h.i);
        ASSERT_EQ(gr.cn[h.w].g[h.a].c, ic);
        ASSERT_EQ(gr.cn[h.w].g[h.a].indc, i++);
    }
}


/// Tests degree 2 pulling over a path traversing a 3-way junction,
/// source chain is engulfed completely.
TEST_F(Pull_2, Deg3junctionSrcRem_X)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way junction, ",
            "source chain is engulfed completely."
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

    const Driver drv {&gr.cn[w1].g[0], eB};
    const Source src {w0, eA};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull two steps.
    pu2f(pp, 2);

    if constexpr (withMaxVerbosity)
        gr.print_components(tagAfter);

    // Compare the result to the expectation.

    ASSERT_EQ(gr.ct.size(), 1);
    ASSERT_EQ(gr.ct[0].num_chains(), 3);
    ASSERT_EQ(gr.ct[0].num_edges(), 5);
    ASSERT_EQ(gr.cn[w0].length(), 1);
    ASSERT_EQ(gr.cn[w1].length(), 2);
    ASSERT_EQ(gr.cn[w2].length(), 2);
    for (ChId i {}; const auto& cn: gr.cn) {
        ASSERT_EQ(cn.idw, i);
        ASSERT_EQ(cn.idc, i);
        ASSERT_EQ(cn.c, 0);
        ++i;
    }
    constexpr std::array<EgId, 5> inds {4, 3, 0, 1, 2};
    for (EgId i {}; const auto& eg: gr.cn[w0].g) {
        ASSERT_EQ(eg.ind, inds[i]);
        ASSERT_EQ(eg.indc, i);
        ASSERT_EQ(eg.indw, i);
        ASSERT_EQ(eg.c, 0);
        ASSERT_EQ(eg.w, w0);
        ++i;
    }
}


/// Tests degree 2 pulling over a path traversing a 3-way and
/// two 4-way junctions, source chain is engulfed completely.
TEST_F(Pull_2, Deg344junctionSrcRem)
{
    ++testCount;

    if constexpr (verboseT)
        print_description(
            "Tests degree 2 pulling over a path traversing a 3-way and ",
            "two 4-way junctions, source chain is engulfed completely."
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

    const Driver drv {&gr.cn[w5].g[2], eA};
    const Source src {w7, eB};
    Path pp {&gr.ct[gr.cn[src.w].c], drv, src};

    const auto ic = pp.cmp->ind;

    // Do the transformation.

    Pulling<2, Orientation::Forwards, G> pu2f {gr};

    // Pull three steps.
    pu2f(pp, 3);

    if constexpr (withMaxVerbosity) {
        gr.print_components(tagAfter);
        pp.print_detailed(tagAfter);
    }

    // Compare the result to the expectation.

    const auto& p = pp.pth;

    ASSERT_EQ(pp.length(), 11);
    ASSERT_EQ(gr.cn[w3].length(), 5);
    ASSERT_EQ(gr.cn[w4].length(), 1);
    ASSERT_EQ(gr.cn[w5].length(), 2);
    ASSERT_EQ(gr.cn[w6].length(), 3);
    ASSERT_EQ(gr.edge(p[0]).ind, gr0.edge(p[0]).ind);
    ASSERT_EQ(gr.edge(p[0]).c, ic);
    ASSERT_EQ(gr.edge(p[0]).w, w6);
    ASSERT_EQ(gr.edge(p[0]).indw, 2);
    ASSERT_EQ(gr.edge(p[1]).ind, gr0.edge(p[1]).ind);
    ASSERT_EQ(gr.edge(p[1]).c, ic);
    ASSERT_EQ(gr.edge(p[1]).w, w6);
    ASSERT_EQ(gr.edge(p[1]).indw, 1);
    ASSERT_EQ(gr.edge(p[2]).ind, gr0.edge(p[2]).ind);
    ASSERT_EQ(gr.edge(p[2]).c, ic);
    ASSERT_EQ(gr.edge(p[2]).w, w6);
    ASSERT_EQ(gr.edge(p[2]).indw, 0);
    ASSERT_EQ(gr.edge(p[3]).ind, gr0.edge(p[3]).ind);
    ASSERT_EQ(gr.edge(p[3]).c, ic);
    ASSERT_EQ(gr.edge(p[3]).w, w5);
    ASSERT_EQ(gr.edge(p[3]).indw, 1);
    ASSERT_EQ(gr.edge(p[4]).ind, gr0.edge(p[4]).ind);
    ASSERT_EQ(gr.edge(p[4]).c, ic);
    ASSERT_EQ(gr.edge(p[4]).w, w5);
    ASSERT_EQ(gr.edge(p[4]).indw, 0);
    ASSERT_EQ(gr.edge(p[5]).ind, gr0.edge(p[5]).ind);
    ASSERT_EQ(gr.edge(p[5]).c, ic);
    ASSERT_EQ(gr.edge(p[5]).w, w4);
    ASSERT_EQ(gr.edge(p[5]).indw, 0);
    ASSERT_EQ(gr.edge(p[6]).ind, gr0.edge(p[6]).ind);
    ASSERT_EQ(gr.edge(p[6]).c, ic);
    ASSERT_EQ(gr.edge(p[6]).w, w3);
    ASSERT_EQ(gr.edge(p[6]).indw, 0);
    ASSERT_EQ(gr.edge(p[7]).ind, gr0.edge(p[7]).ind);
    ASSERT_EQ(gr.edge(p[7]).c, ic);
    ASSERT_EQ(gr.edge(p[7]).w, w3);
    ASSERT_EQ(gr.edge(p[7]).indw, 1);
    ASSERT_EQ(gr.edge(p[8]).ind, gr0.cn[w3].g[0].ind);
    ASSERT_EQ(gr.edge(p[8]).c, ic);
    ASSERT_EQ(gr.edge(p[8]).w, w3);
    ASSERT_EQ(gr.edge(p[8]).indw, 2);
    ASSERT_EQ(gr.edge(p[9]).ind, gr0.cn[w3].g[1].ind);
    ASSERT_EQ(gr.edge(p[9]).c, ic);
    ASSERT_EQ(gr.edge(p[9]).w, w3);
    ASSERT_EQ(gr.edge(p[9]).indw, 3);
    ASSERT_EQ(gr.edge(p[10]).ind, gr0.cn[w3].g[2].ind);
    ASSERT_EQ(gr.edge(p[10]).c, ic);
    ASSERT_EQ(gr.edge(p[10]).w, w3);
    ASSERT_EQ(gr.edge(p[10]).indw, 4);
    ASSERT_EQ(gr.ct[ic].num_edges(), gr0.ct[ic].num_edges());
    ASSERT_EQ(gr.ct[ic].num_chains(), gr0.ct[ic].num_chains());
}
*/


}  // namespace graph_mutator::tests::pulling::d3