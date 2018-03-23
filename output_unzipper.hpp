#ifndef output_unzipper_h
#define output_unzipper_h

#include "helpers/meta.hpp"

namespace fluent
{

template<typename... Iterators>
class output_unzip_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_unzip_iterator(Iterators... iterators) : iterators_(std::make_tuple(iterators...)) {}
    output_unzip_iterator& operator++()
    {
        detail::apply([](auto&& iterator){ ++iterator; }, iterators_);
        return *this;
    }
    output_unzip_iterator& operator++(int){ ++*this; return *this; }
    output_unzip_iterator& operator*(){ return *this; }
    template<typename... Ts>
    output_unzip_iterator& operator=(std::tuple<Ts...> const& values)
    {
        detail::apply2([](auto&& value, auto&& iterator){ *iterator = value; }, values, iterators_);
        return *this;
    }
    template<typename First, typename Second>
    output_unzip_iterator& operator=(std::pair<First, Second> const& values)
    {
        *std::get<0>(iterators_) = values.first;
        *std::get<1>(iterators_) = values.second;
        return *this;
    }
private:
    std::tuple<Iterators...> iterators_;
};

template<typename... Iterators>
output_unzip_iterator<Iterators...> output_unzipper(Iterators... iterators)
{
    return output_unzip_iterator<Iterators...>(iterators...);
}

} // namespace fluent

#endif /* output_unzipper_h */
