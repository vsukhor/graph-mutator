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
 * @file common.h
 * @brief Generic template for performing graph edge deletions.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_EDGE_DELETION_COMMON_H
#define GRAPH_MUTATOR_EDGE_DELETION_COMMON_H

#include <array>

#include "../../definitions.h"
#include "../../structure/graph.h"
#include "../../structure/vertices/degrees.h"

/**
 * @brief Contains utilities for handling edge deletion operations in a graph.
 * These are functors for deleting edges based on chain lengths and vertex
 * degrees.
 */
namespace graph_mutator::edge_deletion {

/**
 * @brief Short name stem for edge deletion functors.
 * Used for naming conventions in the code.
 */
constexpr std::array shortNameStem {'e', '_', 'd', '_'};

/**
 * @brief Full name stem for edge deletion functors.
 * Used for more descriptive naming conventions in the code.
 */
constexpr std::array fullNameStem
   {'e', 'd', 'g', 'e', '_', 'd', 'e', 'l', 'e', 't', 'i', 'o', 'n', '_'};

/**
 * @brief Functor template for performing deletion of a specific graph edge.
 * This template is specialized for different vertex degrees.
 * @tparam D1 Degree of the vertex at one end of the edge to be deleted.
 * @tparam D2 Degree of the vertex at the other end of the edge to be deleted.
 * @tparam G Graph class.
 */
template<Degree D1,
         Degree D2,
         typename G> requires (structure::vertices::is_implemented_degree<D1> &&
                               structure::vertices::is_implemented_degree<D2>)
struct Functor
{};


}  // namespace graph_mutator::edge_deletion

#endif  // GRAPH_MUTATOR_EDGE_DELETION_COMMON_H
