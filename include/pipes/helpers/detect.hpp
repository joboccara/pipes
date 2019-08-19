#ifndef DETECT_HPP
#define DETECT_HPP

#include <type_traits>

namespace pipes
{
namespace detail
{

template<typename...>
using try_to_instantiate = void;

using disregard_this = void;

template<template<typename...> class Expression, typename Attempt, typename... Ts>
struct is_detected_impl : std::false_type{};

template<template<typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, try_to_instantiate<Expression<Ts...>>, Ts...> : std::true_type{};

template<template<typename...> class Expression, typename... Ts>
constexpr bool is_detected = is_detected_impl<Expression, disregard_this, Ts...>::value;

}
}

#endif /* DETECT_HPP */
