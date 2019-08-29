#ifndef PIPES_TEE_HPP
#define PIPES_TEE_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/FWD.hpp"

#include <type_traits>

namespace pipes
{
    
template<typename TeeBranch, typename PipelineTail>
class tee_pipeline : public pipeline_base<tee_pipeline<TeeBranch, PipelineTail>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        send(teeBranch_, FWD(value));
        send(pipelineTail_, FWD(value));
    }
    
    tee_pipeline(TeeBranch const& teeBranch, PipelineTail const& pipelineTail) : teeBranch_(teeBranch), pipelineTail_(pipelineTail){}

private:
    TeeBranch teeBranch_;
    PipelineTail pipelineTail_;
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


#endif /* PIPES_TEE_HPP */
