#ifndef PIPES_VALUES_HPP
#define PIPES_VALUES_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/meta.hpp"

#include <type_traits>
#include <utility>

namespace pipes
{
    class values : public pipe_base
    {
    public:
        template<typename Pair, typename TailPipeline>
        void onReceive(Pair&& value, TailPipeline&& tailPipeline)
        {
            static_assert(detail::is_pair_v<Pair>);
            send(FWD(value).second, tailPipeline);
        }
    };

} // namespace pipes

#endif /* PIPES_VALUES_HPP */
