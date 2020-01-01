#ifndef PIPES_ASSIGNABLE_HPP
#define PIPES_ASSIGNABLE_HPP

#include "pipes/helpers/optional.hpp"
#include <functional>

namespace pipes
{
namespace detail
{
    
template<typename T>
class assignable
{
public:
    assignable& operator=(assignable const& other)
    {
        value_.emplace(*other.value_);
        return *this;
    }
    assignable& operator=(assignable&& other)
    {
        value_ = std::move(other.value_);
        return *this;
    }
    assignable(assignable const& other) : value_(other.value_){}
    
    assignable(assignable&& other) : value_(std::move(other.value_)) {}
    
    assignable(T const& value) : value_(value) {}
    assignable(T&& value) : value_(std::move(value)) {}
    
    T const& get() const { return *value_; }
    T& get() { return *value_; }
    
    template<typename... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return (*value_)(std::forward<Args>(args)...);
    }
private:
    optional<T> value_;
};

template<typename T>
class assignable<T&>
{
public:
    assignable(T& value) : value_(value) {}
    
    T& get() const { return value_; }
    T& get() { return value_; }
    
    template<typename... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return value_(std::forward<Args>(args)...);
    }
private:
    std::reference_wrapper<T> value_;
};

} // namespace detail
} // namespace pipes

#endif /* PIPES_ASSIGNABLE_HPP */
