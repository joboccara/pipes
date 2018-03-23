#ifndef fluent_smart_output_iterators_named_type_h
#define fluent_smart_output_iterators_named_type_h

namespace fluent
{
namespace detail
{
    
    template <typename T, typename Parameter>
    class NamedType
    {
    public:
        explicit NamedType(T const& value) : value_(value) {}
        explicit NamedType(T&& value) : value_(std::move(value)) {}
        T& get() { return value_; }
        T const& get() const {return value_; }
    private:
        T value_;
    };
    
    template<template<typename T> class StrongType, typename T>
    StrongType<T> make_named(T const& value)
    {
        return StrongType<T>(value);
    }
    
}  // namespace detail
}  // namespace fluent

#endif /* fluent_smart_output_iterators_named_type_h */
