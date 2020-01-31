#ifndef PIPES_STEP_HPP
#define PIPES_STEP_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    class stride : public pipe_base
    {
    public:
        
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (nbSinceLastStride_ == 0)
            {
                send(FWD(values)..., FWD(tailPipeline));
            }
            nbSinceLastStride_++;

            if( nbSinceLastStride_ == strideSize_ )
			{
                nbSinceLastStride_ = 0;
            }
        }

        explicit stride(size_t strideSize) : strideSize_{strideSize}, nbSinceLastStride_{0} {}
        
    private:
        size_t strideSize_;
        size_t nbSinceLastStride_;
    };
} // namespace pipes

#endif /* PIPES_STEP_HPP */
