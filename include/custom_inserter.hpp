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
class custom_insert_iterator : public OutputIteratorBase<custom_insert_iterator<InsertFunction>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        insertFunction_(value);
    }
    
    explicit custom_insert_iterator(InsertFunction insertFunction) : insertFunction_(insertFunction) {}
    
private:
    detail::assignable<InsertFunction> insertFunction_;

public: // but technical
    using OutputIteratorBase<custom_insert_iterator<InsertFunction>>::operator=;
    custom_insert_iterator& operator=(custom_insert_iterator const& other) = default;
    custom_insert_iterator& operator=(custom_insert_iterator& other) { *this = const_cast<custom_insert_iterator const&>(other); return *this; } // since the base class has a template operator=, we need an operator= in the derived class that takes a non const parameter, otherwise X x; x2 = x; goes to the template one
};
    
template <typename InsertFunction>
custom_insert_iterator<InsertFunction> custom_inserter(InsertFunction insertFunction)
{
    return custom_insert_iterator<InsertFunction>(insertFunction);
}
    
} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_CUSTOM_INSERTER_HPP */
