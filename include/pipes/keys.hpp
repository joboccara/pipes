#ifndef PIPES_KEYS_HPP
#define PIPES_KEYS_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/meta.hpp"

#include <type_traits>
#include <utility>

namespace pipes
{
    class keys : public pipe_base
    {
    public:
        template<typename Pair, typename TailPipeline>
        void onReceive(Pair&& value, TailPipeline&& tailPipeline)
        {
            static_assert(detail::is_pair_v<Pair>);
            send(FWD(value).first, tailPipeline);
        }
    };

} // namespace pipes

#endif
