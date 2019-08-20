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
        template<typename Pipe, typename Pipeline>
        using plug_to_pipeline_expression = decltype(std::declval<Pipe&>().plug_to_pipeline(std::declval<Pipeline&>()));

        template<typename Pipe, typename Pipeline>
        constexpr bool plug_to_pipeline_detected = detail::is_detected<plug_to_pipeline_expression, Pipe, Pipeline>;

        template<typename Pipe, typename Pipeline>
        using CanCreatePipeline = std::enable_if_t<plug_to_pipeline_detected<Pipe, Pipeline>, bool>;

    } //  namespace detail
    
    template<typename Pipe, typename Pipeline,
        detail::CanCreatePipeline<std::remove_reference_t<Pipe>, std::remove_reference_t<Pipeline>> = true>
    auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
    {
        return pipe.plug_to_pipeline(pipeline);
    }
    
// input >>= pipeline
    
    namespace detail
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
        using IsAnOutputIterator = std::enable_if_t<std::is_same<typename OutputIterator::iterator_category, std::output_iterator_tag>::value, bool>;
    } // namespace detail
    
    template<typename Range, typename OutputIterator,
    detail::IsARange<std::remove_reference_t<Range>> = true,
    detail::IsAnOutputIterator<std::remove_reference_t<OutputIterator>> = true>
    void operator>>=(Range&& range, OutputIterator&& outputIterator)
    {
        std::copy(begin(range), end(range), outputIterator);
    }

} // namespace pipes

#endif /* OPERATOR_HPP */
