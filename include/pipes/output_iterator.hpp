#ifndef PIPES_OUTPUT_ITERATOR_HPP
#define PIPES_OUTPUT_ITERATOR_HPP

#include "pipes/helpers/crtp.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <iterator>

namespace pipes
{
    
template<typename OutputIterator, typename T>
void send(OutputIterator& outputIterator, T&& value)
{
    *outputIterator = FWD(value);
    ++outputIterator;
}

template<typename Derived>
struct OutputIteratorBase : detail::crtp<Derived, OutputIteratorBase>
{
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    Derived& operator++() { return this->derived(); }
    Derived& operator++(int){ ++this->derived(); return this->derived(); }
    Derived& operator*() { return this->derived(); }
    
    template<typename T>
    Derived& operator=(T&& input)
    {
        this->derived().onReceive(FWD(input));
        return this->derived();
    }
};

} // namespace pipes

#endif /* PIPES_OUTPUT_ITERATOR_HPP */
