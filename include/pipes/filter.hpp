#ifndef PIPES_FILTER_HPP
#define PIPES_FILTER_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/pipeline_base.hpp"

namespace pipes
{
template<typename Predicate, typename Pipeline>
class filter_pipeline : public pipeline_base<filter_pipeline<Predicate, Pipeline>>
{
public:    
    template<typename T>
    void onReceive(T&& value)
    {
        if (predicate_(FWD(value)))
        {
            send(pipeline_, FWD(value));
        }
    }

    explicit filter_pipeline(Predicate predicate, Pipeline pipeline) : predicate_(predicate), pipeline_(pipeline) {}
    
private:
    detail::assignable<Predicate> predicate_;
    Pipeline pipeline_;
};

template<typename Predicate>
class filter_pipe
{
public:
    template<typename Pipeline>
    auto plug_to_pipeline(Pipeline&& pipeline) const
    {
        return filter_pipeline<Predicate, std::remove_reference_t<Pipeline>>{predicate_, pipeline};
    }
    
    explicit filter_pipe(Predicate predicate) : predicate_(predicate){}
    
private:
    Predicate predicate_;
};

template<typename Predicate>
filter_pipe<Predicate> filter(Predicate const& predicate)
{
    return filter_pipe<Predicate>{predicate};
}

} // namespace pipes

#endif /* PIPES_FILTER_HPP */
