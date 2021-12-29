#ifndef PIPES_DEAD_END_ITERATOR_HPP
#define PIPES_DEAD_END_ITERATOR_HPP

#include "pipes/base.hpp"

namespace pipes
{
    
    class dev_null : public pipeline_base<dev_null>
    {
    public:
        template<typename T>
        void onReceive(T&&)
        {
            //do nothing
        }
    };
    
} // namespace pipes

#endif /* PIPES_DEAD_END_ITERATOR_HPP */
