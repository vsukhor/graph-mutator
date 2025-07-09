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
 * @file functor_new_chain.h
 * @brief Contains class performing creation of graph edges in new chain.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_EDGE_CREATION_FUNCTOR_NEW_CHAIN_H
#define GRAPH_MUTATOR_EDGE_CREATION_FUNCTOR_NEW_CHAIN_H

#include "../../definitions.h"
#include "../../structure/graph.h"
#include "../../structure/vertices/degrees.h"
#include "../../to_string.h"
#include "../component_creation/functor.h"
#include "../vertex_merger/functor_10.h"
#include "../vertex_merger/functor_11.h"
#include "../vertex_merger/functor_12.h"
#include "../vertex_merger/functor_13.h"


namespace graph_mutator::edge_creation {


/**
 * @brief Creates an edge adjoint to a vertex, branching from it a new chain.
 * @details Creates a graph edge together with its host chain. The edge will
 * have as one of its ends a vertex whose initial degree \p D will be
 * incremented by addition of the new chain branching from it. Because the new
 * chain has length 1, the other vertex of the new edge has degree 1.
 * @tparam D Degree of one of the vertexes accepting the created edge.
 * @tparam G Graph class.
 */
template<Degree D,
         typename G>
struct FunctorNewChain {

    static_assert(std::is_base_of_v<graph_mutator::structure::GraphBase, G>);
    static_assert (structure::vertices::is_bulk_degree<D> || D == 3);

    static constexpr auto isNewChain = true;

    static constexpr auto I1 = undefined<Degree>;
    static constexpr auto I2 = D;
    static constexpr auto J1 = static_cast<Degree>(1);
    static constexpr auto J2 = D == 0
                             ? static_cast<Degree>(3)
                             : I2 + static_cast<Degree>(1);

    static constexpr auto d {str1<I2>};
    static constexpr auto shortName {graph_mutator::shName<d, 1, 'e', 'c', '_', 'n'>};
    static constexpr const char* fullName {"edge_creation_new_chain"};

    using Graph = G;
    using Edge = G::Edge;
    using Chain = G::Chain;
    using Ends = Chain::Ends;
    using EndSlot = Chain::EndSlot;
    using BulkSlot = Chain::BulkSlot;
    using ResT = CmpId;
    using Res = std::array<ResT, 1>;
    using S = std::conditional_t<D == 0U, ChId,
                                          std::conditional_t<D == 2U, BulkSlot,
                                                                      EndSlot>>;

    /**
     * @brief Constructs a Functor object based on the Graph instance.
     * @param gr Graph on which the transformations operate.
     */
    explicit FunctorNewChain(G& gr);

    /**
     * @brief Creates new edge packed into a single-edge chain.
     * @details Depending on the value of \p D the source vertex may be either
     * a boundary vertex of a disconnected chain (D == 0), a chain bulk vertex
     * (D == 2) or a three-way junction (D == 3). Depending on the case, type
     * of the source S corresponds to the argument of the functor of the
     * vertex_merger functor applied to connect the new chain to the source
     * vertex.
     * @tparam S Type of the source: ChId (D == 0), BulkSlot (D == 2) or
     * EndSlot (D == 3).
     * @param s Slot/index of the source vertex.
     */
    auto operator()(const S s) noexcept -> Res;

protected:

    G& gr;  ///< Reference to the graph object.
    decltype(gr.cn)& cn;

    vertex_merger::Functor<1, I2, G> merge_vertexes;

private:

    void check_slot(const S s) const;

    constexpr auto chain_ind(const S s) const noexcept -> ChId;

    auto pos_str(const S s) const noexcept -> std::string;
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<Degree D,
         typename G>
FunctorNewChain<D, G>::
FunctorNewChain(G& gr)
    : gr {gr}
    , cn {gr.cn}
    , merge_vertexes {gr}
{}


// Creates new single-edge chain attached to the original chain.
template<Degree D,
         typename G>
auto FunctorNewChain<D, G>::
operator()(const S s) noexcept -> Res
{
    const auto w = chain_ind(s);
    const auto at = pos_str(s);

    if constexpr (verboseF) {
        log_(
            colorcodes::GREEN, "Edge creation ", J1, I2, colorcodes::RESET,
            " with new chain: ", colorcodes::BOLDYELLOW,  w, at,
                   colorcodes::RESET, '\n');
        cn[w].print("before ", shortName);
        if constexpr (D == 3)
            for (const auto u : gr.ngs_at(s)())
                cn[u.w].print("                  ");
        log_("");
    }

    check_slot(s);

    gr.add_single_chain_component(1);

    constexpr auto e = Ends::A;

    merge_vertexes(EndSlot{cn.back().idw, e}, s);

    const auto u = gr.glm[gr.edgenum-1];

    if constexpr (verboseF) {
        cn[u].print(shortName, " produces");
        const auto ngs = gr.ngs_at(EndSlot{u, e})();
        for (const auto& ng : ngs)
            cn[ng.w].print("           and");
        log_("");
    }

    return {cn[u].c};
}


template<Degree D,
         typename G>
void FunctorNewChain<D, G>::
check_slot(const S s) const
{
    const auto w = chain_ind(s);

    if constexpr (D == 0)

        // The new edge is attached to a boundary vertex of a disconnected cycle.
        ASSERT(cn[w].is_disconnected_cycle(),
               "chain", w, " is not a disconnected cycle.");

    else if constexpr (D == 2)

        // The new edge is attached to a three-way junction.
        ASSERT(s.a() && s.a() < cn[w].length(),
               "a ", s.a(), "is not at a bulk edge of chain ", w);

    else if constexpr (D == 3)

        // The new edge is attached to a four-way junction.
        ASSERT(gr.ngs_at(s).num() == D - 1,
               "num of connections ", gr.ngs_at(s).num(), " at end ", s.ea_str(),
               " of chain ", s.w, " should be ", D - 1);

    else

        static_assert(false);
}


template<Degree D,
         typename G>
constexpr
auto FunctorNewChain<D, G>::
chain_ind(const S s) const noexcept -> ChId
{
    if constexpr (std::is_same_v<S, ChId>)
        return s;

    else if constexpr (structure::slot<S>)

        return s.w;

    else
        static_assert(false);

    return undefined<ChId>;
}


template<Degree D,
         typename G>
auto FunctorNewChain<D, G>::
pos_str(const S s) const noexcept -> std::string
{
    if constexpr (D == 0)
        return ": disconnected cycle";
    else if constexpr (D == 2)
        return " at "s + s.ea_str();
    else if constexpr (D == 3)
        return " "s + s.ea_str();
    else
        static_assert(false);

    return {};
}


}  // namespace graph_mutator::edge_creation

#endif  // GRAPH_MUTATOR_EDGE_CREATION_FUNCTOR_NEW_CHAIN_H
