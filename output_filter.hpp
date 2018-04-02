#ifndef output_filter_h
#define output_filter_h

#include "helpers/meta.hpp"
#include <iterator>

namespace fluent
{
template<typename Iterator, typename Predicate>
class output_filter_iterator
{
public:    
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_filter_iterator(Iterator iterator, Predicate predicate) : iterator_(iterator), predicate_(predicate) {}
    output_filter_iterator& operator++(){ return *this; }
    output_filter_iterator& operator++(int){ ++*this; return *this; }
    output_filter_iterator& operator*(){ return *this; }
    template<typename T>
    output_filter_iterator& operator=(T const& value)
    {
        if (predicate_(value))
        {
            *iterator_ = value;
            ++iterator_;
        }
        return *this;
    }
private:
    Iterator iterator_;
    Predicate predicate_;
};

template<typename Predicate>
class output_filter
{
public:
    explicit output_filter(Predicate predicate) : predicate_(predicate) {}
    template<typename Iterator>
    output_filter_iterator<Iterator, Predicate> operator()(Iterator iterator) const
    {
        return output_filter_iterator<Iterator, Predicate>(iterator, predicate_);
    }
    
private:
    Predicate predicate_;
};

template<typename FilterFunction, typename Iterator>
output_filter_iterator<Iterator, FilterFunction> operator|(output_filter<FilterFunction> const& outputFilter, Iterator iterator)
{
    return outputFilter(iterator);
}

template<typename Predicate>
output_filter<Predicate> make_output_filter(Predicate predicate)
{
    return output_filter<Predicate>(predicate);
}
    
} // namespace fluent

#endif /* output_transformer_h */
