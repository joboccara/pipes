#ifndef fluent_meta_h
#define fluent_meta_h

#include <iterator>
#include <type_traits>

namespace fluent
{

template<typename Iterator>
using IsOutputIterator = std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::output_iterator_tag>;
    
}

#endif /* fluent_meta */
