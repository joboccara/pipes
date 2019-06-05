#ifndef output_demux_h
#define output_demux_h

#include "helpers/meta.hpp"
#include "output_iterator.hpp"

namespace pipes
{

template<typename... OutputPipes>
class demux_pipe : public OutputIteratorBase<demux_pipe<OutputPipes...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        detail::apply([&value](auto&& outputPipe){ send(outputPipe, value); }, outputPipes_);
    }

    explicit demux_pipe(OutputPipes const&... outputPipes) : outputPipes_(outputPipes...) {}
    
private:
    std::tuple<OutputPipes...> outputPipes_;
    
public: // but technical
    using OutputIteratorBase<demux_pipe<OutputPipes...>>::operator=;
    friend OutputIteratorBase<demux_pipe<OutputPipes...>>;
};

template<typename... OutputPipes>
demux_pipe<OutputPipes...> demux(OutputPipes const&... outputPipes)
{
    return demux_pipe<OutputPipes...>(outputPipes...);
}

} // namespace pipes

#endif /* output_demux_h */
