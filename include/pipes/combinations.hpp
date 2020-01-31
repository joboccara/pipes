#ifndef PIPES_COMBINATIONS_HPP
#define PIPES_COMBINATIONS_HPP

#include <tuple>
#include <utility>
#include <vector>

namespace pipes
{
    namespace detail
    {
        template<typename ForwardRange>
        struct combinations_range
        {
            ForwardRange const& range;
            explicit combinations_range(ForwardRange const&range) : range(range) {}
        };
    
        template<typename ForwardIterator, typename Sentinel>
        std::pair<ForwardIterator, ForwardIterator> next_combination(ForwardIterator first, ForwardIterator second, Sentinel end)
        {
           if(std::next(second) != end)
           {
              return { first, std::next(second) };
           }
           else if (std::next(first) != end && std::next(first, 2) != end)
           {
              return { std::next(first), std::next(first, 2) };
           }
           else
           {
              return { end, end };
           }
        }
    }

    template<typename ForwardRange>
    auto combinations(ForwardRange&& range)
    {
        return detail::combinations_range<std::decay_t<ForwardRange>>(FWD(range));
    }
    
    template<typename ForwardRange, typename Pipeline, detail::IsAPipeline<Pipeline> = true>
    void operator>>=(detail::combinations_range<ForwardRange> rangeHolder, Pipeline&& pipeline)
    {
       auto&& range = rangeHolder.range;
       if (begin(range) == end(range)) return;

       auto first = begin(range);
       auto second = std::next(begin(range));
       for (; first != end(range); std::tie(first, second) = detail::next_combination(first, second, end(range)))
       {
          send(*first, *second, pipeline);
       }
    }
}

#endif /* PIPES_COMBINATIONS_HPP */
