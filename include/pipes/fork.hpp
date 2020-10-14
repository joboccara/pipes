#ifndef PIPES_DEMUX_HPP
#define PIPES_DEMUX_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/meta.hpp"
#include "pipes/base.hpp"
#include "pipes/impl/pipelines_reduction.hpp"

namespace pipes
{

template<typename... TailPipelines>
class fork_pipeline : public pipeline_base<fork_pipeline<TailPipelines...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        detail::for_each(tailPipelines_, [&value](auto&& tailPipeline){ send(FWD(value), tailPipeline); });
    }

	template<size_t... indices>
	auto move_reduced_value_from(std::index_sequence<indices...>)
	{
		return std::make_tuple(detail::move_reduced_value_from(std::get<indices>(tailPipelines_))...);
	}
	auto move_reduced_value_from()
	{
		return move_reduced_value_from(std::make_index_sequence<sizeof...(TailPipelines)>());
	}

    explicit fork_pipeline(TailPipelines const&... tailPipelines) : tailPipelines_(tailPipelines...) {}
    
private:
    std::tuple<TailPipelines...> tailPipelines_;
};

template<typename... TailPipelines>
fork_pipeline<TailPipelines...> fork(TailPipelines const&... tailPipelines)
{
    return fork_pipeline<TailPipelines...>(tailPipelines...);
}

} // namespace pipes

#endif /* PIPES_DEMUX_HPP */
