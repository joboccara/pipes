#ifndef PIPES_DEAD_END_ITERATOR_HPP
#define PIPES_DEAD_END_ITERATOR_HPP

#include <iterator>
#include "output_iterator.hpp"

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
        using OutputIteratorBase<dev_null>::operator=;
    };
    
} // namespace fluent

#endif /* PIPES_DEAD_END_ITERATOR_HPP */
