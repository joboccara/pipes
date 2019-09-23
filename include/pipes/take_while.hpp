#ifndef PIPES_TAKE_WHILE_HPP
#define PIPES_TAKE_WHILE_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/assignable.hpp"

namespace pipes
{
    template<typename Predicate>
    class take_while_pipe : public pipe_base
    {
    public:
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (!predicateHasBeenFalse_)
            {
                if (predicate_(values...))
                {
                    send(FWD(values)..., tailPipeline);
                }
                else
                {
                    predicateHasBeenFalse_ = true;
                }
            }
        }
        
        explicit take_while_pipe(Predicate predicate) : predicate_{predicate}, predicateHasBeenFalse_{false} {}
        
    private:
        detail::assignable<Predicate> predicate_;
        bool predicateHasBeenFalse_;
    };
    
    template<typename Predicate>
    inline take_while_pipe<Predicate> take_while(Predicate predicate)
    {
        return take_while_pipe<Predicate>{predicate};
    }
    
} // namespace pipes

#endif /* PIPES_TAKE_WHILE_HPP */
