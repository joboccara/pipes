#ifndef output_unzip_h
#define output_unzip_h

#include "../helpers/FWD.hpp"
#include "../helpers/meta.hpp"
#include "transform.hpp"

#include <tuple>
#include <utility>

namespace fluent
{
namespace output
{

template <size_t I>
struct Get
{
    template <typename Tuple>
    decltype(auto) operator()(Tuple&& tuple)
    {
        return std::get<I>(FWD(tuple));
    }
};
    
template<size_t... Is>
auto make_transform(std::index_sequence<Is...> const&)
{
    return fluent::output::transform(Get<Is>{}...);
}
    
template<typename... Iterators>
auto unzip(Iterators... iterators)
{
    return make_transform(std::make_index_sequence<sizeof...(Iterators)>{})(iterators...);
}
    
} // namespace output
} // namespace fluent

#endif /* output_unzipper_h */
