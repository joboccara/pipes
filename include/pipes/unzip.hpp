#ifndef PIPES_UNZIP_HPP
#define PIPES_UNZIP_HPP

#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/transform.hpp"

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
