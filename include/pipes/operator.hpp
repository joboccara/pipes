#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/impl/concepts.hpp"
#include "pipes/impl/pipes_assembly.hpp"

#include <type_traits>
#include <utility>

namespace pipes
{

// range >>= pipeline
    
    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true>
    void operator>>=(Range&& range, Pipeline&& pipeline)
    {
        std::copy(begin(range), end(range), pipeline);
    }

// pipe >>= pipe
    
    template<typename Pipe1, typename Pipe2, detail::IsAPipe<Pipe1> = true, detail::IsAPipe<Pipe2> = true>
    auto operator>>=(Pipe1&& pipe1, Pipe2&& pipe2)
    {
        return detail::make_composite_pipe(FWD(pipe1), FWD(pipe2));
    }

// pipe >>= pipeline
    
    template<typename Pipe, typename Pipeline, detail::IsAPipe<Pipe> = true, detail::IsAPipeline<Pipeline> = true>
    auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
    {
        return make_generic_pipeline(pipe, pipeline);
    }
    
// range | pipe
    
    template<typename Range, typename Pipe, detail::IsARange<Range> = true, detail::IsAPipe<Pipe> = true>
    auto operator|(Range&& range, Pipe&& pipe)
    {
        return detail::make_range_pipe(FWD(range), FWD(pipe));
    }
    
// RangePipe | pipe
    
    template<typename Range, typename Pipe1, typename Pipe2, detail::IsAPipe<Pipe2> = true>
    auto operator|(detail::RangePipe<Range, Pipe1> rangePipe, Pipe2&& pipe2)
    {
        return detail::make_range_pipe(FWD(rangePipe.range), detail::make_composite_pipe(rangePipe.pipe, FWD(pipe2)));
    }
    
// pipe | pipe
    
    template<typename Pipe1, typename Pipe2, detail::IsAPipe<Pipe1> = true, detail::IsAPipe<Pipe2> = true>
    auto operator|(Pipe1&& pipe1, Pipe2&& pipe2)
    {
        return detail::make_composite_pipe(FWD(pipe1), FWD(pipe2));
    }
    
// RangePipe | pipeline
    
    template<typename Range, typename Pipe, typename Pipeline, detail::IsAPipeline<Pipeline> = true>
    auto operator|(detail::RangePipe<Range, Pipe> rangePipe, Pipeline&& pipeline)
    {
        return rangePipe.range >>= rangePipe.pipe >>= FWD(pipeline);
    }
    
// pipe | pipeline
    
    template<typename Pipe, typename Pipeline, detail::IsAPipe<Pipe> = true, detail::IsAPipeline<Pipeline> = true>
    auto operator|(Pipe&& pipe, Pipeline&& pipeline)
    {
        return FWD(pipe) >>= FWD(pipeline);
    }
    
// Range | pipeline
    
    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true>
    auto operator|(Range&& range, Pipeline&& pipeline)
    {
        return FWD(range) >>= FWD(pipeline);
    }
} // namespace pipes

#endif /* OPERATOR_HPP */
