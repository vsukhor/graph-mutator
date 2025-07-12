#ifndef GRAPH_MUTATOR_STRUCTURE_VERTICES_DEGREES_H
#define GRAPH_MUTATOR_STRUCTURE_VERTICES_DEGREES_H

#include <array>
#include <algorithm>  // for std::find

// does not require definitions.h

/**
 * @brief Contains definitions and utilities for vertex degrees in the graph.
 * @details Defines the maximal degree, checks for implemented degrees,
 * and categorizes degrees into bulk, leaf, junction, and end degrees.
 */
namespace graph_mutator {
namespace structure::vertices::degrees{

using Degree = unsigned int;

constexpr Degree maxDegree {4};   ///< Maximal vertex degree allowed.

constexpr std::array<Degree, maxDegree + 1> degrees {0, 1, 2, 3, 4};

template <Degree D>
constexpr auto is_implemented_degree = D <= maxDegree && D >= 0;

constexpr std::array<Degree, 2> bulkDegrees {0, 2};
template <Degree D>
constexpr auto is_bulk_degree =
    std::find(bulkDegrees.begin(), bulkDegrees.end(), D) != bulkDegrees.end();

constexpr Degree leafDegree {1};
template <Degree D>
constexpr auto is_leaf_degree = D == leafDegree;

template<Degree D>
concept LeafDegree = D == leafDegree;

constexpr std::array<Degree, 2> junctionDegrees {3, 4};
template <Degree D>
constexpr auto is_junction_degree =
    std::find(junctionDegrees.begin(), junctionDegrees.end(), D) !=
    junctionDegrees.end();


template <Degree D>
constexpr auto is_end_degree = is_leaf_degree<D> ||
                               is_junction_degree<D>;
template<Degree D>
concept BulkDegree =
    std::find(bulkDegrees.cbegin(), bulkDegrees.cend(), D) !=
    bulkDegrees.cend();


template<Degree D>
concept EndDegree = not BulkDegree<D>;

}  // namespace structure::vertices::degrees

}  // namespace graph_mutator


#endif  // GRAPH_MUTATOR_STRUCTURE_VERTICES_DEGREES_H
