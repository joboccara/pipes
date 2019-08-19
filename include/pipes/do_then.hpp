#ifndef PIPES_DO_THEN_HPP
#define PIPES_DO_THEN_HPP

#include <functional>
#include <vector>

#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    
template<typename Function, typename... Functions>
class Do_
{
public:
    explicit Do_(Function&& function, Do_<Functions...>&& previousFunctions) : function_(FWD(function)), previousFunctions_(std::move(previousFunctions)){}
    
    template<typename NewFunction>
    Do_<NewFunction, Function, Functions...> then_(NewFunction&& newFunction)
    {
        return Do_<NewFunction, Function, Functions...>{FWD(newFunction), std::move(*this)};
    }
    
    template<typename Arg>
    void operator()(Arg&& arg)
    {
        previousFunctions_(FWD(arg));
        function_(FWD(arg));
    }
private:
    Function function_;
    Do_<Functions...> previousFunctions_;
};

template<typename Function>
class Do_<Function>
{
public:
    explicit Do_(Function&& function) : function_(FWD(function)){}

    template<typename NewFunction>
    Do_<NewFunction, Function> then_(NewFunction&& newFunction)
    {
        return Do_<NewFunction, Function>{FWD(newFunction), std::move(*this)};
    }
    
    template<typename Arg>
    void operator()(Arg&& arg)
    {
        function_(FWD(arg));
    }
private:
    Function function_;
};
    
template<typename Function>
Do_<Function> do_(Function&& function)
{
    return Do_<Function>(FWD(function));
}
} // namespace pipes

#endif /* PIPES_DO_THEN_HPP */
