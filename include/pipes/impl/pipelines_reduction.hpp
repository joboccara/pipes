#ifndef PIPELINES_REDUCTION_HPP
#define PIPELINES_REDUCTION_HPP 

#include "pipes/impl/concepts.hpp"

#include <tuple>

namespace pipes
{
	namespace detail{
		template<typename Pipeline, detail::IsANonReturningPipeline<Pipeline> = true>
		auto move_reduced_value_from(Pipeline&&){return std::ignore;}

		template<typename Pipeline, detail::IsAReturningPipeline<Pipeline> = true>
		auto move_reduced_value_from(Pipeline&& pipeline)
		{
			return pipeline.move_reduced_value_from();
		}
	}
}

#endif /* PIPELINES_REDUCTION_HPP */
