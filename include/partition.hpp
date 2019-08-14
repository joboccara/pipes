#ifndef PIPES_PARTITION_HPP
#define PIPES_PARTITION_HPP

#include "helpers/assignable.hpp"
#include "helpers/FWD.hpp"
#include "helpers/meta.hpp"
#include "helpers/warnings.hpp"
#include "output_iterator.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename OutputPipeTrue, typename OutputPipeFalse, typename Predicate>
class partition_pipe : public OutputIteratorBase<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        if (predicate_(value))
        {
            send(outputPipeTrue_, value);
        }
        else
        {
            send(outputPipeFalse_, value);
        }
    }

    explicit partition_pipe(OutputPipeTrue iteratorTrue, OutputPipeFalse iteratorFalse, Predicate predicate) : outputPipeTrue_(iteratorTrue), outputPipeFalse_(iteratorFalse), predicate_(predicate) {}
    
private:
    OutputPipeTrue outputPipeTrue_;
    OutputPipeFalse outputPipeFalse_;
    detail::assignable<Predicate> predicate_;

public: // but technical
    using OutputIteratorBase<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>::operator=;
    partition_pipe& operator=(partition_pipe const& other)
    {
        outputPipeTrue_ = other.outputPipeTrue_;
        outputPipeFalse_ = other.outputPipeFalse_;
        predicate_ = other.predicate_;
        return *this;
    }
    partition_pipe& operator=(partition_pipe& other) { *this = const_cast<partition_pipe const&>(other); return *this; }
};

template<typename Predicate, typename OutputPipeTrue, typename OutputPipeFalse>
partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate> partition(Predicate predicate, OutputPipeTrue&& outputPipeTrue, OutputPipeFalse&& outputPipeFalse)
{
    return partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>(FWD(outputPipeTrue), FWD(outputPipeFalse), predicate);
}

} // namespace pipes
    
PIPES_DISABLE_WARNING_POP

#endif /* PIPES_PARTITION_HPP */
