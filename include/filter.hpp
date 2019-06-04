#ifndef output_pipe_maker_h
#define output_pipe_maker_h

#include "helpers/meta.hpp"
#include "output_iterator.hpp"

namespace pipes
{
template<typename OutputPipe, typename Predicate>
    class filter_pipe : public OutputIteratorBase<filter_pipe<OutputPipe, Predicate>>
{
public:    
    template<typename T>
    void onReceive(T const& value)
    {
        if (predicate_(value))
        {
            send(outputPipe_, value);
        }
    }

    explicit filter_pipe(OutputPipe outputPipe, Predicate predicate) : outputPipe_(outputPipe), predicate_(predicate) {}
    
private:
    OutputPipe outputPipe_;
    Predicate predicate_;

public: // but technical
    using OutputIteratorBase<filter_pipe<OutputPipe, Predicate>>::operator=;
};

template<typename Predicate>
class output_pipe_maker
{
public:
    explicit output_pipe_maker(Predicate predicate) : predicate_(predicate) {}
    template<typename OutputPipe>
    filter_pipe<OutputPipe, Predicate> operator()(OutputPipe outputPipe) const
    {
        return filter_pipe<OutputPipe, Predicate>(outputPipe, predicate_);
    }
    
private:
    Predicate predicate_;
};

template<typename FilterFunction, typename OutputPipe>
filter_pipe<OutputPipe, FilterFunction> operator>>=(output_pipe_maker<FilterFunction> const& outputFilter, OutputPipe outputPipe)
{
    return outputFilter(outputPipe);
}

template<typename Predicate>
output_pipe_maker<Predicate> filter(Predicate predicate)
{
    return output_pipe_maker<Predicate>(predicate);
}

} // namespace pipes

#endif /* output_transformer_h */
