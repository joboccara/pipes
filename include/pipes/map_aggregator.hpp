#ifndef PIPES_MAP_AGGREGATOR_HPP
#define PIPES_MAP_AGGREGATOR_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/warnings.hpp"

#include <functional>
#include <iterator>

namespace pipes
{

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

template<typename Map, typename Function>
class map_aggregate_iterator : public pipeline_base<map_aggregate_iterator<Map, Function>>
{
public:
    template<typename T>
    void onReceive(T&& keyValue)
    {
        auto position = map_.get().find(keyValue.first);
        if (position != map_.get().end())
        {
            position->second = aggregator_(position->second, keyValue.second);
        }
        else
        {
            map_.get().insert(keyValue);
        }
    }
    
    using container_type = Map;
    map_aggregate_iterator(Map& map, Function aggregator) : map_(map), aggregator_(aggregator) {}
    
private:
    std::reference_wrapper<Map> map_;
    detail::assignable<Function> aggregator_;
    
public: // but technical
    using base = pipeline_base<map_aggregate_iterator<Map, Function>>;
    using base::operator=;
    map_aggregate_iterator& operator=(map_aggregate_iterator const& other)
    {
        map_ = other.map_;
        aggregator_ = other.aggregator_;
        return *this;
    }
    map_aggregate_iterator& operator=(map_aggregate_iterator& other) { *this = const_cast<map_aggregate_iterator const&>(other); return *this; }
};

PIPES_DISABLE_WARNING_POP

template<typename Map, typename Function>
map_aggregate_iterator<Map, Function> map_aggregator(Map& map, Function aggregator)
{
    return map_aggregate_iterator<Map, Function>(map, aggregator);
}
    
} // namespace pipes

#endif // PIPES_MAP_AGGREGATOR_HPP
