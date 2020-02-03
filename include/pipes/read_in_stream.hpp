#ifndef READ_IN_STREAM_HPP
#define READ_IN_STREAM_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

#include <iterator>

namespace pipes
{

    template<typename Value, typename Pipeline>
    struct read_in_stream_pipeline
    {
        Pipeline pipeline_;
        explicit read_in_stream_pipeline(Pipeline& pipeline) : pipeline_(pipeline){}
    };

    template<typename Value>
    struct read_in_stream {};

    template<typename Value, typename Pipeline>
    auto operator>>= (read_in_stream<Value>, Pipeline&& pipeline)
    {
        return read_in_stream_pipeline<Value, std::decay_t<Pipeline>>{pipeline};
    }

    template<typename InStream, typename Value, typename Pipeline>
    void operator>>= (InStream&& inStream, read_in_stream_pipeline<Value, Pipeline> readInStreamPipe)
    {
        for (auto inValue = std::istream_iterator<Value>{inStream}; inValue != std::istream_iterator<Value>{}; ++inValue)
        {
            pipes::send(*inValue, readInStreamPipe.pipeline_);
        }
    }

} // namespace pipes
#endif /* READ_IN_STREAM_HPP */
