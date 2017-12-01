#ifndef dead_end_iterator_h
#define dead_end_iterator_h

#include <iterator>

namespace fluent
{

class dead_end_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    
    dead_end_iterator& operator++() { return *this; }
    dead_end_iterator& operator++(int) { return *this; }
    
    dead_end_iterator& operator*() { return *this; }
    
    template<typename T>
    dead_end_iterator& operator=(T const&) { return *this; }
};

} // namespace fluent

#endif /* dead_end_iterator_h */
