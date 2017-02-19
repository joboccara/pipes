#ifndef SET_AGGREGATOR_HPP
#define SET_AGGREGATOR_HPP

#include <iterator>

template<typename Set, typename Function>
class set_aggregate_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
    using container_type = Set;
    set_aggregate_iterator(Set& set, Function aggregator) : set_(set), aggregator_(aggregator) {}
    set_aggregate_iterator operator++(){ return *this; }
    set_aggregate_iterator operator*(){ return *this; }
    template<typename Value>
    set_aggregate_iterator& operator=(Value const& value)
    {
        auto position = set_.find(value);
        if (position != set_.end())
        {
            auto containedValue = *position;
            set_.erase(position);
            set_.insert(position, aggregator_(value, containedValue));
        }
        else
        {
            set_.insert(position, value);
        }
        return *this;
    }
    
private:
    Set& set_;
    Function aggregator_;

};

template<typename Set, typename Function>
set_aggregate_iterator<Set, Function> set_aggregator(Set& set, Function aggregator)
{
    return set_aggregate_iterator<Set, Function>(set, aggregator);
}

#endif
