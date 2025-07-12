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
 * @brief Contains classes controling vertex split transformation.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_VERTEX_SPLIT_COMMON_H
#define GRAPH_MUTATOR_VERTEX_SPLIT_COMMON_H

#include <array>

#include "../../definitions.h"


namespace graph_mutator::vertex_split {


constexpr std::array fullNameStem
   {'v', 'e', 'r', 't', 'e', 'x', '_', 's', 'p', 'l', 'i', 't', '_'};

constexpr std::array shortNameStem {'v', '_', 's', '_'};

/**
 * @brief Template specialization for vertex split functor.
 * @details This class template is specialized for specific vertex degrees
 * and provides the functionality to split a vertex into two vertices of
 * specified degrees.
 * @tparam D1 Degree of the 1st resulting vertex.
 * @tparam D2 Degree of the 2nd resulting vertex.
 * @tparam G Graph type on which the split operation is performed.
 */
template<Degree D1,
         Degree D2,
         typename G>
class To
{};

}  // namespace graph_mutator::vertex_split

#endif  // GRAPH_MUTATOR_VERTEX_SPLIT_COMMON_H
