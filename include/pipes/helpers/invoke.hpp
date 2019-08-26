#ifndef PIPES_INVOKE_HPP
#define PIPES_INVOKE_HPP

#include <functional>
#include <type_traits>

namespace pipes
{
    namespace detail
    {
        template<typename Functor, typename... Args>
        typename std::enable_if<
        std::is_member_pointer<typename std::decay<Functor>::type>::value,
        typename std::result_of<Functor&&(Args&&...)>::type
        >::type invoke(Functor&& f, Args&&... args)
        {
            return std::mem_fn(f)(std::forward<Args>(args)...);
        }
        
        template<typename Functor, typename... Args>
        typename std::enable_if<
        !std::is_member_pointer<typename std::decay<Functor>::type>::value,
        typename std::result_of<Functor&&(Args&&...)>::type
        >::type invoke(Functor&& f, Args&&... args)
        {
            return std::forward<Functor>(f)(std::forward<Args>(args)...);
        }
    }
}

#endif /* PIPES_INVOKE_HPP */
