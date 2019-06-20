#ifndef partition_pipe_maker_hpp
#define partition_pipe_maker_hpp

#include "helpers/assignable.hpp"
#include "helpers/meta.hpp"
#include "output_iterator.hpp"

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
    partition_pipe& operator=(partition_pipe const&) = default;
    partition_pipe& operator=(partition_pipe& other) { *this = const_cast<partition_pipe const&>(other); return *this; }
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

template<typename Predicate>
partition_pipe_maker<Predicate> partition(Predicate predicate)
{
    return partition_pipe_maker<Predicate>(predicate);
}

} // namespace pipes
    
#endif /* partition_pipe_maker_hpp */
