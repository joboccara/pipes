#ifndef PIPES_DEAD_END_ITERATOR_HPP
#define PIPES_DEAD_END_ITERATOR_HPP

#include "pipes/operator.hpp"

#include "pipes/output_iterator.hpp"
#include <iterator>

namespace pipes
{
    
    class dev_null : public OutputIteratorBase<dev_null>
    {
    public:
        template<typename T>
        void onReceive(T const&)
        {
            //do nothing
        }
        
    public: // but technical
        using base = OutputIteratorBase<dev_null>;
        using base::operator=;
    };
    
} // namespace fluent

#endif /* PIPES_DEAD_END_ITERATOR_HPP */
