#ifndef PIPES_INVOKE_HPP
#define PIPES_INVOKE_HPP

#include <functional>
#include <type_traits>

namespace pipes
{
    namespace detail
    {
#if defined(_MSC_VER) && _MSVC_LANG >=  201703L
        template<typename Functor, typename... Args>
        typename std::enable_if<
            std::is_member_pointer<typename std::decay<Functor>::type>::value,
            typename std::invoke_result<Functor && (Args&&...)>::type
        >::type invoke(Functor&& f, Args&&... args)
        {
            return std::mem_fn(f)(std::forward<Args>(args)...);
        }

        template<typename Functor, typename... Args>
        typename std::enable_if<
            !std::is_member_pointer<typename std::decay<Functor>::type>::value,
            typename std::invoke_result<Functor && (Args&&...)>::type
        >::type invoke(Functor&& f, Args&&... args)
        {
            return std::forward<Functor>(f)(std::forward<Args>(args)...);
        }
#else
        template<typename Functor, typename... Args>
        typename std::enable_if<
            std::is_member_pointer<typename std::decay<Functor>::type>::value,
            typename std::result_of<Functor && (Args&&...)>::type
        >::type invoke(Functor&& f, Args&&... args)
        {
            return std::mem_fn(f)(std::forward<Args>(args)...);
        }

        template<typename Functor, typename... Args>
        typename std::enable_if<
            !std::is_member_pointer<typename std::decay<Functor>::type>::value,
            typename std::result_of<Functor && (Args&&...)>::type
        >::type invoke(Functor&& f, Args&&... args)
        {
            return std::forward<Functor>(f)(std::forward<Args>(args)...);
        }
#endif

    }
}

#endif /* PIPES_INVOKE_HPP */
