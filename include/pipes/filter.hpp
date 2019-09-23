#ifndef PIPES_FILTER_HPP
#define PIPES_FILTER_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/base.hpp"

namespace pipes
{
    template<typename Predicate>
    class filter_pipe : public pipe_base
    {
    public:
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (predicate_(values...))
            {
                send(FWD(values)..., tailPipeline);
            }
        }
        
        explicit filter_pipe(Predicate predicate) : predicate_(predicate){}
        
    private:
        detail::assignable<Predicate> predicate_;
    };
    
    template<typename Predicate>
    auto filter(Predicate&& predicate)
    {
        return filter_pipe<std::decay_t<Predicate>>{predicate};
    }
    
} // namespace pipes


#endif /* PIPES_FILTER_HPP */
