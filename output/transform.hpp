#ifndef output_transform_h
#define output_transform_h

#include "../helpers/crtp.hpp"
#include "../helpers/FWD.hpp"
#include "../helpers/meta.hpp"

namespace pipes
{

template<typename Derived>
struct OutputIteratorBase : detail::crtp<Derived, OutputIteratorBase>
{
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    Derived& operator++() { return this->derived(); }
    Derived& operator++(int){ ++this->derived(); return this->derived(); }
    Derived& operator*() { return this->derived(); }
    
    template<typename T>
    Derived& operator=(T&& input)
    {
        this->derived().onReceive(FWD(input));
        return this->derived();
    }
};
    
template<typename OutputIterator, typename T>
void send(OutputIterator& outputIterator, T&& value)
{
    *outputIterator = FWD(value);
    ++outputIterator;
}
    
template<typename TransformFunctionTuple, typename... OutputPipes>
class transform_pipe : public OutputIteratorBase<transform_pipe<TransformFunctionTuple, OutputPipes...>>
{
public:
    explicit transform_pipe(TransformFunctionTuple transformFunctionTuple, OutputPipes... outputPipes) : outputPipes_(outputPipes...), transformFunctionTuple_(transformFunctionTuple) {}

private:
    template<typename T>
    void onReceive(T&& input)
    {
        detail::apply2([&input](auto&& function, auto&& outputPipe)
        {
            send(outputPipe, function(input));
        }, transformFunctionTuple_, outputPipes_);
    }

private:
    std::tuple<OutputPipes...> outputPipes_;
    TransformFunctionTuple transformFunctionTuple_;

public: // but technical
    using OutputIteratorBase<transform_pipe<TransformFunctionTuple, OutputPipes...>>::operator=;
    friend OutputIteratorBase<transform_pipe<TransformFunctionTuple, OutputPipes...>>;
};

template<typename... TransformFunctions>
class transform_pipe_maker
{
public:
    explicit transform_pipe_maker(TransformFunctions... transformFunctions) : transformFunctionsTuple_(transformFunctions...) {}
    template<typename... OutputPipes>
    transform_pipe<std::tuple<TransformFunctions...>, OutputPipes...> operator()(OutputPipes... outputPipes) const
    {
        return transform_pipe<std::tuple<TransformFunctions...>, OutputPipes...>(transformFunctionsTuple_, outputPipes...);
    }
    
private:
    std::tuple<TransformFunctions...> transformFunctionsTuple_;
};
    
template<typename TransformFunction, typename OutputPipe>
    transform_pipe<std::tuple<TransformFunction>, OutputPipe> operator>>=(transform_pipe_maker<TransformFunction> const& outputTransformer, OutputPipe outputPipe)
{
    return outputTransformer(outputPipe);
}

namespace output
{

template<typename... TransformFunctions>
transform_pipe_maker<TransformFunctions...> transform(TransformFunctions... transformFunctions)
{
    return transform_pipe_maker<TransformFunctions...>(transformFunctions...);
}

} // namespace output

} // namespace pipes

#endif /* output_transformer_h */
