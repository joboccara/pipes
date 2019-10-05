#ifndef PIPES_STEP_HPP
#define PIPES_STEP_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    class step : public pipe_base
    {
    public:
        
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (nbSinceLastStep_ == 0)
            {
                send(FWD(values)..., FWD(tailPipeline));
            }
            nbSinceLastStep_++;

            if( nbSinceLastStep_ == stepSize_ ) {
                nbSinceLastStep_ = 0;
            }
        }

        explicit step(size_t stepSize) : stepSize_{stepSize}, nbSinceLastStep_{0} {}
        
    private:
        const size_t stepSize_;
        size_t nbSinceLastStep_;
    };
} // namespace pipes

#endif /* PIPES_STEP_HPP */
