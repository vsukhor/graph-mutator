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

/**
 * @file functor_13.h
 * @brief Class performing split of graph components.
 * @details Te split is done at vertices of specific degrees.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_VERTEX_SPLIT_FUNCTOR_13_H
#define GRAPH_MUTATOR_VERTEX_SPLIT_FUNCTOR_13_H

#include <array>

#include "../../definitions.h"
#include "../../structure/vertices/degrees.h"
#include "common.h"
#include "log.h"


namespace graph_mutator::vertex_split {

/**
 * @brief Template specialization for a functor generating degree 1 and 3 vertices.
 * @details Adds vertex type-specific split capability and updates the
 * graph for it. This Functor template specialization operates on degree 4
 * vertices to produce vertices of degree 1 and 3: V4 -> V1 + V3.
 * @tparam G Graph class on which it operates.
 */
template<typename G>
struct Functor<1, 3, G> {

    static_assert(std::is_base_of_v<graph_mutator::structure::GraphBase, G>);

    static constexpr auto J1 = static_cast<Degree>(1);  ///< Degree of the 1st output vertex.
    static constexpr auto J2 = static_cast<Degree>(3);  ///< Degree of the 2nd output vertex.
    static constexpr auto I1 = J1 + J2;            ///< Degree of the input vertex.
    static constexpr auto I2 = undefined<Degree>;  ///< No 2nd input vertex.
    static constexpr auto I = I1;                  ///< Input vertex degree.

    static constexpr auto dd = str2<J1, J2>;
    static constexpr auto shortName = graph_mutator::concat<shortNameStem, dd, 2>;
    static constexpr auto fullName  = graph_mutator::concat<fullNameStem, dd, 2>;

    using Graph = G;
    using Chain = G::Chain;
    using Ends = Chain::Ends;
    using EndSlot = Chain::EndSlot;
    using BulkSlot = Chain::BulkSlot;
    using ResT = CmpId;
    using Res = std::array<ResT, 2>;

    /**
     * @brief Constructs a Functor object based on the Graph instance.
     * @param gr Graph on which the transformations operate.
     */
    explicit Functor(Graph& gr);

    /**
     * @brief Divides the chain at a vertex of degree 4 to 1+3.
     * @param w Global chain index.
     * @param end The chain end.
     */
    auto operator()(const EndSlot& s) -> Res;

private:

    Graph& gr;  ///< Reference to the graph object.

    Graph::Chains& cn;  ///< Reference to the graph edge chains.

    Log<Graph> log;
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<typename G>
Functor<1, 3, G>::
Functor(Graph& gr)
    : gr {gr}
    , cn {gr.cn}
    , log {dd, gr}
{}


// cuts a 4-junction
template<typename G>
auto Functor<1, 3, G>::
operator()(const EndSlot& s) -> Res
{
    const auto [w, end] = s.we();

    ASSERT(gr.ngs_at(s).num() == I - 1,
           shortName, ": number of connections ", gr.ngs_at(s).num(),
           " at slot ", s.w, " ", s.ea_str(), " is not as expected ", I - 1);

    if constexpr (verboseF)
        log.before(s);

    const auto clini = cn[w].c;

    const auto& ngs = gr.ngs_at(s);

    std::array<EgId, I> ind;
    ind[0] = gr.slot2ind(s);
    for (szt i=1; i<I; ++i)
        ind[i] = gr.slot2ind(ngs[i-1]);

    // If not a loop, this forms a new component
    // from w's end 1 neigs and beyond (excluding w itself):
    const auto isCycle = gr.cut_component_at(s);

    gr.remove_slot_from_neigs(s);

    gr.ct[clini].set_chis();
    if (!isCycle)
        gr.ct.back().set_chis();

    gr.update_books();
    if constexpr (G::useAgl)
        for (const auto ii: ind)
            gr.update_adjacency_edges(ii);

    const auto w1 = gr.glm[ind[0]];
    const auto w2 = gr.glm[ind[1]];

    ASSERT(cn[w1].c == clini || cn[w2].c == clini,
           shortName, ": cn[w1].c ", cn[w1].c, " != clini && cn[w2].c ",
           cn[w2].c, " != clini ", clini);

    // Print out summary after:

    if constexpr (verboseF)
        log.after(w1, {ind[1], ind[2], ind[3]});

    return {cn[w1].c,
            cn[w2].c};
}


}  // namespace graph_mutator::vertex_split

#endif  // GRAPH_MUTATOR_VERTEX_SPLIT_FUNCTOR_13_H
