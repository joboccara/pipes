#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <type_traits>
#include <utility>

namespace pipes
{
    namespace detail
    {
        namespace impl
        {
            // definition of range
            
            namespace adl
            {
                using std::begin;
                using std::end;
                template<typename T>
                using begin_expression = decltype(begin(std::declval<T&>()));
                template<typename T>
                using end_expression = decltype(end(std::declval<T&>()));
            }

            template<typename Range>
            constexpr bool range_expression_detected = detail::is_detected<adl::begin_expression, Range> && detail::is_detected<adl::end_expression, Range>;
            
            template<typename Range>
            using IsARange = std::enable_if_t<range_expression_detected<Range>, bool>;
            
            // definition of pipe
            
            struct aPipeline : pipeline_base<aPipeline>{};
            template<typename Pipe>
            using pipe_expression = decltype(std::declval<Pipe&>().plug_to_pipeline(std::declval<aPipeline&>()));
            
            template<typename Pipe>
            constexpr bool pipe_expression_detected = detail::is_detected<pipe_expression, Pipe>;
            
            template<typename Pipe>
            using IsAPipe = std::enable_if_t<pipe_expression_detected<Pipe>, bool>;

            //definition of pipeline
            
            template<typename Pipeline>
            using IsAPipeline = std::enable_if_t<std::is_base_of<pipeline_base<Pipeline>, Pipeline>::value, bool>;
        }
        
        template<typename Range>
        using IsARange = impl::IsARange<std::remove_reference_t<Range>>;
        
        template<typename Pipe>
        using IsAPipe = impl::IsAPipe<std::remove_reference_t<Pipe>>;
        
        template<typename Pipeline>
        using IsAPipeline = impl::IsAPipeline<std::remove_reference_t<Pipeline>>;

    } // namespace detail

    template<typename Pipe, typename Pipeline, detail::IsAPipe<Pipe> = true, detail::IsAPipeline<Pipeline> = true>
    auto operator>>=(Pipe&& pipe, Pipeline&& pipeline);

    namespace detail
    {
        template<typename Pipe1, typename Pipe2>
        class CompositePipe
        {
        public:
            template<typename Pipeline>
            auto plug_to_pipeline(Pipeline&& pipeline)
            {
                return pipe1_ >>= pipe2_ >>= pipeline;
            }
            
            template<typename Pipe1_, typename Pipe2_>
            CompositePipe(Pipe1_&& pipe1, Pipe2_&& pipe2) : pipe1_(FWD(pipe1)), pipe2_(FWD(pipe2)){}
        private:
            Pipe1 pipe1_;
            Pipe2 pipe2_;
        };
    } // namespace detail
    

// range >>= pipeline
    
template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true>
void operator>>=(Range&& range, Pipeline&& pipeline)
{
    std::copy(begin(range), end(range), pipeline);
}

// pipe >>= pipe
    
    template<typename Pipe1, typename Pipe2, detail::IsAPipe<Pipe1> = true, detail::IsAPipe<Pipe2> = true>
    detail::CompositePipe<std::remove_reference_t<Pipe1>, std::remove_reference_t<Pipe2>> operator>>=(Pipe1&& pipe1, Pipe2&& pipe2)
    {
        return detail::CompositePipe<std::remove_reference_t<Pipe1>, std::remove_reference_t<Pipe2>>(FWD(pipe1), FWD(pipe2));
    }

// pipe >>= pipeline
    
    template<typename Pipe, typename Pipeline, detail::IsAPipe<Pipe>, detail::IsAPipeline<Pipeline>>
    auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
    {
        return pipe.plug_to_pipeline(pipeline);
    }
    
} // namespace pipes

#endif /* OPERATOR_HPP */
