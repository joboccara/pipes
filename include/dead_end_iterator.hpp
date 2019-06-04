#ifndef dead_end_iterator_h
#define dead_end_iterator_h

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

#endif /* dead_end_iterator_h */
