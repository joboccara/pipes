#ifndef TO_OUT_STREAM_HPP
#define TO_OUT_STREAM_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include <functional>

namespace pipes
{

template<typename OutStream>
class to_out_stream_pipeline : public pipeline_base<to_out_stream_pipeline<OutStream>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        outStream_.get() << FWD(value);
    }
    
    explicit to_out_stream_pipeline(OutStream& outStream) : outStream_(outStream) {}

private:
    std::reference_wrapper<OutStream> outStream_;
};

template<typename OutStream>
to_out_stream_pipeline<OutStream> to_out_stream(OutStream& outStream)
{
    return to_out_stream_pipeline<OutStream>(outStream);
}

} // namespace pipes

#endif /* TO_OUT_STREAM_HPP */
