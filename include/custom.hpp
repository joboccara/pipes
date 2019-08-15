#ifndef PIPES_CUSTOM_INSERTER_HPP
#define PIPES_CUSTOM_INSERTER_HPP

#include "output_iterator.hpp"
#include "helpers/assignable.hpp"
#include "helpers/warnings.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
    
template<typename InsertFunction>
class custom_iterator : public OutputIteratorBase<custom_iterator<InsertFunction>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        insertFunction_(value);
    }
    
    explicit custom_iterator(InsertFunction insertFunction) : insertFunction_(insertFunction) {}
    
private:
    detail::assignable<InsertFunction> insertFunction_;

public: // but technical
    using OutputIteratorBase<custom_iterator<InsertFunction>>::operator=;
    custom_iterator& operator=(custom_iterator const& other)
    {
        insertFunction_ = other.insertFunction_;
        return *this;
    } // MSVC refuses "= default" for this overload of operator=, because of the presence of the other overload. Compile error message: multiple versions of a defaulted special member functions are not allowed

    custom_iterator& operator=(custom_iterator& other) { *this = const_cast<custom_iterator const&>(other); return *this; } // since the base class has a template operator=, we need an operator= in the derived class that takes a non const parameter, otherwise X x; x2 = x; goes to the template one
};
    
template <typename InsertFunction>
custom_iterator<InsertFunction> custom(InsertFunction insertFunction)
{
    return custom_iterator<InsertFunction>(insertFunction);
}
    
} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_CUSTOM_INSERTER_HPP */
