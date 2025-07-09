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
 * @file functor_1.h
 * @brief Struct performing deletion of graph edges along with host chain.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_EDGE_DELETION_FUNCTOR_1_H
#define GRAPH_MUTATOR_EDGE_DELETION_FUNCTOR_1_H

#include "../../definitions.h"
#include "../../structure/vertices/degrees.h"
#include "../../structure/graph.h"
#include "../../to_string.h"
#include "../component_deletion/functor.h"
#include "../vertex_split/functor_1B.h"
#include "../vertex_split/functor_13.h"
#include "common.h"



/**
 * @brief Contains utilities for handling edge deletion operations in a graph.
 * These are functors for deleting edges based on chain lengths and vertex
 * degrees.
 */
namespace graph_mutator::edge_deletion {

/**
 * @brief Deletion of graph edges along with their host chain.
 * Handles the case of single-edge chains having single unconnected end.
 * @tparam D Vertex degree at the connected end.
 * @tparam Graph hosting the edge to be deleted.
 */
template<Degree D,
         typename G>
struct Functor<1, D, G> {

    static_assert(structure::vertices::is_junction_degree<D>);

    static_assert(std::is_base_of_v<structure::GraphBase, G>);

    using Graph = G;
    using Chain = G::Chain;
    using EndSlot = Chain::EndSlot;
    using BulkSlot = Chain::BulkSlot;
    using ResT = CmpId;
    using Res = std::array<ResT, 1>;

    static constexpr auto withChain = true;

    /// This is a single-edge chain, so one of the edge ends has degree 1.
    static constexpr auto I1 = static_cast<Degree>(1);
    /// This is a single-edge connected chain, so one of the edge ends is a (3-way or 4-way) junction.
    static constexpr auto I2 = D;

    static constexpr auto J1 = D == 3 ? static_cast<Degree>(0)
                                      : undefined<Degree>;
    static constexpr auto J2 = I2 - 1;

    static constexpr auto dd = str2<I1, I2>;
    static constexpr auto shortName = concat<shortNameStem, dd, 2>;
    static constexpr auto fullName  = concat<fullNameStem, dd, 2>;

    /**
     * @brief Constructs a Functor object based on the Graph instance.
     * @param gr Graph on which the transformations operate.
     */
    explicit Functor(G& gr);

    /**
     * @brief Function call operator executing the deletion using edge index.
     * @param ind Graph-wide index of the deleted edge.
     */
    auto operator()(EgId ind) noexcept -> Res;

    /**
     * @brief Function call operator executing the deletion using edge position.
     * @param s End slot connecting the deleted edge.
     */
    auto operator()(const EndSlot& s) noexcept -> Res;

protected:

    G& gr;  ///< Reference to the graph object.

    // Auxiliary functors.
    component_deletion::Functor<G> clrem;          ///< Component deletion.

    vertex_split::Functor<1, D-1, G> disconnectD;  ///< Vertex split (1, D-1).
    vertex_split::Functor<1, 0, G> disconnect0;    ///< Vertex split (1, 0).
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<Degree D,
         typename G>
Functor<1, D, G>::
Functor(G& gr)
    : gr {gr}
    , clrem {gr}
    , disconnectD {gr}
    , disconnect0 {gr}
{}


template<Degree D,
         typename G>
auto Functor<1, D, G>::
operator()(const EndSlot& s) noexcept -> Res
{
    const auto [w, e] = s.we();

    auto& m = gr.cn[w];
    const auto c = m.c;
    const auto& ngs = gr.ngs_at(s);
    const auto ind = m.end_edge(e).ind;

    if constexpr (verboseF) {
        log_(colorcodes::GREEN, "Edge_deletion ", dd, ": ",
                   colorcodes::BOLDYELLOW, w, " ", s.ea_str(),
                   colorcodes::RESET, '\n');
        m.print("before ", shortName, " ", s.ea_str());
        for (const auto& ng : ngs())
            gr.cn[ng.w].print("              ", ng.ea_str());

        log_("");
    }

    ASSERT(w < gr.chain_num(),
           "w ", w, " exceeds number of chains ", gr.chain_num());
    ASSERT(m.length() == 1, "inncorrect functor for edge deletion: length > 1");
    ASSERT(m.is_shrinkable(), "chain ", w, " is not shrinkable.");
    ASSERT(m.has_one_free_end(),
            "chain ", w, " does not a have single connected end");
    ASSERT(ngs.num() == D - 1, "slot ", s.w, " ", s.ea_str(),
           " has incorrect degree ", ngs.num() + 1, " != D ", D);


    // There is a connected end because othrwise a component deletion
    // rather than edge deletion should have been active.
    // The connected end is single because if both ends were connected,
    // edge removal would amount to chain removal and induce topology
    // change by vertex merger on the chain ends.

    const auto cc = D == 3 && gr.cn[ngs[0].w].is_connected_cycle()
                  ? disconnect0(s)
                  : disconnectD(s);

    // index of the disconnected component
    const auto cr = gr.ct[cc[0]].gl[0].i == ind  ? cc[0] : cc[1];

    ASSERT(gr.ct[cr].num_edges() == 1, "component to remove is too big");

    clrem(cr);    // calls gr.update() inside

    if constexpr (verboseF)
        log_(shortName, " ends with success \n");

    return {c};
}


template<Degree D,
         typename G>
auto Functor<1, D, G>::
operator()(const EgId ind) noexcept -> Res
{
    return (*this)(gr.ind2bslot(ind));
}


}  // namespace graph_mutator::edge_deletion

#endif // GRAPH_MUTATOR_EDGE_DELETION_FUNCTOR_1_H
