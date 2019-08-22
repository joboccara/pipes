#ifndef PIPES_TEE_HPP
#define PIPES_TEE_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/warnings.hpp"

#include <type_traits>

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
    
template<typename TeeBranch, typename PipelineTail>
class tee_pipeline : public pipeline_base<tee_pipeline<TeeBranch, PipelineTail>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        send(teeBranch_, value);
        send(pipelineTail_, value);
    }
    
    tee_pipeline(TeeBranch const& teeBranch, PipelineTail const& pipelineTail) : teeBranch_(teeBranch), pipelineTail_(pipelineTail){}

private:
    TeeBranch teeBranch_;
    PipelineTail pipelineTail_;

public: // but technical
    using base = pipeline_base<tee_pipeline<TeeBranch, PipelineTail>>;
    using base::operator=;
    tee_pipeline& operator=(tee_pipeline const& other)
    {
        teeBranch_ = other.teeBranch_;
        pipelineTail_ = other.pipelineTail_;
        return *this;
    }
    tee_pipeline& operator=(tee_pipeline& other) { *this = const_cast<tee_pipeline const&>(other); return *this; }
};
    
template<typename TeeBranch>
class tee_pipe
{
public:
    template<typename Pipeline>
    auto plug_to_pipeline(Pipeline&& pipeline) const
    {
        return tee_pipeline<TeeBranch, std::remove_reference_t<Pipeline>>{teeBranch_, pipeline};
    }
    
    explicit tee_pipe(TeeBranch teeBranch) : teeBranch_(teeBranch){}
    
private:
    TeeBranch teeBranch_;
};
    
template<typename TeeBranch>
tee_pipe<TeeBranch> tee(TeeBranch const& teeBranch)
{
    return tee_pipe<TeeBranch>{teeBranch};
}
    
} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_TEE_HPP */
