#ifndef GRAPH_MUTATOR_PULLING_FUNCTOR_1_H
#define GRAPH_MUTATOR_PULLING_FUNCTOR_1_H

#include "../../definitions.h"
#include "../../string_ops.h"
#include "../../structure/graph.h"
#include "common.h"
#include "functor_base.h"
#include "log.h"
#include "paths.h"


namespace graph_mutator::pulling {


template<Orientation Dir,
         typename G>
struct Functor<Deg1, Dir, G>
    : public FunctorBase<G> {

    static constexpr auto Direction = Dir;

    /// Degree of the pulled vertex.
    static constexpr auto D = Deg1;
    static_assert(is_pullable_degree<D>);

    static constexpr auto d = string_ops::str1<D>;

    static constexpr auto shortName
        {string_ops::shName<d, 1, 'p', 'u', '_', signchar<Dir>>};
    static constexpr auto fullName
        {string_ops::shName<d, 1,
         'P', 'u', 'l', 'l', 'i', 'n', 'g', ' ', 'f', 'r', 'o', 'm', ' ',
         'V', 'e', 'r', 't', 'e', 'x', ' ', 'D', 'e', 'g', ' ', signchar<Dir>>};

    using Graph = G;
    using Cmpt = Graph::Cmpt;
    using Chain = Graph::Chain;
    using Base = FunctorBase<G>;
    using L = Log<Functor<D, Dir, Graph>>;

    /**
     * @brief Constructs a Functor object based on the Graph instance.
     * @param gr Graph on which the transformations operate.
     */
    explicit Functor(Graph& gr);

    /**
     * @brief Pulls a vertex of degree 1.
     * The path starts with a driver edge and ends with a source edge.
     * @note The path is expected to be a valid path over the graph.
     * @param pp Path to the vertex to pull.
     * @param n Number of pulling steps.
     * @return Result containing the component ID after the pull operation.
     */
    auto operator()(
        Paths<Cmpt>& pp,
        int n
    ) noexcept -> Res;

protected:

    using Base::cn;

    void check_path(
        const Paths<Cmpt>& pp,
        int n
    ) const noexcept;

private:

    L log;
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<Orientation Dir,
         typename G>
Functor<Deg1, Dir, G>::
Functor(Graph& gr)
    : Base {gr}
    , log {*this}
{}


template<Orientation Dir,
         typename G>
auto Functor<Deg1, Dir, G>::
operator()(
    Paths<Cmpt>& pp,
    int n
) noexcept -> Res
{
    if constexpr (verboseF)
        log.before(pp, n);

    check_path(pp, n);

    this->pull_free_end(pp, n);

    if constexpr (verboseF)
        log.after(pp);

    return {pp.cmp->ind};
}


template<Orientation Dir,
         typename G>
void Functor<Deg1, Dir, G>::
check_path(
    const Paths<Cmpt>& pp,
    const int n
) const noexcept
{
    const auto c = pp.cmp;
    const auto p = pp.pth;
    const auto wS = pp.s.w;
    const auto eS = pp.s.e;
    const auto wD = pp.d.w;
    const auto eD = pp.d.end_slot(c->chain(wD)).e;

    ASSERT(c->chain(wD).ngs[eD].num() == 0, "Driver end is not disconnected");

    const auto sourceIsCycle = cn[wS].is_cycle();

    ASSERT(sourceIsCycle ||
           (!sourceIsCycle && c->chain(wS).ngs[eS].num() == 0),
           "Source end is not disconnected");

    const auto lenS = pp.length_over_source_chain();

    ASSERT((!sourceIsCycle &&
            lenS >= static_cast<EgId>(n)) ||
           (sourceIsCycle &&
            lenS >= Chain::minCycleLength + static_cast<EgId>(n)),
           "Pulling over the track longer than the source chain");
}


}  // namespace graph_mutator::pulling

#endif  // GRAPH_MUTATOR_PULLING_FUNCTOR_1_H
