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
 * @file functor_00.h
 * @brief Contains class managing merger of two boundary vertices of a cycle chain.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_VERTEX_MERGER_FUNCTOR_00_H
#define GRAPH_MUTATOR_VERTEX_MERGER_FUNCTOR_00_H

#include <array>
#include <vector>

#include "../../definitions.h"
#include "../../structure/vertices/degrees.h"
#include "../../to_string.h"
#include "../vertex_split/functor_11.h"
#include "common.h"
#include "log.h"


namespace graph_mutator::vertex_merger {

template<typename> class Reaction;

/**
 * @brief Class encapsulating merger of 2 cycle chains on their boundary vertices.
 * @details Adds vertex type-specific vertex merger capability and updates the
 * graph for it.
 * @tparam G Graph class on which operator() acts.
 */
template<typename G>
struct Functor<0, 0, G> {

    static_assert(std::is_base_of_v<graph_mutator::structure::GraphBase, G>);

    static constexpr auto I1 = static_cast<Degree>(0);  ///< Degree of the 1st input vertex.
    static constexpr auto I2 = static_cast<Degree>(0);  ///< Degree of the 2nd input vertex.
    static constexpr auto J1 = static_cast<Degree>(4);  ///< Degree of the 1st output vertex.
    static constexpr auto J2 = undefined<Degree>;       ///< No 2nd output vertex.

    static constexpr auto dd = str2<I1, I2>;
    static constexpr auto shortName = graph_mutator::concat<shortNameStem, dd, 2>;
    static constexpr auto fullName  = graph_mutator::concat<fullNameStem, dd, 2>;

    using Graph = G;
    using Chain = G::Chain;
    using Ends = Chain::Ends;
    using EndSlot = Chain::EndSlot;
    using BulkSlot = Chain::BulkSlot;
    using Neigs = Chain::Neigs;
    using ResT = CmpId;
    using Res = std::array<ResT, 2>;

    /**
     * @brief Constructs a Functor object based on the Graph instance.
     * @param gr Graph on which the transformations operate.
     */
    explicit Functor(G& gr);

    /**
     * Merges two disconnected cycles on their boundaty vertices.
     * @param w1 Chain index of the 1st cycle chain.
     * @param w2 Chain index of the 2nd cycle chain.
     */
    auto operator()(ChId w1, ChId w2) noexcept -> Res;

private:

    G& gr;  ///< Reference to the graph object.

    G::Chains& cn;  ///< Reference to the graph edge chains.

    ///< Auxiliary functor producing a splitted intermediary.
    vertex_split::Functor<1, 1, G> split_to11;

//    Core<G> merge;  ///< Low-level free end connector.

    Log<G> log;
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<typename G>
Functor<0, 0, G>::
Functor(G& gr)
    : gr {gr}
    , cn {gr.cn}
    , split_to11 {gr}
//    , merge {gr, shortName}
    , log {dd, gr, "a disconnected CYCLE ", "a disconnected CYCLE "}
{}


template<typename G>
auto Functor<0, 0, G>::
operator()(
    const ChId w1,
    const ChId w2
) noexcept -> Res
{
    // Indexes of the participating components:

    const auto c1 = cn[w1].c;
    const auto c2 = cn[w2].c;

    // Print out summary before the operation.

    if constexpr (verboseF)
        log.before(EndSlot{w1, Ends::Undefined},
                   EndSlot{w2, Ends::Undefined});

    ASSERT(cn[w1].is_disconnected_cycle(),
           shortName, ": w1 ", w1, " is not a disconnected cycle");
    ASSERT(cn[w2].is_disconnected_cycle(),
           shortName, ": w2 ", w2, " is not a disconnected cycle");

    // Produce a disconnected intermediary:

    split_to11(BulkSlot{w1, 0});    // produces: cn[w1] with both ends free
    split_to11(BulkSlot{w2, 0});    // produces: cn[w2] with both ends free

    // Connect slots at free ends:

    const EndSlot a1 {w1, Ends::A};
    const EndSlot b1 {w1, Ends::B};
    const EndSlot a2 {w2, Ends::A};
    const EndSlot b2 {w2, Ends::B};

    gr.ngs_at(a1) = Neigs{b1, a2, b2};
    gr.ngs_at(b1) = Neigs{a1, a2, b2};
    gr.ngs_at(a2) = Neigs{b1, a1, b2};
    gr.ngs_at(b2) = Neigs{b1, a2, a1};

    // Update the graph components:

    gr.merge_components(c1, c2);

    gr.update();

    // Print out summary after the operation.

    if constexpr (verboseF)
        log.after(w1, {w2});

    return {c1, c2};
}


}  // namespace graph_mutator::vertex_merger

#endif  // GRAPH_MUTATOR_VERTEX_MERGER_FUNCTOR_00_H
