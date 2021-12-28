#ifndef PIPES_TRANSFORM_HPP
#define PIPES_TRANSFORM_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/invoke.hpp"

#include <type_traits>

namespace pipes
{
    template<typename Function>
    class transform_pipe : public pipe_base
    {
    public:
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
#ifdef _MSC_VER
#if _MSVC_LANG >=  201703L
            send(std::invoke(function_.get(), FWD(values)...), tailPipeline);
#else
            send(details::invoke(function_.get(), FWD(values)...), tailPipeline);
#endif
#endif
        }
        
        explicit transform_pipe(Function function) : function_(function){}

    private:
        detail::assignable<Function> function_;
    };
    
    template<typename Function>
    transform_pipe<Function> transform(Function&& function)
    {
        return transform_pipe<Function>{function};
    }

} // namespace pipes

#endif /* PIPES_TRANSFORM_HPP */
