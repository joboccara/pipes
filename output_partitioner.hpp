#ifndef output_partitioner_hpp
#define output_partitioner_hpp

#include "helpers/meta.hpp"
#include <iterator>

namespace fluent
{

template<typename IteratorTrue, typename IteratorFalse, typename Predicate>
class output_partition_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_partition_iterator(IteratorTrue iteratorTrue, IteratorFalse iteratorFalse, Predicate predicate) : iteratorTrue_(iteratorTrue), iteratorFalse_(iteratorFalse), predicate_(predicate) {}
    output_partition_iterator& operator++(){ return *this; }
    output_partition_iterator& operator++(int){ ++*this; return *this; }
    output_partition_iterator& operator*(){ return *this; }
    template<typename T>
    output_partition_iterator& operator=(T const& value)
    {
        if (predicate_(value))
        {
            *iteratorTrue_ = value;
            ++iteratorTrue_;
        }
        else
        {
            *iteratorFalse_ = value;
            ++iteratorFalse_;
        }
        return *this;
    }
private:
    IteratorTrue iteratorTrue_;
    IteratorFalse iteratorFalse_;
    Predicate predicate_;
};

template<typename Predicate>
class output_partitioner
{
public:
    explicit output_partitioner(Predicate predicate) : predicate_(predicate) {}
    template<typename IteratorTrue, typename IteratorFalse>
    output_partition_iterator<IteratorTrue, IteratorFalse, Predicate> operator()(IteratorTrue iteratorTrue, IteratorFalse iteratorFalse) const
    {
        return output_partition_iterator<IteratorTrue, IteratorFalse, Predicate>(iteratorTrue, iteratorFalse, predicate_);
    }
    
private:
    Predicate predicate_;
};

template<typename Predicate>
output_partitioner<Predicate> make_output_partitioner(Predicate predicate)
{
    return output_partitioner<Predicate>(predicate);
}

} // namespace fluent
    
#endif /* output_partitioner_hpp */
