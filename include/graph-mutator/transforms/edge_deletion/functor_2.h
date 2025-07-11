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
 * @file functor_2.h
 * @brief Contains classes performing deletion of specific graph edges.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_EDGE_DELETION_FUNCTOR_2_H
#define GRAPH_MUTATOR_EDGE_DELETION_FUNCTOR_2_H

#include "../../definitions.h"
#include "../../structure/graph.h"
#include "../../structure/vertices/degrees.h"
#include "../../to_string.h"
#include "common.h"


/**
 * @brief Contains utilities for handling edge deletion operations in a graph.
 * These are functors for deleting edges based on chain lengths and vertex
 * degrees.
 */
namespace graph_mutator::edge_deletion {


/**
 * @brief Deletion of graph edges without deleting their host chain.
 * Handles the case of multi-edge chains.
 * @tparam D Vertex degree at the connected end.
 * @tparam Graph hosting the edge to be deleted.
 */
template<Degree D,
         typename G>
struct Functor<2, D, G> {

    static_assert(std::is_base_of_v<graph_mutator::structure::GraphBase, G>);
    static_assert(structure::vertices::is_implemented_degree<D>);

    using Graph = G;
    using Chain = G::Chain;
    using Ends = Chain::Ends;
    using BulkSlot = Chain::BulkSlot;
    using ResT = CmpId;
    using Res = std::array<ResT, 1>;

    static constexpr auto withChain = false;

    // This is a multi-edge chain, so one of the edge ends has degree 2.
    static constexpr auto I1 = static_cast<Degree>(2);
    /// This is a multi-edge chain, so one of the edge ends has arbitrary degree.
    static constexpr auto I2 = D;

    static constexpr auto J1 = undefined<Degree>;
    static constexpr auto J2 = undefined<Degree>;

    static constexpr auto dd = str2<I1, I2>;
    static constexpr auto shortName = graph_mutator::concat<shortNameStem, dd, 2>;
    static constexpr auto fullName  = graph_mutator::concat<fullNameStem, dd, 2>;

    /**
     * @brief Constructs a Functor object based on the Graph instance.
     * @param gr Graph on which the transformations operate.
     */
    explicit Functor(Graph& gr);

    /**
     * @brief Function call operator executing the deletion using edge index.
     * @param ind Graph-wide index of the deleted edge.
     */
    auto operator()(EgId ind) noexcept -> Res;

    /**
     * @brief Function call operator executing the deletion using edge position.
     * @param s Slot in the chain internal.
     */
    auto operator()(const BulkSlot& s) noexcept -> Res;

protected:

    Graph& gr;  ///< Reference to the graph object.
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<Degree D,
         typename G>
Functor<2, D, G>::
Functor(Graph& gr)
    : gr {gr}
{}


template<Degree D,
         typename G>
auto Functor<2, D, G>::
operator()(const BulkSlot& s) noexcept -> Res
{
    const auto [w, a] = s.we();

    auto& m = gr.cn[w];

    if constexpr (verboseF) {
        log_(colorcodes::GREEN, "Edge_deletion ", dd, ": ",
                   colorcodes::BOLDYELLOW, w, " at ", a, colorcodes::RESET, '\n');
        m.print("before ed at ", s.ea_str());
        log_("");
    }

    ASSERT(w < gr.chain_num(),
           shortName, ": w ", w, "exceeds number of chains ", gr.chain_num());
    ASSERT(a < m.length(), shortName,
           ": a ", a, " exceeds chain length ", m.length(), "of chain ", w);
    ASSERT(m.is_shrinkable(), shortName, ": chain ", w, "is not shrinkable");
    ASSERT(
        (D == 2 && !m.is_tail(a) && !m.is_head(a)) ||
        (!D && (m.is_tail(a) || m.is_head(a)) && m.is_disconnected_cycle()) ||
        (D && D != 2 && ((m.is_tail(a) && m.ngs[Ends::A].num() == D-1) ||
                         (m.is_head(a)  && m.ngs[Ends::B].num() == D-1))),
        shortName, ": incorrect a ", a, " for chain ", w
    );

    const auto c = m.c;
    const auto ind = m.g[a].ind;

    // topology is not affected because the chain is not destroyed

    if (const auto& b = gr.ct[c].gl.back();
        m.g[a].ind != b.i)
        gr.cn[b.w].g[b.a].indc = m.g[a].indc;

    if (ind < gr.edgenum-1) {
        auto& elast = gr.edge(gr.edgenum-1);
        elast.ind = ind;
        gr.ct[elast.c].set_gl();
    }

    gr.edgenum--;
    m.g.erase(m.g.begin() + static_cast<long>(a));
    m.set_g_w();

    gr.ct[c].set_gl();

    gr.update();

    if constexpr (verboseF) {
        m.print(shortName, " produces");
        log_("");
    }

    return {c};
}


template<Degree D,
         typename G>
auto Functor<2, D, G>::
operator()(const EgId ind) noexcept -> Res
{
    return (*this)(gr.ind2bslot(ind));
}



}  // namespace graph_mutator::edge_deletion

#endif // GRAPH_MUTATOR_EDGE_DELETION_FUNCTOR_2_H
