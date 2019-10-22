#ifndef PIPES_CUSTOM_INSERTER_HPP
#define PIPES_CUSTOM_INSERTER_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"

namespace pipes
{
    
    template<typename Function>
    class for_each_pipeline : public pipeline_base<for_each_pipeline<Function>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            function_(FWD(value));
        }
        
        explicit for_each_pipeline(Function function) : function_(function) {}
        
    private:
        detail::assignable<Function> function_;
    };
    
    template <typename InsertFunction>
    for_each_pipeline<InsertFunction> for_each(InsertFunction insertFunction)
    {
        return for_each_pipeline<InsertFunction>(insertFunction);
    }
    
} // namespace pipes

#endif /* PIPES_CUSTOM_INSERTER_HPP */
