#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/helpers/detect.hpp"

#include <utility>

namespace pipes
{

template<typename Pipe, typename Pipeline>
using plug_to_pipeline_expression = decltype(std::declval<Pipe&>().plug_to_pipeline(std::declval<Pipeline&>()));

template<typename Pipe, typename Pipeline>
constexpr bool plug_to_pipeline_detected = detail::is_detected<plug_to_pipeline_expression, Pipe, Pipeline>;

template<typename Pipe, typename Pipeline>
using CanCreatePipeline = std::enable_if_t<plug_to_pipeline_detected<Pipe, Pipeline>, bool>;

template<typename Pipe, typename Pipeline, CanCreatePipeline<Pipe, Pipeline> = true>
auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
{
    return pipe.plug_to_pipeline(pipeline);
}

} // namespace pipes

#endif /* OPERATOR_HPP */
