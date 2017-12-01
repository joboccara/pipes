//
//  optional.hpp
//  Defaulted
//
//  Created by Jonathan Boccara on 18/11/2017.
//  Copyright © 2017 Jonathan Boccara. All rights reserved.
//

#ifndef optional_h
#define optional_h

namespace fluent
{

struct nullopt_t {};
static const nullopt_t nullopt;

template<typename T>
class optional
{
public:
    optional(const T& object) : m_initialized(true) { new (m_object) T(object); };
    optional(nullopt_t) : m_initialized(false) {};
    optional() : m_initialized(false) {};
    optional(const optional& other) : m_initialized(other.m_initialized) { if (other.m_initialized) new (m_object) T(*other); }
    optional& operator=(const optional& other)
    {
        clear();
        new (m_object) T(*other);
        m_initialized = other.m_initialized;
        return *this;
    }
    optional& operator=(nullopt_t) { clear(); return *this; }
    ~optional() { clear(); }
    operator bool() const { return m_initialized; };
    T& operator*() { return *reinterpret_cast<T*>(m_object); };
    const T& operator*() const { return *reinterpret_cast<const T*>(m_object); };
    T* operator->() { return &**this; };
    const T* operator->() const { return &**this; };
private:
    void clear() { if (m_initialized) (&**this)->~T(); m_initialized = false; }
private:
    char m_object[sizeof(T)];
    bool m_initialized;
};

}

#endif /* optional_h */
