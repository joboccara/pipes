#ifndef PIPES_OPTIONAL_HPP
#define PIPES_OPTIONAL_HPP

#include <type_traits>

namespace pipes
{
namespace detail
{
    struct nullopt_t {};
    static const nullopt_t nullopt;

    template<typename T>
    class optional
    {
    public:
        optional(const T& object) : m_initialized(true) { new (&m_object) T(object); };
        optional(nullopt_t) : m_initialized(false) {};
        optional() : m_initialized(false) {};
        optional(const optional& other) : m_initialized(other.m_initialized) { if (other.m_initialized) new (&m_object) T(*other); }
        optional& operator=(const optional& other)
        {
            reset();
            if (other) {
                new (&m_object) T(*other);
                m_initialized = other.m_initialized;
            }
            return *this;
        }
        optional& operator=(nullopt_t) { reset(); return *this; }
        ~optional() { reset(); }
        template<typename... Args>
        void emplace(Args&&... args)
        {
            reset();
            new (&m_object) T(std::forward<Args>(args)...);
            m_initialized = true;
        }
        operator bool() const { return m_initialized; };
        T& operator*() { return *reinterpret_cast<T*>(&m_object); };
        const T& operator*() const { return *reinterpret_cast<const T*>(&m_object); };
        T* operator->() { return &**this; };
        const T* operator->() const { return &**this; };
    private:
        void reset() { if (m_initialized) (&**this)->~T(); m_initialized = false; }
    private:
        std::aligned_storage_t<sizeof(T)> m_object;
        bool m_initialized;
    };

} // namespace detail
} // namespace pipes

#endif /* PIPES_OPTIONAL_HPP */
