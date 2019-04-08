#ifndef custom_inserter_hpp
#define custom_inserter_hpp

namespace pipes
{
    
template<typename InsertFunction>
class custom_insert_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit custom_insert_iterator(InsertFunction insertFunction) : insertFunction_(insertFunction) {}
    
    custom_insert_iterator& operator++(){ return *this; }
    custom_insert_iterator& operator++(int){ return *this; }
    custom_insert_iterator& operator*(){ return *this; }
    
    template<typename T>
    custom_insert_iterator& operator=(T const& value)
    {
        insertFunction_(value);
        return *this;
    }
    
private:
    InsertFunction insertFunction_;
};
    
template <typename InsertFunction>
custom_insert_iterator<InsertFunction> custom_inserter(InsertFunction insertFunction)
{
    return custom_insert_iterator<InsertFunction>(insertFunction);
}
    
} // namespace pipes

#endif /* custom_inserter_hpp */
