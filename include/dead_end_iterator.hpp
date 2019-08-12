#ifndef PIPES_DEAD_END_ITERATOR_HPP
#define PIPES_DEAD_END_ITERATOR_HPP

#include <iterator>
#include "output_iterator.hpp"

namespace pipes
{
    
    class dead_end_iterator : public OutputIteratorBase<dead_end_iterator>
    {
    public:
        template<typename T>
        void onReceive(T const&)
        {
            //do nothing
        }
        
    public: // but technical
        using OutputIteratorBase<dead_end_iterator>::operator=;
    };
    
} // namespace fluent

#endif /* PIPES_DEAD_END_ITERATOR_HPP */
