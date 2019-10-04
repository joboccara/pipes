#ifndef PIPES_TAP_HPP
#define PIPES_TAP_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    
    template<typename Function>
    class tap_pipe : public pipe_base
    {
    public:
        template<typename Value, typename TailPipeline>
        void onReceive(Value&& value, TailPipeline&& tailPipeline)
        {
            function_(FWD(value));
            send(FWD(value), tailPipeline);
        }
        
        explicit tap_pipe(Function function) : function_(function) {}
        
    private:
        detail::assignable<Function> function_;
    };
    
    template <typename Function>
    tap_pipe<Function> tap(Function function) {
        return tap_pipe<Function>(function);
    }
    
} // namespace pipes

#endif /* PIPES_TAP_HPP */
