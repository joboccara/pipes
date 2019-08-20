#ifndef PIPES_OUTPUT_ITERATOR_HPP
#define PIPES_OUTPUT_ITERATOR_HPP

#include "pipes/helpers/crtp.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>

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

namespace detail
{
namespace adl
{
    using std::begin;
    using std::end;
    template<typename T>
    using begin_expression = decltype(begin(std::declval<T>()));
    template<typename T>
    using end_expression = decltype(end(std::declval<T>()));
}
template<typename Range>
    constexpr bool range_expression_detected = detail::is_detected<adl::begin_expression, Range> && detail::is_detected<adl::end_expression, Range>;
    
template<typename Range>
using IsARange = std::enable_if_t<range_expression_detected<Range>, bool>;
    
template<typename OutputIterator>
using IsAnOutputIterator = std::enable_if_t<std::is_same<typename OutputIterator::iterator_category, std::output_iterator_tag>::value, bool>;
} // namespace detail
    
template<typename Range, typename OutputIterator,
detail::IsARange<std::remove_reference_t<Range>> = true,
detail::IsAnOutputIterator<std::remove_reference_t<OutputIterator>> = true>
void operator>>=(Range&& range, OutputIterator&& outputIterator)
{
    std::copy(begin(range), end(range), outputIterator);
}
<<<<<<< HEAD
=======


>>>>>>> ff639e4b35aec31ed6a096776f26d94c978eb1ba
} // namespace pipes

#endif /* PIPES_OUTPUT_ITERATOR_HPP */
