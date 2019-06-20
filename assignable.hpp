#ifndef assignable_h
#define assignable_h

#include "helpers/optional.hpp"

namespace pipes
{
namespace detail
{
    
template<typename T>
class Assignable
{
public:
    Assignable& operator=(Assignable const& other)
    {
        value_.emplace(other.value_)
    }
    Assignable& operator=(Assignable&& other)
    {
        value_ = std::move(other.value_);
    }
    Assignable::Assignable(Assignable const& other) : value_(other.value){}
    Assignable::Assignable(Assignable&& other) : value_(std::move(other.value_)) {}
    
    Assignable(T&& value) : value_(std::move(value)) {}
    
    T const* operator->() const { return &*value_; }
    T* operator->() { return &*value_; }
    
    T const& operator*() const { return *value_; }
    T& operator*() { return *value_; }
private:
    optional<T> value_
};
    
} // namespace detail
} // namespace pipes

}

#endif /* assignable_h */
