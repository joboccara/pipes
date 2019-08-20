#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "pipes/helpers/detect.hpp"

#include <utility>

namespace pipes
{

template<typename Pipe, typename Pipeline>
using create_pipeline_expression = decltype(std::declval<Pipe&>().create_pipeline(std::declval<Pipeline&>()));

template<typename Pipe, typename Pipeline>
constexpr bool create_pipeline_detected = detail::is_detected<create_pipeline_expression, Pipe, Pipeline>;

template<typename Pipe, typename Pipeline>
using CanCreatePipeline = std::enable_if_t<create_pipeline_detected<Pipe, Pipeline>, bool>;

template<typename Pipe, typename Pipeline, CanCreatePipeline<Pipe, Pipeline> = true>
auto operator>>=(Pipe&& pipe, Pipeline&& pipeline)
{
    return pipe.create_pipeline(pipeline);
}

} // namespace pipes

#endif /* OPERATOR_HPP */
