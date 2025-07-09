#ifndef GRAPH_MUTATOR_STRUCTURE_CHAIN_COLLECTION_H
#define GRAPH_MUTATOR_STRUCTURE_CHAIN_COLLECTION_H


#include <string>

#include "../definitions.h"
#include "../vector_container.h"
#include "chain.h"
#include "ends.h"
#include "slot.h"


namespace graph_mutator::structure {


template<typename Ch>
struct ChainContainer
    : public VectorContainer<Ch> {

using Base = VectorContainer<Ch>;
using size_type = Base::size_type;
using EndSlot = Ch::EndSlot;

constexpr auto num() const noexcept -> size_type;

constexpr auto ind_last() const noexcept -> size_type;

constexpr auto ngs_of(const EndSlot s) const noexcept;

void print(const std::string& tag) const;



private:

    using Base::data;
};


// IMPLEMENTATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template<typename Ch>
constexpr
auto ChainContainer<Ch>::
num() const noexcept -> size_type
{
    return this->size();
}


template<typename Ch>
constexpr
auto ChainContainer<Ch>::
ind_last() const noexcept -> size_type
{
    const auto n = num();

    return n ? num() - 1
             : undefined<size_type>;
}


template<typename Ch>
constexpr
auto ChainContainer<Ch>::
ngs_of(const EndSlot s) const noexcept
{
    return data[s.w].ngs[s.e];
}


template<typename Ch>
void ChainContainer<Ch>::
print(const std::string& tag) const
{
    for (const auto& m: data)
        m.print(tag);
}


}  // namespace graph_mutator::structure

#endif  // GRAPH_MUTATOR_STRUCTURE_CHAIN_COLLECTION_H
