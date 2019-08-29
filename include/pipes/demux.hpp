#ifndef PIPES_DEMUX_HPP
#define PIPES_DEMUX_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"
#include "pipes/pipeline_base.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename... TailPipelines>
class demux_pipeline : public pipeline_base<demux_pipeline<TailPipelines...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        detail::for_each([&value](auto&& tailPipeline){ send(tailPipeline, FWD(value)); }, tailPipelines_);
    }

    explicit demux_pipeline(TailPipelines const&... tailPipelines) : tailPipelines_(tailPipelines...) {}
    
private:
    std::tuple<TailPipelines...> tailPipelines_;
    
public: // but technical
    using base = pipeline_base<demux_pipeline<TailPipelines...>>;
    using base::operator=;
    demux_pipeline& operator=(demux_pipeline const& other)
    {
        tailPipelines_ = other.tailPipelines_;
        return *this;
    }
    demux_pipeline& operator=(demux_pipeline& other) { *this = const_cast<demux_pipeline const&>(other); return *this; }
};

template<typename... TailPipelines>
demux_pipeline<TailPipelines...> demux(TailPipelines const&... tailPipelines)
{
    return demux_pipeline<TailPipelines...>(tailPipelines...);
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_DEMUX_HPP */
