#ifndef PIPES_CONCEPTS_HPP
#define PIPES_CONCEPTS_HPP

#include <type_traits>
#include <utility>

#include "pipes/base.hpp"
#include "pipes/helpers/detect.hpp"

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
            
            template<typename Pipe>
            using IsAPipe = std::enable_if_t<std::is_base_of<pipe_base, Pipe>::value, bool>;
            
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
} // namespace pipes

#endif /* PIPES_CONCEPTS_HPP */
