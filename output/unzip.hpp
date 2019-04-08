#ifndef output_unzip_h
#define output_unzip_h

#include "../helpers/FWD.hpp"
#include "../helpers/meta.hpp"
#include "transform.hpp"

#include <tuple>
#include <utility>

namespace pipes
{
namespace output
{

template<size_t... Is>
auto make_transform(std::index_sequence<Is...> const&)
{
    return transform([](auto&& tup){return std::get<Is>(FWD(tup));}...);
}

template<typename... Iterators>
auto unzip(Iterators... iterators)
{
    return make_transform(std::index_sequence_for<Iterators...>{})(iterators...);
}
    
} // namespace output
} // namespace pipes

#endif /* output_unzipper_h */
