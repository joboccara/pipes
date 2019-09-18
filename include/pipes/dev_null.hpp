#ifndef PIPES_DEAD_END_ITERATOR_HPP
#define PIPES_DEAD_END_ITERATOR_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include <iterator>

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
    
} // namespace fluent

#endif /* PIPES_DEAD_END_ITERATOR_HPP */
