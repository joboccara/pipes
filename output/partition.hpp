#ifndef partition_pipe_maker_hpp
#define partition_pipe_maker_hpp

#include "../helpers/meta.hpp"
#include "../output_iterator.hpp"

namespace pipes
{

template<typename OutputPipeTrue, typename OutputPipeFalse, typename Predicate>
class partition_pipe : public OutputIteratorBase<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>
{
public:
    explicit partition_pipe(OutputPipeTrue iteratorTrue, OutputPipeFalse iteratorFalse, Predicate predicate) : outputPipeTrue_(iteratorTrue), outputPipeFalse_(iteratorFalse), predicate_(predicate) {}

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

private:
    OutputPipeTrue outputPipeTrue_;
    OutputPipeFalse outputPipeFalse_;
    Predicate predicate_;

public: // but technical
    using OutputIteratorBase<partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>>::operator=;
};

template<typename Predicate>
class partition_pipe_maker
{
public:
    explicit partition_pipe_maker(Predicate predicate) : predicate_(predicate) {}
    template<typename OutputPipeTrue, typename OutputPipeFalse>
    partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate> operator()(OutputPipeTrue outputPipeTrue, OutputPipeFalse outputPipeFalse) const
    {
        return partition_pipe<OutputPipeTrue, OutputPipeFalse, Predicate>(outputPipeTrue, outputPipeFalse, predicate_);
    }
    
private:
    Predicate predicate_;
};

namespace output
{

template<typename Predicate>
partition_pipe_maker<Predicate> partition(Predicate predicate)
{
    return partition_pipe_maker<Predicate>(predicate);
}

} // namespace output

} // namespace pipes
    
#endif /* partition_pipe_maker_hpp */
