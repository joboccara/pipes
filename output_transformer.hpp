#ifndef output_transformer_h
#define output_transformer_h

namespace fluent
{

template<typename Iterator, typename TransformFunction>
class output_transform_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_transform_iterator(Iterator iterator, TransformFunction transformFunction) : iterator_(iterator), transformFunction_(transformFunction) {}
    output_transform_iterator& operator++(){ ++iterator_; return *this; }
    output_transform_iterator& operator++(int){ ++*this; return *this; }
    output_transform_iterator& operator*(){ return *this; }
    template<typename T>
    output_transform_iterator& operator=(T const& value)
    {
        *iterator_ = transformFunction_(value);
        return *this;
    }
private:
    Iterator iterator_;
    TransformFunction transformFunction_;
};

template<typename TransformFunction>
class output_transformer
{
public:
    explicit output_transformer(TransformFunction transformFunction) : transformFunction_(transformFunction) {}
    template<typename Iterator>
    output_transform_iterator<Iterator, TransformFunction> operator()(Iterator iterator) const
    {
        return output_transform_iterator<Iterator, TransformFunction>(iterator, transformFunction_);
    }
    
private:
    TransformFunction transformFunction_;
};
    
template<typename TransformFunction, typename Iterator>
output_transform_iterator<Iterator, TransformFunction> operator|(output_transformer<TransformFunction> const& outputTransformer, Iterator iterator)
{
    return outputTransformer(iterator);
}

template<typename TransformFunction>
output_transformer<TransformFunction> make_output_transformer(TransformFunction transformFunction)
{
    return output_transformer<TransformFunction>(transformFunction);
}

} // namespace fluent

#endif /* output_transformer_h */
