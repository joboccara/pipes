#ifndef PIPES_CARTESIAN_PRODUCT_HPP
#define PIPES_CARTESIAN_PRODUCT_HPP

#include "pipes/helpers/meta.hpp"
#include "pipes/impl/concepts.hpp"

namespace pipes
{
    template<typename... Ranges>
    struct cartesian_product_ranges
    {
        std::tuple<Ranges const&...> ranges;
        explicit cartesian_product_ranges(Ranges const&... ranges) : ranges(ranges...) {}
    };
    
    template<typename... Ranges>
    auto cartesian_product(Ranges&&... ranges)
    {
        static_assert(sizeof...(Ranges) > 0, "There should be at least one range in cartesian_product.");
        return cartesian_product_ranges<std::decay_t<Ranges>...>(FWD(ranges)...);
    }
    
    namespace detail
    {
        template<size_t I>
        struct increment_iterator
        {
            template<typename... Iterators>
            static void _(std::tuple<Iterators...>& iterators, std::tuple<Iterators...> const& beginIterators, std::tuple<Iterators...> const& endIterators)
            {
                auto& it = std::get<I>(iterators);
                auto const begin = std::get<I>(beginIterators);
                auto const end = std::get<I>(endIterators);
                
                ++it;
                
                if (it == end)
                {
                    it = begin;
                    increment_iterator<I - 1>::_(iterators, beginIterators, endIterators);
                }
            }
        };
        
        template<>
        struct increment_iterator<0>
        {
            template<typename... Iterators>
            static void _(std::tuple<Iterators...>& iterators, std::tuple<Iterators...> const&, std::tuple<Iterators...> const&)
            {
                auto& it = std::get<0>(iterators);
                
                ++it;
            }
        };

        
        template<typename... Iterators>
        void next_combination(std::tuple<Iterators...>& iterators, std::tuple<Iterators...> const& beginIterators, std::tuple<Iterators...> const& endIterators)
        {
            constexpr auto N = sizeof...(Iterators);
            increment_iterator<N - 1>::_(iterators, beginIterators, endIterators);
        }
    }
    
    template<typename... Ranges, typename Pipeline, detail::IsAPipeline<Pipeline> = true>
    void operator>>= (cartesian_product_ranges<Ranges...> rangesHolder, Pipeline&& pipeline)
    {
        auto const hasEmptyRange = detail::any_of(rangesHolder.ranges, [](auto&& range){ return range.size() == 0; });

        if (!hasEmptyRange)
        {
            auto const beginIterators = detail::transform(rangesHolder.ranges, [](auto&& range){ return begin(range); });
            auto const endIterators = detail::transform(rangesHolder.ranges, [](auto&& range){ return end(range); });
            
            for (auto iterators = beginIterators; std::get<0>(iterators) != std::get<0>(endIterators); detail::next_combination(iterators, beginIterators, endIterators))
            {
                sendTupleValues(detail::dereference(iterators), pipeline);
            }
        }
    }
}

#endif /* PIPES_CARTESIAN_PRODUCT_HPP */
