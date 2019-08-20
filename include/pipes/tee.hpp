#ifndef PIPES_TEE_HPP
#define PIPES_TEE_HPP

#include "pipes/operator.hpp"

#include "pipes/output_iterator.hpp"
#include "pipes/helpers/warnings.hpp"

#include <type_traits>

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
    
template<typename TeePipeline, typename Pipeline>
class tee_pipeline : public OutputIteratorBase<tee_pipeline<TeePipeline, Pipeline>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        send(teePipeline_, value);
        send(pipeline_, value);
    }
    
    tee_pipeline(TeePipeline const& teePipeline, Pipeline const& pipeline) : teePipeline_(teePipeline), pipeline_(pipeline){}

private:
    TeePipeline teePipeline_;
    Pipeline pipeline_;

public: // but technical
    using base = OutputIteratorBase<tee_pipeline<TeePipeline, Pipeline>>;
    using base::operator=;
    tee_pipeline& operator=(tee_pipeline const& other)
    {
        teePipeline_ = other.teePipeline_;
        pipeline_ = other.pipeline_;
        return *this;
    }
    tee_pipeline& operator=(tee_pipeline& other) { *this = const_cast<tee_pipeline const&>(other); return *this; }
};
    
template<typename TeePipeline>
class tee_pipe
{
public:
    template<typename Pipeline>
    auto create_pipeline(Pipeline&& pipeline) const
    {
        return tee_pipeline<TeePipeline, std::remove_reference_t<Pipeline>>{teePipeline_, pipeline};
    }
    
    explicit tee_pipe(TeePipeline teePipeline) : teePipeline_(teePipeline){}
    
private:
    TeePipeline teePipeline_;
};
    
template<typename TeePipeline>
tee_pipe<TeePipeline> tee(TeePipeline const& teePipeline)
{
    return tee_pipe<TeePipeline>{teePipeline};
}
    
} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_TEE_HPP */
