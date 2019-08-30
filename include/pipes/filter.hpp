#ifndef PIPES_FILTER_HPP
#define PIPES_FILTER_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/pipeline_base.hpp"

namespace pipes
{
    template<typename Predicate>
    class filter_pipe : public pipe_base
    {
    public:
        template<typename Value, typename TailPipeline>
        void onReceive(Value&& value, TailPipeline&& tailPipeline)
        {
            if (predicate_(FWD(value)))
            {
                send(tailPipeline, FWD(value));
            }
        }
        
        explicit filter_pipe(Predicate predicate) : predicate_(predicate){}
        
    private:
        detail::assignable<Predicate> predicate_;
    };
    
    template<typename Predicate>
    filter_pipe<std::decay_t<Predicate>> filter(Predicate&& predicate)
    {
        return filter_pipe<std::decay_t<Predicate>>{predicate};
    }
    
} // namespace pipes


#endif /* PIPES_FILTER_HPP */
