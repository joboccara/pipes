#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/helpers/detect.hpp"

#include <type_traits>
#include <utility>

namespace pipes
{
    
// pipe >>= pipeline

    namespace detail
    {
        namespace impl
        {
            struct aPipeline{};
            template<typename Pipe>
            using pipe_expression = decltype(std::declval<Pipe&>().plug_to_pipeline(aPipeline{}));
            
            template<typename Pipe>
            constexpr bool pipe_expression_detected = detail::is_detected<pipe_expression, Pipe>;
            
            template<typename Pipe>
            using IsAPipe = std::enable_if_t<pipe_expression_detected<Pipe>, bool>;
        }
        
        template<typename Pipe>
        using IsAPipe = impl::IsAPipe<std::remove_reference_t<Pipe>>;
        
    } //  namespace detail
    
    template<typename Pipe, typename Pipeline, detail::IsAPipe<Pipe> = true>
    auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
    {
        return pipe.plug_to_pipeline(pipeline);
    }
    
// range >>= pipeline
    
    namespace detail
    {
        namespace impl
        {
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
            
            template<typename OutputIterator>
            using IsAPipeline = std::enable_if_t<std::is_same<typename OutputIterator::iterator_category, std::output_iterator_tag>::value, bool>;
        }
        
        template<typename Range>
        using IsARange = impl::IsARange<std::remove_reference_t<Range>>;
        
        template<typename Pipeline>
        using IsAPipeline = impl::IsAPipeline<std::remove_reference_t<Pipeline>>;

    } // namespace detail
    
    template<typename Range, typename Pipeline, detail::IsARange<Range> = true, detail::IsAPipeline<Pipeline> = true>
    void operator>>=(Range&& range, Pipeline&& pipeline)
    {
        std::copy(begin(range), end(range), pipeline);
    }

} // namespace pipes

#endif /* OPERATOR_HPP */
