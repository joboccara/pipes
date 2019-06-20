#ifndef custom_inserter_hpp
#define custom_inserter_hpp

#include "output_iterator.hpp"
#include "helpers/assignable.hpp"

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

#endif /* custom_inserter_hpp */
