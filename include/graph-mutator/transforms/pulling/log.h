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
 * @file log.h
 * @brief Contains class responsible for logging the summary of pulling transformations.
 * @author Valerii Sukhorukov
 */

#ifndef GRAPH_MUTATOR_PULLING_LOG_H
#define GRAPH_MUTATOR_PULLING_LOG_H

#include <array>

#include "paths.h"  // for Log


namespace graph_mutator::pulling {

/**
 * @brief Functor for logging the summary of pulling transformations.
 * @tparam F Functor class for the edge pulling initiated from a vertex of
 * specific degree.
 */
template<typename F>
struct Log {

using Ps = Paths<typename F::Cmpt>;

const F& functor;

explicit Log(const F& functor)
    : functor {functor}
{}


void before(
    const Ps& pp,
    const int n
) const noexcept
{
    const auto chs = pp.path_chains(pp.pthc);
    const auto& schain = cn(chs.back(), pp);
    const auto src_is_engulfed = schain.length() > 1 &&
                                 n == static_cast<int>(schain.length());

    log_(colorcodes::GREEN, F::fullName, " :: ", colorcodes::YELLOW,
         n, " step", n > 1 ? "s " : " ", colorcodes::RESET, "over path:");
    pp.print_detailed("   ");
    pp.template print_short<true>("");
    log_("");

    for (szt i {}; const auto w: chs)
        cn(w, pp).print("path ch ", i++, " before: ");
    if (src_is_engulfed) {
        const auto ss = pp.src().opp();  // connected slot of the source chain
        const auto next = chs[chs.size() - 2];
        for (const auto& s: cn(ss.w, pp).ngs[ss.e]())
            if (s.w != next && s.w != ss.w)
                cn(s.w, pp).print("before src side chain ");
    }

    log_("");
}


void after(const Ps& pp) const noexcept
{
    const auto chs = pp.path_chains(pp.pthc);

    log_("");
    log_(colorcodes::GREEN, F::shortName, colorcodes::RESET, " producing:");

    for (szt i {}; const auto w: chs)
        cn(w, pp).print("path ch ", i++, " after ");

    log_("");
}

constexpr
auto cn(
    const ChId w,
    const Ps& pp
) const noexcept -> const Ps::Chain&
{
    return pp.cmp->chain(w);
}

};

}  // namespace graph_mutator::pulling

#endif  // GRAPH_MUTATOR_PULLING_LOG_H
