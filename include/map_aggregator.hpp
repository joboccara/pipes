#ifndef PIPES_MAP_AGGREGATOR_HPP
#define PIPES_MAP_AGGREGATOR_HPP

#include <functional>
#include <iterator>
#include "output_iterator.hpp"
#include "helpers/assignable.hpp"
#include "helpers/warnings.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename Map, typename Function>
class map_aggregate_iterator : public OutputIteratorBase<map_aggregate_iterator<Map, Function>>
{
public:
    template<typename T>
    void onReceive(T const& keyValue)
    {
        auto position = map_.get().find(keyValue.first);
        if (position != map_.get().end())
        {
            position->second = (*aggregator_)(position->second, keyValue.second);
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
    using OutputIteratorBase<map_aggregate_iterator<Map, Function>>::operator=;
    map_aggregate_iterator& operator=(map_aggregate_iterator const& other)
    {
        map_ = other.map_;
        aggregator_ = other.aggregator_;
        return *this;
    }
    map_aggregate_iterator& operator=(map_aggregate_iterator& other) { *this = const_cast<map_aggregate_iterator const&>(other); return *this; }
};

template<typename Map, typename Function>
map_aggregate_iterator<Map, Function> map_aggregator(Map& map, Function aggregator)
{
    return map_aggregate_iterator<Map, Function>(map, aggregator);
}
    
} // namespace pipes


PIPES_DISABLE_WARNING_POP

#endif // PIPES_MAP_AGGREGATOR_HPP
