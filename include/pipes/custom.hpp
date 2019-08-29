#ifndef PIPES_CUSTOM_INSERTER_HPP
#define PIPES_CUSTOM_INSERTER_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/assignable.hpp"

namespace pipes
{
    
template<typename InsertFunction>
class custom_iterator : public pipeline_base<custom_iterator<InsertFunction>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        insertFunction_(FWD(value));
    }
    
    explicit custom_iterator(InsertFunction insertFunction) : insertFunction_(insertFunction) {}
    
private:
    detail::assignable<InsertFunction> insertFunction_;
};
    
template <typename InsertFunction>
custom_iterator<InsertFunction> custom(InsertFunction insertFunction)
{
    return custom_iterator<InsertFunction>(insertFunction);
}
    
} // namespace pipes

#endif /* PIPES_CUSTOM_INSERTER_HPP */
