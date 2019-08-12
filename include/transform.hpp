#ifndef PIPES_TRANSFORM_HPP
#define PIPES_TRANSFORM_HPP

#include "helpers/assignable.hpp"
#include "helpers/FWD.hpp"
#include "helpers/meta.hpp"
#include "helpers/warnings.hpp"

#include "output_iterator.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename TransformFunctionTuple, typename... OutputPipes>
class transform_pipe : public OutputIteratorBase<transform_pipe<TransformFunctionTuple, OutputPipes...>>
{
public:
    template<typename T>
    void onReceive(T&& input)
    {
        detail::for_each2([&input](auto&& function, auto&& outputPipe)
        {
            send(outputPipe, function(input));
        }, transformFunctionTuple_, outputPipes_);
    }

    explicit transform_pipe(TransformFunctionTuple transformFunctionTuple, OutputPipes... outputPipes) : outputPipes_(outputPipes...), transformFunctionTuple_(transformFunctionTuple) {}
    
private:
    std::tuple<OutputPipes...> outputPipes_;
    TransformFunctionTuple transformFunctionTuple_;

public: // but technical
    using OutputIteratorBase<transform_pipe<TransformFunctionTuple, OutputPipes...>>::operator=;
    transform_pipe& operator=(transform_pipe const& other)
    {
        outputPipes_ = other.outputPipes_;
        transformFunctionTuple_ = other.transformFunctionTuple_;
        return *this;
    }
    transform_pipe& operator=(transform_pipe& other) { *this = const_cast<transform_pipe const&>(other); return *this; }
};

template<typename... TransformFunctions>
class transform_pipe_maker
{
public:
    explicit transform_pipe_maker(TransformFunctions... transformFunctions) : transformFunctionsTuple_(transformFunctions...) {}
    template<typename... OutputPipes>
    transform_pipe<std::tuple<detail::assignable<TransformFunctions>...>, OutputPipes...> operator()(OutputPipes... outputPipes) const
    {
        return transform_pipe<std::tuple<detail::assignable<TransformFunctions>...>, OutputPipes...>(transformFunctionsTuple_, outputPipes...);
    }
    
private:
    std::tuple<detail::assignable<TransformFunctions>...> transformFunctionsTuple_;
};
    
template<typename TransformFunction, typename OutputPipe>
    transform_pipe<std::tuple<detail::assignable<TransformFunction>>, OutputPipe> operator>>=(transform_pipe_maker<TransformFunction> const& outputTransformer, OutputPipe outputPipe)
{
    return outputTransformer(outputPipe);
}

template<typename... TransformFunctions>
transform_pipe_maker<TransformFunctions...> transform(TransformFunctions... transformFunctions)
{
    return transform_pipe_maker<TransformFunctions...>(transformFunctions...);
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_TRANSFORM_HPP */
