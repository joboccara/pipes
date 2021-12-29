#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/impl/concepts.hpp"
#include "pipes/impl/pipes_assembly.hpp"

#include <type_traits>

namespace pipes
{

// range >>= pipeline (rvalue ranges)

    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true>
    std::enable_if_t<!std::is_lvalue_reference<Range>::value> operator>>=(Range&& range, Pipeline&& pipeline)
    {
        using std::begin; using std::end;
        auto beg_it = begin(range);
        auto end_it = end(range);
        while (beg_it != end_it)
        {
            *pipeline = std::move(*beg_it);
            (void) ++beg_it;
        }
    }

// range >>= pipeline (lvalue ranges)

    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true>
    std::enable_if_t<std::is_lvalue_reference<Range>::value> operator>>=(Range&& range, Pipeline&& pipeline)
    {
        using std::begin; using std::end;
        auto beg_it = begin(range);
        auto end_it = end(range);
        while (beg_it != end_it)
        {
            *pipeline = *beg_it;
            (void) ++beg_it;
        }
    }

// pipe >>= pipe

    template<typename Pipe1, typename Pipe2, detail::IsAPipe<Pipe1> = true, detail::IsAPipe<Pipe2> = true>
    auto operator>>=(Pipe1&& pipe1, Pipe2&& pipe2)
    {
        return detail::CompositePipe<std::decay_t<Pipe1>, std::decay_t<Pipe2>>(FWD(pipe1), FWD(pipe2));
    }

// pipe >>= pipeline

    template<typename Pipe, typename Pipeline, detail::IsAPipe<Pipe> = true, detail::IsAPipeline<Pipeline> = true>
    auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
    {
        return make_generic_pipeline(std::forward<Pipe>(pipe), std::forward<Pipeline>(pipeline));
    }

} // namespace pipes

#endif /* OPERATOR_HPP */
