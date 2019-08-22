#ifndef PIPES_TRANSFORM_HPP
#define PIPES_TRANSFORM_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename Function, typename Pipeline>
class transform_pipeline : public pipeline_base<transform_pipeline<Function, Pipeline>>
{
public:
    template<typename T>
    void onReceive(T&& input)
    {
        send(pipeline_, function_(input));
    }

    explicit transform_pipeline(Function function, Pipeline pipeline) : function_(function), pipeline_(pipeline) {}
    
private:
    detail::assignable<Function> function_;
    Pipeline pipeline_;

public: // but technical
    using base = pipeline_base<transform_pipeline<Function, Pipeline>>;
    using base::operator=;
    transform_pipeline& operator=(transform_pipeline const& other)
    {
        function_ = other.function_;
        pipeline_ = other.pipeline_;
        return *this;
    }
    transform_pipeline& operator=(transform_pipeline& other) { *this = const_cast<transform_pipeline const&>(other); return *this; }
};

template<typename Function>
class transform_pipe
{
public:
    template<typename Pipeline>
    auto plug_to_pipeline(Pipeline&& pipeline) const
    {
        return transform_pipeline<Function, std::remove_reference_t<Pipeline>>{function_, pipeline};
    }
    
    explicit transform_pipe(Function function) : function_(function){}

private:
    Function function_;
};

template<typename Function>
transform_pipe<Function> transform(Function&& function)
{
    return transform_pipe<Function>{function};
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_TRANSFORM_HPP */
