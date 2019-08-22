#ifndef PIPES_FILTER_HPP
#define PIPES_FILTER_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"
#include "pipes/pipeline_base.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
template<typename Predicate, typename Pipeline>
class filter_pipeline : public pipeline_base<filter_pipeline<Predicate, Pipeline>>
{
public:    
    template<typename T>
    void onReceive(T const& value)
    {
        if (predicate_(value))
        {
            send(pipeline_, value);
        }
    }

    explicit filter_pipeline(Predicate predicate, Pipeline pipeline) : predicate_(predicate), pipeline_(pipeline) {}
    
private:
    detail::assignable<Predicate> predicate_;
    Pipeline pipeline_;

public: // but technical
    using base = pipeline_base<filter_pipeline<Predicate, Pipeline>>;
    using base::operator=;
    filter_pipeline& operator=(filter_pipeline const& other)
    {
        predicate_ = other.predicate_;
        pipeline_ = other.pipeline_;
        return *this;
    }
    filter_pipeline& operator=(filter_pipeline& other) { *this = const_cast<filter_pipeline const&>(other); return *this; }
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

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_FILTER_HPP */
