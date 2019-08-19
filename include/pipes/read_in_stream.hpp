#ifndef READ_IN_STREAM_HPP
#define READ_IN_STREAM_HPP

#include "pipes/output_iterator.hpp"
#include "pipes/helpers/FWD.hpp"

#include <iterator>

namespace pipes
{

template<typename Value>
struct read_in_stream {};

template<typename Value, typename OutputPipe>
struct read_in_stream_pipe
{
    OutputPipe outputPipe_;
    explicit read_in_stream_pipe(OutputPipe&& outputPipe) : outputPipe_(std::move(outputPipe)){}
    explicit read_in_stream_pipe(OutputPipe& outputPipe) : outputPipe_(outputPipe){}
};

template<typename Value, typename OutputPipe>
read_in_stream_pipe<Value, OutputPipe> operator>>= (read_in_stream<Value>, OutputPipe&& outputPipe)
{
    return read_in_stream_pipe<Value, OutputPipe>(FWD(outputPipe));
}

template<typename InStream, typename Value, typename OutputPipe>
void operator>>= (InStream&& inStream, read_in_stream_pipe<Value, OutputPipe> readInStreamPipe)
{
    for (auto inValue = std::istream_iterator<Value>{inStream}; inValue != std::istream_iterator<Value>{}; ++inValue)
    {
        pipes::send(readInStreamPipe.outputPipe_, *inValue);
    }
}

} // namespace pipes
#endif /* READ_IN_STREAM_HPP */
