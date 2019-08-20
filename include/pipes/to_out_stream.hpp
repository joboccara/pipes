#ifndef TO_OUT_STREAM_HPP
#define TO_OUT_STREAM_HPP

#include "pipes/operator.hpp"

#include "pipes/output_iterator.hpp"
#include <functional>

namespace pipes
{
template<typename OutStream>
class to_out_stream_pipe : public OutputIteratorBase<to_out_stream_pipe<OutStream>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        outStream_.get() << value;
    }
    
    to_out_stream_pipe(OutStream& outStream) : outStream_(outStream) {}

private:
    std::reference_wrapper<OutStream> outStream_;
    
public: // but technical
    using base = OutputIteratorBase<to_out_stream_pipe<OutStream>>;
    using base::operator=;
    to_out_stream_pipe& operator=(to_out_stream_pipe const& other)
    {
        outStream_ = other.outStream_;
        return *this;
    }
    to_out_stream_pipe& operator=(to_out_stream_pipe& other) { *this = const_cast<to_out_stream_pipe const&>(other); return *this; }
};
    
template<typename OutStream>
to_out_stream_pipe<OutStream> to_out_stream(OutStream& outStream)
{
    return to_out_stream_pipe<OutStream>(outStream);
}

} // namespace pipes

#endif /* TO_OUT_STREAM_HPP */
