#ifndef PIPES_MAP_AGGREGATOR_HPP
#define PIPES_MAP_AGGREGATOR_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"

#include <functional>
#include <iterator>

namespace pipes
{

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
};

template<typename Map, typename Function>
map_aggregate_iterator<Map, Function> map_aggregator(Map& map, Function aggregator)
{
    return map_aggregate_iterator<Map, Function>(map, aggregator);
}
    
} // namespace pipes

#endif // PIPES_MAP_AGGREGATOR_HPP
