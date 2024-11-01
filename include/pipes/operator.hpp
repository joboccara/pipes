#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/impl/concepts.hpp"
#include "pipes/impl/pipes_assembly.hpp"
#include "pipes/impl/pipelines_reduction.hpp"

#include <type_traits>

namespace pipes
{

// range >>= pipeline (rvalue ranges)
    
    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true, std::enable_if_t<!std::is_lvalue_reference<Range>::value, int> = 0>
    auto operator>>=(Range&& range, Pipeline&& pipeline)
    {
        using std::begin;
        using std::end;
		return detail::move_reduced_value_from(std::copy(std::make_move_iterator(begin(range)), std::make_move_iterator(end(range)), pipeline));
    }

// range >>= pipeline (lvalue ranges)
    
    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true, std::enable_if_t<std::is_lvalue_reference<Range>::value, int> = 0>
    auto operator>>=(Range&& range, Pipeline&& pipeline)
    {
        using std::begin;
        using std::end;
		return detail::move_reduced_value_from(std::copy(begin(range), end(range), pipeline));
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
        return make_generic_pipeline(pipe, pipeline);
    }
    
} // namespace pipes

#endif /* OPERATOR_HPP */
