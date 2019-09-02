#ifndef PIPES_OUTPUT_ITERATOR_HPP
#define PIPES_OUTPUT_ITERATOR_HPP

#include "pipes/helpers/crtp.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <iterator>
#include <tuple>

namespace pipes
{
    struct pipe_base {};

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

    template<typename Pipeline>
    struct pipeline_proxy
    {
        template<typename T>
        pipeline_proxy& operator=(T&& input)
        {
            send(FWD(input), pipeline_);
            return *this;
        }
        
        explicit pipeline_proxy(Pipeline& pipeline) : pipeline_(pipeline){}
        
    private:
        Pipeline& pipeline_;
    };

    template<typename Derived>
    struct pipeline_base : detail::crtp<Derived, pipeline_base>
    {
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        
        Derived& operator++() { return this->derived(); }
        Derived& operator++(int){ ++this->derived(); return this->derived(); }
        pipeline_proxy<Derived> operator*() { return pipeline_proxy<Derived>(this->derived()); }
    };


    
} // namespace pipes

#endif /* PIPES_OUTPUT_ITERATOR_HPP */
