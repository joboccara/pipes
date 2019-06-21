#ifndef PIPES_UNZIP_HPP
#define PIPES_UNZIP_HPP

#include "helpers/FWD.hpp"
#include "helpers/meta.hpp"
#include "transform.hpp"

#include <tuple>
#include <utility>

namespace pipes
{

template<size_t... Is>
auto make_transform(std::index_sequence<Is...> const&)
{
    return transform([](auto&& tup){return std::get<Is>(FWD(tup));}...);
}

template<typename... OutputPipes>
auto unzip(OutputPipes... outputPipes)
{
    return make_transform(std::index_sequence_for<OutputPipes...>{})(outputPipes...);
}
    
} // namespace pipes

#endif /* PIPES_UNZIP_HPP */
