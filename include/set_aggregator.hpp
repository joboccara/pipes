#ifndef PIPES_SET_AGGREGATOR_HPP
#define PIPES_SET_AGGREGATOR_HPP

#include <functional>
#include <iterator>
#include "output_iterator.hpp"
#include "helpers/assignable.hpp"
#include "helpers/warnings.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename Set, typename Function>
class set_aggregate_iterator : public OutputIteratorBase<set_aggregate_iterator<Set, Function>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        auto position = set_.get().find(value);
        if (position != set_.get().end())
        {
            auto containedValue = *position;
            position = set_.get().erase(position);
            set_.get().insert(position, aggregator_(value, containedValue));
        }
        else
        {
            set_.get().insert(position, value);
        }
    }
    
    using container_type = Set;
    set_aggregate_iterator(Set& set, Function aggregator) : set_(set), aggregator_(aggregator) {}
    
private:
    std::reference_wrapper<Set> set_;
    detail::assignable<Function> aggregator_;

public: // but technical
    using OutputIteratorBase<set_aggregate_iterator<Set, Function>>::operator=;
    set_aggregate_iterator& operator=(set_aggregate_iterator const& other)
    {
        set_ = other.set_;
        aggregator_ = other.aggregator_;
        return *this;
    }
    set_aggregate_iterator& operator=(set_aggregate_iterator& other) { *this = const_cast<set_aggregate_iterator const&>(other); return *this; }
};

template<typename Set, typename Function>
set_aggregate_iterator<Set, Function> set_aggregator(Set& set, Function aggregator)
{
    return set_aggregate_iterator<Set, Function>(set, aggregator);
}
    
} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif // PIPES_SET_AGGREGATOR_HPP
