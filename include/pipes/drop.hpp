#ifndef PIPES_DROP_HPP
#define PIPES_DROP_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    class drop : public pipe_base
    {
    public:
        
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (nbDropped_ == nbToDrop_)
            {
                send(FWD(values)..., FWD(tailPipeline));
            }
            else
            {
                ++nbDropped_;
            }
        }
        
        explicit drop(size_t nbToDrop) : nbToDrop_{nbToDrop}, nbDropped_{0} {}
        
    private:
        size_t nbToDrop_;
        size_t nbDropped_;
    };
} // namespace pipes

#endif /* PIPES_DROP_HPP */
