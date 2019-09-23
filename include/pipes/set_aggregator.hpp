#ifndef PIPES_SET_AGGREGATOR_HPP
#define PIPES_SET_AGGREGATOR_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"

#include <functional>
#include <iterator>

namespace pipes
{

template<typename Set, typename Function>
class set_aggregate_iterator : public pipeline_base<set_aggregate_iterator<Set, Function>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        auto position = set_.get().find(value);
        if (position != set_.get().end())
        {
            auto containedValue = *position;
            position = set_.get().erase(position);
            set_.get().insert(position, aggregator_(FWD(value), containedValue));
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
};

template<typename Set, typename Function>
set_aggregate_iterator<Set, Function> set_aggregator(Set& set, Function aggregator)
{
    return set_aggregate_iterator<Set, Function>(set, aggregator);
}
    
} // namespace pipes

#endif // PIPES_SET_AGGREGATOR_HPP
