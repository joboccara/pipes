#ifndef HAS_ON_RECEIVE_HPP
#define HAS_ON_RECEIVE_HPP

#include <type_traits>

namespace pipes
{
namespace detail
{

template<typename... Ts>
struct sfinae_empty_helper {};

struct dummy_value_input_type {};
struct dummy_tail_pipeline_type {};

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