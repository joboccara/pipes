#ifndef PIPES_INTERSPERSE_HPP
#define PIPES_INTERSPERSE_HPP

#include "pipes/drop.hpp"
#include "pipes/join.hpp"
#include "pipes/transform.hpp"
#include "pipes/helpers/FWD.hpp"

#include <array>
#include <type_traits>

namespace pipes
{
    template<typename Delimiter>
    auto intersperse(Delimiter&& delimiter)
    {
        return pipes::transform([delimiter = FWD(delimiter)](auto&& value) { return std::array<std::decay_t<decltype(value)>, 2>{delimiter, FWD(value)}; })
           >>= pipes::join
           >>= pipes::drop(1);
    }
}

#endif /* PIPES_INTERSPERSE_HPP */
