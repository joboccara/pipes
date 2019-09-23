#ifndef PIPES_TAKE_HPP
#define PIPES_TAKE_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    class take : public pipe_base
    {
    public:
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (nbTaken_ < nbToTake_)
            {
                send(FWD(values)..., tailPipeline);
                ++nbTaken_;
            }
        }
        
        explicit take(size_t nbToTake) : nbToTake_{nbToTake}, nbTaken_{0} {}
        
    private:
        size_t nbToTake_;
        size_t nbTaken_;
    };
} // namespace pipes

#endif /* PIPES_TAKE_HPP */
