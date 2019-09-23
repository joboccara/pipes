#ifndef PIPES_TEE_HPP
#define PIPES_TEE_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

#include <type_traits>

namespace pipes
{
    template<typename TeeBranch>
    class tee_pipe : public pipe_base
    {
    public:
        template<typename Value, typename TailPipeline>
        void onReceive(Value&& value, TailPipeline&& tailPipeline)
        {
            send(value, teeBranch_);
            send(FWD(value), tailPipeline);
        }
        
        explicit tee_pipe(TeeBranch teeBranch) : teeBranch_(teeBranch){}
        
    private:
        TeeBranch teeBranch_;
    };
    
    template<typename TeeBranch>
    tee_pipe<std::decay_t<TeeBranch>> tee(TeeBranch&& predicate)
    {
        return tee_pipe<std::decay_t<TeeBranch>>{predicate};
    }
    
} // namespace pipes


#endif /* PIPES_TEE_HPP */
