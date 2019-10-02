#ifndef HAS_ON_RECEIVE_HPP
#define HAS_ON_RECEIVE_HPP

#include <type_traits>

#include "pipes/base.hpp"

namespace pipes
{
namespace detail
{

/*
** Helper
*/
template<typename... Ts>
struct sfinae_empty_helper {};

/*
** Dummies
*/

struct dummy_value_input_type {};

struct dummy_tail_pipeline_type: public pipeline_base<dummy_tail_pipeline_type> {};

/*
** Detect has a right onReceive
*/

template<typename T, typename _ = void>
struct has_onReceive_method: std::false_type {};

template<typename T>
struct has_onReceive_method
<
    T,
    std::conditional_t
    <
        false,
        sfinae_empty_helper
        <
            decltype(std::declval<T>().template onReceive<dummy_value_input_type>(dummy_value_input_type{}, dummy_tail_pipeline_type{}))
        >,
        void
    >
> : public std::true_type {};

}
}

#endif