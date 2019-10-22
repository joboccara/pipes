#ifndef PIPES_SEND_HPP
#define PIPES_SEND_HPP

#include "pipes/helpers/FWD.hpp"

#include <tuple>

namespace pipes
{
    namespace detail
    {
        template<size_t>
        struct send_tag{};
        
        template<typename Pipeline, typename... Ts>
        void send(send_tag<1>, Pipeline& pipeline, Ts&&... values)
        {
            pipeline.onReceive(FWD(values)...);
        }
        
        template<typename... ValuesThenPipeline, size_t... ValuesIndexes>
        void send(send_tag<0>, std::tuple<ValuesThenPipeline...> valuesThenPipeline, std::index_sequence<ValuesIndexes...>)
        {
            auto constexpr pipelineIndex = sizeof...(ValuesThenPipeline) - 1;
            send(send_tag<1>{}, std::get<pipelineIndex>(valuesThenPipeline), std::get<ValuesIndexes>(valuesThenPipeline)...);
        }
    }

    template<typename T, typename Pipeline>
    void send(T&& value, Pipeline& pipeline)
    {
        pipeline.onReceive(FWD(value));
    }
    
    // usage: send(0, 1, 2, 3, myPipeline);
    template<typename... ValuesThenPipeline>
    void send(ValuesThenPipeline&&... valuesThenPipeline)
    {
        detail::send(detail::send_tag<0>{}, std::forward_as_tuple(FWD(valuesThenPipeline)...), std::make_index_sequence<sizeof...(ValuesThenPipeline) - 1>{});
    }
    
    namespace detail
    {
        template<typename... Ts, typename Pipeline, size_t... Is>
        void sendTupleValues(std::tuple<Ts...> const& tuple, Pipeline& pipeline, std::index_sequence<Is...>)
        {
            send(std::get<Is>(tuple)..., pipeline);
        }
    }
    
    template<typename... Ts, typename Pipeline>
    void sendTupleValues(std::tuple<Ts...> const& tuple, Pipeline& pipeline)
    {
        detail::sendTupleValues(tuple, pipeline, std::make_index_sequence<sizeof...(Ts)>{});
    }
}

#endif /* PIPES_SEND_HPP */
