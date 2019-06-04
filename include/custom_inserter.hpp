#ifndef custom_inserter_hpp
#define custom_inserter_hpp

#include "output_iterator.hpp"

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
    InsertFunction insertFunction_;

public: // but technical
    using OutputIteratorBase<custom_insert_iterator<InsertFunction>>::operator=;
};
    
template <typename InsertFunction>
custom_insert_iterator<InsertFunction> custom_inserter(InsertFunction insertFunction)
{
    return custom_insert_iterator<InsertFunction>(insertFunction);
}
    
} // namespace pipes

#endif /* custom_inserter_hpp */
