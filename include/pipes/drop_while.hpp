#ifndef PIPES_DROP_WHILE_HPP
#define PIPES_DROP_WHILE_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    template<typename Predicate>
    class drop_while_pipe : public pipe_base
    {
    public:
        
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (!predicateHasBeenFalse_)
            {
                predicateHasBeenFalse_ = !predicate_(values...);
            }
            
            if (predicateHasBeenFalse_)
            {
                send(FWD(values)..., FWD(tailPipeline));
            }
        }
        
        explicit drop_while_pipe(Predicate predicate) : predicate_{predicate}, predicateHasBeenFalse_{false} {}
        
    private:
        detail::assignable<Predicate> predicate_;
        bool predicateHasBeenFalse_;
    };
    
    template<typename Predicate>
    drop_while_pipe<Predicate> drop_while(Predicate&& predicate)
    {
        return drop_while_pipe<Predicate>(FWD(predicate));
    }
} // namespace pipes

#endif /* PIPES_DROP_WHILE_HPP */
