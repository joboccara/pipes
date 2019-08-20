#ifndef PIPES_TRANSFORM_HPP
#define PIPES_TRANSFORM_HPP

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"

#include "pipes/output_iterator.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename Function, typename NextPipe>
class transform_pipe : public OutputIteratorBase<transform_pipe<Function, NextPipe>>
{
public:
    template<typename T>
    void onReceive(T&& input)
    {
        send(nextPipe_, function_(input));
    }

    explicit transform_pipe(Function function, NextPipe nextPipe) : function_(function), nextPipe_(nextPipe) {}
    
private:
    detail::assignable<Function> function_;
    NextPipe nextPipe_;

public: // but technical
    using base = OutputIteratorBase<transform_pipe<Function, NextPipe>>;
    using base::operator=;
    transform_pipe& operator=(transform_pipe const& other)
    {
        function_ = other.function_;
        nextPipe_ = other.nextPipe_;
        return *this;
    }
    transform_pipe& operator=(transform_pipe& other) { *this = const_cast<transform_pipe const&>(other); return *this; }
};

template<typename Function>
class transform_pipe_maker
{
public:
    explicit transform_pipe_maker(Function function) : function_(function) {}
    template<typename NextPipe>
    transform_pipe<Function, NextPipe> operator()(NextPipe nextPipe) const
    {
        return transform_pipe<Function, NextPipe>(function_, nextPipe);
    }
    
private:
    Function function_;
};
    
template<typename Function, typename NextPipe>
    transform_pipe<Function, NextPipe> operator>>=(transform_pipe_maker<Function> const& outputTransformer, NextPipe nextPipe)
{
    return outputTransformer(nextPipe);
}

template<typename Function>
transform_pipe_maker<Function> transform(Function function)
{
    return transform_pipe_maker<Function>(function);
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_TRANSFORM_HPP */
