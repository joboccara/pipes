#ifndef PIPES_PARTITION_HPP
#define PIPES_PARTITION_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"
#include "pipes/pipeline_base.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename OutputPipeTrue, typename OutputPipeFalse, typename Predicate>
class partition_pipe : public pipeline_base<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        if (predicate_(FWD(value)))
        {
            send(outputPipeTrue_, FWD(value));
        }
        else
        {
            send(outputPipeFalse_, FWD(value));
        }
    }

    explicit partition_pipe(OutputPipeTrue iteratorTrue, OutputPipeFalse iteratorFalse, Predicate predicate) : outputPipeTrue_(iteratorTrue), outputPipeFalse_(iteratorFalse), predicate_(predicate) {}
    
private:
    OutputPipeTrue outputPipeTrue_;
    OutputPipeFalse outputPipeFalse_;
    detail::assignable<Predicate> predicate_;

public: // but technical
    using base = pipeline_base<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>;
    using base::operator=;
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
