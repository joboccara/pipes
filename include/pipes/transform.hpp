#ifndef PIPES_TRANSFORM_HPP
#define PIPES_TRANSFORM_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/invoke.hpp"

#include <type_traits>

namespace pipes
{
    template<typename Function>
    class transform_pipe : public pipe_base
    {
    public:
        template<typename Value, typename TailPipeline>
        void onReceive(Value&& value, TailPipeline&& tailPipeline)
        {
            send(detail::invoke(function_.get(), FWD(value)), tailPipeline);
        }
        
        explicit transform_pipe(Function function) : function_(function){}

    private:
        detail::assignable<Function> function_;
    };
    
    template<typename Function>
    transform_pipe<std::decay_t<Function>> transform(Function&& function)
    {
        return transform_pipe<std::decay_t<Function>>{function};
    }

} // namespace pipes

#endif /* PIPES_TRANSFORM_HPP */
