#ifndef SET_AGGREGATOR_HPP
#define SET_AGGREGATOR_HPP

#include <iterator>
#include "output_iterator.hpp"

namespace pipes
{

template<typename Set, typename Function>
class set_aggregate_iterator : public OutputIteratorBase<set_aggregate_iterator<Set, Function>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        auto position = set_.find(value);
        if (position != set_.end())
        {
            auto containedValue = *position;
            position = set_.erase(position);
            set_.insert(position, aggregator_(value, containedValue));
        }
        else
        {
            set_.insert(position, value);
        }
    }
    
    using container_type = Set;
    set_aggregate_iterator(Set& set, Function aggregator) : set_(set), aggregator_(aggregator) {}
    
private:
    Set& set_;
    Function aggregator_;

public: // but technical
    using OutputIteratorBase<set_aggregate_iterator<Set, Function>>::operator=;
};

template<typename Set, typename Function>
set_aggregate_iterator<Set, Function> set_aggregator(Set& set, Function aggregator)
{
    return set_aggregate_iterator<Set, Function>(set, aggregator);
}
    
} // namespace pipes

#endif
