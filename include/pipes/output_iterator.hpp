#ifndef PIPES_OUTPUT_ITERATOR_HPP
#define PIPES_OUTPUT_ITERATOR_HPP

#include "pipes/helpers/crtp.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <algorithm>
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
template<typename T>
using begin_expression = decltype(std::begin(std::declval<T>()));
template<typename T>
using end_expression = decltype(std::end(std::declval<T>()));
template<typename Range>
constexpr bool range_expression_detected = detail::is_detected<begin_expression, Range> && detail::is_detected<end_expression, Range>;
    
template<typename Range>
using IsARange = std::enable_if_t<range_expression_detected<Range>, bool>;
    
template<typename OutputIterator>
using IsAnOutputIterator = std::enable_if_t<std::is_same<typename OutputIterator::iterator_category, std::output_iterator_tag>::value, bool>;
} // namespace detail
    
template<typename Range, typename OutputIterator, detail::IsARange<Range> = true, detail::IsAnOutputIterator<OutputIterator> = true>
void operator>>=(Range&& range, OutputIterator&& outputIterator)
{
    std::copy(begin(range), end(range), outputIterator);
}
    
} // namespace pipes

#endif /* PIPES_OUTPUT_ITERATOR_HPP */
