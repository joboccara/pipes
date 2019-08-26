#ifndef TO_OUT_STREAM_HPP
#define TO_OUT_STREAM_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/warnings.hpp"
#include <functional>

namespace pipes
{

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

template<typename OutStream>
class to_out_stream_pipe : public pipeline_base<to_out_stream_pipe<OutStream>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        outStream_.get() << FWD(value);
    }
    
    to_out_stream_pipe(OutStream& outStream) : outStream_(outStream) {}

private:
    std::reference_wrapper<OutStream> outStream_;
    
public: // but technical
    using base = pipeline_base<to_out_stream_pipe<OutStream>>;
    using base::operator=;
    to_out_stream_pipe& operator=(to_out_stream_pipe const& other)
    {
        outStream_ = other.outStream_;
        return *this;
    }
    to_out_stream_pipe& operator=(to_out_stream_pipe& other) { *this = const_cast<to_out_stream_pipe const&>(other); return *this; }
};
    
PIPES_DISABLE_WARNING_POP

template<typename OutStream>
to_out_stream_pipe<OutStream> to_out_stream(OutStream& outStream)
{
    return to_out_stream_pipe<OutStream>(outStream);
}

} // namespace pipes

#endif /* TO_OUT_STREAM_HPP */
