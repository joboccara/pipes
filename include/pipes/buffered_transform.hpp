#ifndef PIPES_BUFFERED_TRANSFORM_HPP
#define PIPES_BUFFERED_TRANSFORM_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/base.hpp"

namespace pipes
{
    template<typename Buffered, typename Function>
    class buffered_transform_pipe : public pipe_base
    {
    public:
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
#if defined(_MSC_VER) && _MSVC_LANG >=  201703L
            send(std::invoke(Fn_.get(), FWD(values)..., buffered_), tailPipeline);
#else
            send(details::invoke(Fn_.get(), FWD(values)..., buffered_), tailPipeline);
#endif
        }

        template <typename BufferedTypeFwd, typename FunctionFwd>
        explicit buffered_transform_pipe(BufferedTypeFwd&& buffered, FunctionFwd&& binary_fn) :
            buffered_(std::forward<BufferedTypeFwd>(buffered)),
            Fn_(std::forward<FunctionFwd>(binary_fn)) {}

    private:
        Buffered buffered_;
        detail::assignable<Function> Fn_;
    };

    template<typename InitType, typename Function>
    auto buffered_transform(InitType&& init, Function&& binary_fn)
    {
        return buffered_transform_pipe<std::decay_t<InitType>, std::decay_t<Function>>{
        	std::forward<InitType>(init),
        	std::forward<Function>(binary_fn)
        };
    }

} // namespace pipes


#endif /* PIPES_BUFFERED_TRANSFORM_HPP */
