#ifndef PIPES_CUSTOM_INSERTER_HPP
#define PIPES_CUSTOM_INSERTER_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"

namespace pipes
{
    
    template<typename Function>
    class custom_pipeline : public pipeline_base<custom_pipeline<Function>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            function_(FWD(value));
        }
        
        explicit custom_pipeline(Function function) : function_(function) {}
        
    private:
        detail::assignable<Function> function_;
    };
    
    template <typename InsertFunction>
    custom_pipeline<InsertFunction> custom(InsertFunction insertFunction)
    {
        return custom_pipeline<InsertFunction>(insertFunction);
    }
    
} // namespace pipes

#endif /* PIPES_CUSTOM_INSERTER_HPP */
