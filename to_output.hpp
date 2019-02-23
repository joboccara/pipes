#ifndef output_iterator_h
#define output_iterator_h

namespace fluent
{

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
void operator>>=(Range&& range, output_to_iterator<Iterator> const& outputToIterator)
{
    std::copy(begin(range), end(range), outputToIterator.get());
}

} // namespace fluent

#endif /* output_demuxer_h */
