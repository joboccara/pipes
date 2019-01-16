#ifndef output_transform_h
#define output_transform_h

#include "../helpers/meta.hpp"

namespace fluent
{

template<typename TransformFunctionTuple, typename... Iterators>
class output_transform_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_transform_iterator(TransformFunctionTuple transformFunctionTuple, Iterators... iterators) : iterators_(iterators...), transformFunctionTuple_(transformFunctionTuple) {}
    output_transform_iterator& operator++()
    {
        detail::apply([](auto&& iterator){ ++iterator; }, iterators_);
        return *this;
    }
    output_transform_iterator& operator++(int){ ++*this; return *this; }
    output_transform_iterator& operator*(){ return *this; }
    template<typename T>
    output_transform_iterator& operator=(T const& value)
    {
        detail::apply2([&value](auto&& function, auto&& iterator){ *iterator = function(value); }, transformFunctionTuple_, iterators_);
        return *this;
    }
private:
    std::tuple<Iterators...> iterators_;
    TransformFunctionTuple transformFunctionTuple_;
};

template<typename... TransformFunctions>
class output_transformer
{
public:
    explicit output_transformer(TransformFunctions... transformFunctions) : transformFunctionsTuple_(transformFunctions...) {}
    template<typename... Iterators>
    output_transform_iterator<std::tuple<TransformFunctions...>, Iterators...> operator()(Iterators... iterators) const
    {
        return output_transform_iterator<std::tuple<TransformFunctions...>, Iterators...>(transformFunctionsTuple_, iterators...);
    }
    
private:
    std::tuple<TransformFunctions...> transformFunctionsTuple_;
};
    
template<typename TransformFunction, typename Iterator>
    output_transform_iterator<std::tuple<TransformFunction>, Iterator> operator>>=(output_transformer<TransformFunction> const& outputTransformer, Iterator iterator)
{
    return outputTransformer(iterator);
}

namespace output
{

template<typename... TransformFunctions>
output_transformer<TransformFunctions...> transform(TransformFunctions... transformFunctions)
{
    return output_transformer<TransformFunctions...>(transformFunctions...);
}

} // namespace output

} // namespace fluent

#endif /* output_transformer_h */
