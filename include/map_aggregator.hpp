#ifndef MAP_AGGREGATOR_HPP
#define MAP_AGGREGATOR_HPP

#include <iterator>
#include "output_iterator.hpp"

namespace pipes
{

template<typename Map, typename Function>
class map_aggregate_iterator : public OutputIteratorBase<map_aggregate_iterator<Map, Function>>
{
public:
    template<typename T>
    void onReceive(T const& keyValue)
    {
        auto position = map_.find(keyValue.first);
        if (position != map_.end())
        {
            position->second = aggregator_(position->second, keyValue.second);
        }
        else
        {
            map_.insert(keyValue);
        }
    }
    
    using container_type = Map;
    map_aggregate_iterator(Map& map, Function aggregator) : map_(map), aggregator_(aggregator) {}
    
private:
    Map& map_;
    Function aggregator_;
    
public: // but technical
    using OutputIteratorBase<map_aggregate_iterator<Map, Function>>::operator=;
};

template<typename Map, typename Function>
map_aggregate_iterator<Map, Function> map_aggregator(Map& map, Function aggregator)
{
    return map_aggregate_iterator<Map, Function>(map, aggregator);
}
    
} // namespace pipes


#endif
