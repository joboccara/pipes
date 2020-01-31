#ifndef PIPES_META_HPP
#define PIPES_META_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace pipes
{
    namespace detail
    {
        
        template <class Tuple, class F, std::size_t... I>
        F for_each_impl(Tuple&& t, F&& f, std::index_sequence<I...>)
        {
            return (void)std::initializer_list<int>{(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))),0)...}, f;
        }
        
        template <class Tuple1, class Tuple2, class F, std::size_t... I>
        F for_each2_impl(Tuple1&& t1, Tuple2&& t2, F&& f, std::index_sequence<I...>)
        {
            return (void)std::initializer_list<int>{(std::forward<F>(f)(std::get<I>(std::forward<Tuple1>(t1)), std::get<I>(std::forward<Tuple2>(t2))),0)...}, f;
        }
        
        template <class F, class Tuple>
        constexpr decltype(auto) for_each(Tuple&& t, F&& f)
        {
            return detail::for_each_impl(std::forward<Tuple>(t),std::forward<F>(f),
                                         std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
        }
        
        template <class Tuple1, class Tuple2, class F>
        constexpr decltype(auto) for_each2(Tuple1&& t1, Tuple2&& t2, F&& f)
        {
            return detail::for_each2_impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<F>(f),
                                          std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple1>>::value>{});
        }
        
        template<typename...Ts, typename Function, size_t... Is>
        auto transform_impl(std::tuple<Ts...> const& inputs, Function function, std::index_sequence<Is...>)
        {
            return std::make_tuple(function(std::get<Is>(inputs))...);
        }
        
        template<typename... Ts, typename Function>
        auto transform(std::tuple<Ts...> const& inputs, Function function)
        {
            return transform_impl(inputs, function, std::make_index_sequence<sizeof...(Ts)>{});
        }
        
        template<typename Tuple, typename Predicate>
        size_t find_if(Tuple&& tuple, Predicate pred)
        {
            size_t index = std::tuple_size<std::decay_t<Tuple>>::value;
            size_t currentIndex = 0;
            bool found = false;
            for_each(tuple, [&](auto&& value)
                     {
                         if (!found && pred(value))
                         {
                             ++index = currentIndex;
                             found = true;
                         }
                         ++currentIndex;
                     });
            return index;
        }
        
        template<typename Tuple, typename Predicate>
        bool any_of(Tuple&& tuple, Predicate pred)
        {
            return find_if(tuple, pred) != std::tuple_size<std::decay_t<Tuple>>::value;
        }

        template<typename Tuple, typename Action>
        void perform(Tuple&& tuple, size_t index, Action action)
        {
            size_t currentIndex = 0;
            for_each(tuple, [action = std::move(action), index, &currentIndex](auto&& value)
                     {
                         if (currentIndex == index)
                         {
                             action(std::forward<decltype(value)>(value));
                         }
                         ++currentIndex;
                     });
        }
        
        template<typename... Ts>
        auto dereference(std::tuple<Ts...> const& tuple)
        {
            return transform(tuple, [](auto&& element) -> decltype(auto) { return *element; });
        }
        
    }  // namespace detail
}  // namespace pipes


#endif /* PIPES_META_HPP */
