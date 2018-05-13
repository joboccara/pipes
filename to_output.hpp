#ifndef output_iterator_h
#define output_iterator_h

namespace fluent
{

/*
template<typename Iterator>
class output_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_iterator(Iterator iterator) : iterator_(std::move(iterator)) {}
    output_iterator& operator++() { return *this; }
    output_iterator& operator++(int){ ++*this; return *this; }
    output_iterator& operator*(){ return *this; }
    template<typename T>
    output_iterator& operator=(T&& value)
    {
        *iterator_ = std::forward<T>(value);
        ++iterator_;
        return *this;
    }
    
    output_iterator& operator=(output_iterator const&) = default;
    output_iterator& operator=(output_iterator&&) = default;
    output_iterator(output_iterator const&) = default;
    output_iterator(output_iterator&&) = default;
    
private:
    Iterator iterator_;
};

template<typename Iterator>
output_iterator<Iterator> output(Iterator&& iterator)
{
    return output_iterator<Iterator>(std::forward<Iterator>(iterator));
}

*/

struct to_output_t {};
const to_output_t to_output{};

template<typename Iterator>
class output_to_iterator
{
public:
    explicit output_to_iterator(Iterator iterator) : iterator_(iterator) {}
    Iterator get() const { return iterator_; }
private:
    Iterator iterator_;
};

template<typename Iterator>
output_to_iterator<Iterator> operator>>=(to_output_t, Iterator iterator)
{
    return output_to_iterator<Iterator>(iterator);
}

template<typename Range, typename Iterator>
void operator>>=(Range const& range, output_to_iterator<Iterator> const& outputToIterator)
{
    std::copy(begin(range), end(range), outputToIterator.get());
}

} // namespace fluent

#endif /* output_demuxer_h */
