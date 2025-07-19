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

constexpr auto Deg0 = static_cast<Degree>(0);
constexpr auto Deg1 = static_cast<Degree>(1);
constexpr auto Deg2 = static_cast<Degree>(2);
constexpr auto Deg3 = static_cast<Degree>(3);
constexpr auto Deg4 = static_cast<Degree>(4);

constexpr Degree maxDegree {Deg4};   ///< Maximal vertex degree allowed.

constexpr std::array<Degree, maxDegree + Deg1> degrees {
    Deg0, Deg1, Deg2, Deg3, Deg4
};

template <Degree D>
constexpr auto effective_degree = (D == Deg0 ? Deg2 : D);

template <Degree D>
constexpr auto is_implemented_degree =
    D <= maxDegree &&
    D >= Deg0;

template <Degree D1,
          Degree D2>
constexpr auto are_compatible_degrees =
    D1 >= Deg0 &&
    D2 >= Deg0 &&
    effective_degree<D1> + effective_degree<D2> <= maxDegree;

constexpr std::array<Degree, 2> bulkDegrees {Deg0, Deg2};
template <Degree D>
constexpr auto is_bulk_degree =
    std::find(bulkDegrees.begin(), bulkDegrees.end(), D) != bulkDegrees.end();

constexpr Degree leafDegree {Deg1};
template <Degree D>
constexpr auto is_leaf_degree = D == leafDegree;

template<Degree D>
concept LeafDegree = D == leafDegree;

constexpr std::array<Degree, 2> junctionDegrees {Deg3, Deg4};
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
