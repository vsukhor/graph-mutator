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
 * @brief Contains class templates for degree-specific vertex merger.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_VERTEX_MERGER_COMMON_H
#define GRAPH_MUTATOR_VERTEX_MERGER_COMMON_H

#include <array>

#include "../../definitions.h"


namespace graph_mutator::vertex_merger {

constexpr std::array fullNameStem
   {'v','e','r','t','e','x','_','m','e','r','g','e','r'};
constexpr std::array shortNameStem {'v', '_', 'm', '_'};


/**
 * @brief Generic template for vertex degree-specific merger executing classes.
 * @tparam I1 Degree of the 1st input vertex.
 * @tparam I2 Degree of the 2nd input vertex.
 * @tparam G Graph to which the transformation is applied.
 */
template<Degree I1,
         Degree I2,
         typename G> requires are_compatible_degrees<I1, I2>
struct From {};


}  // namespace graph_mutator::vertex_merger

#endif  // GRAPH_MUTATOR_VERTEX_MERGER_COMMON_H
