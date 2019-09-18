#ifndef PIPES_DEMUX_HPP
#define PIPES_DEMUX_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/meta.hpp"
#include "pipes/base.hpp"

namespace pipes
{

template<typename... TailPipelines>
class demux_pipeline : public pipeline_base<demux_pipeline<TailPipelines...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        detail::for_each(tailPipelines_, [&value](auto&& tailPipeline){ send(FWD(value), tailPipeline); });
    }

    explicit demux_pipeline(TailPipelines const&... tailPipelines) : tailPipelines_(tailPipelines...) {}
    
private:
    std::tuple<TailPipelines...> tailPipelines_;
};

template<typename... TailPipelines>
demux_pipeline<TailPipelines...> demux(TailPipelines const&... tailPipelines)
{
    return demux_pipeline<TailPipelines...>(tailPipelines...);
}

} // namespace pipes

#endif /* PIPES_DEMUX_HPP */
