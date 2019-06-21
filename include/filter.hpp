#ifndef PIPES_FILTER_HPP
#define PIPES_FILTER_HPP

#include "helpers/assignable.hpp"
#include "helpers/meta.hpp"
#include "helpers/warnings.hpp"
#include "output_iterator.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

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
    detail::assignable<Predicate> predicate_;

public: // but technical
    using OutputIteratorBase<filter_pipe<OutputPipe, Predicate>>::operator=;
    filter_pipe& operator=(filter_pipe const& other)
    {
        outputPipe_ = other.outputPipe_;
        predicate_ = other.predicate_;
        return *this;
    }
    filter_pipe& operator=(filter_pipe& other) { *this = const_cast<filter_pipe const&>(other); return *this; }
};

template<typename Predicate>
class filter_pipe_maker
{
public:
    explicit filter_pipe_maker(Predicate predicate) : predicate_(predicate) {}
    template<typename OutputPipe>
    filter_pipe<OutputPipe, Predicate> operator()(OutputPipe outputPipe) const
    {
        return filter_pipe<OutputPipe, Predicate>(outputPipe, predicate_);
    }
    
private:
    Predicate predicate_;
};

template<typename FilterFunction, typename OutputPipe>
filter_pipe<OutputPipe, FilterFunction> operator>>=(filter_pipe_maker<FilterFunction> const& outputFilter, OutputPipe outputPipe)
{
    return outputFilter(outputPipe);
}

template<typename Predicate>
filter_pipe_maker<Predicate> filter(Predicate predicate)
{
    return filter_pipe_maker<Predicate>(predicate);
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_FILTER_HPP */
