#ifndef teehpp_hpp
#define teehpp_hpp

#include "output_iterator.hpp"

namespace pipes
{
    
template<typename TeeOutputPipe, typename NextOutputPipe>
class tee_pipe : public OutputIteratorBase<tee_pipe<TeeOutputPipe, NextOutputPipe>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        send(teeOutputPipe_, value);
        send(nextOutputPipe_, value);
    }
    
    tee_pipe(TeeOutputPipe const& teeOutputPipe, NextOutputPipe const& nextOutputPipe) : teeOutputPipe_(teeOutputPipe), nextOutputPipe_(nextOutputPipe){}

private:
    TeeOutputPipe teeOutputPipe_;
    NextOutputPipe nextOutputPipe_;

public: // but technical
    using OutputIteratorBase<tee_pipe<TeeOutputPipe, NextOutputPipe>>::operator=;
};
    
template<typename TeeOutputPipe>
struct TeeOutputPipeWrapper{ TeeOutputPipe const& teeOutputPipe;};

template<typename TeeOutputPipe>
TeeOutputPipeWrapper<TeeOutputPipe> tee(TeeOutputPipe const& teeOutputPipe)
{
    return TeeOutputPipeWrapper<TeeOutputPipe>{teeOutputPipe};
}
    
template<typename NextOutputPipe, typename TeeOutputPipe>
tee_pipe<NextOutputPipe, TeeOutputPipe> operator>>=(TeeOutputPipeWrapper<TeeOutputPipe> const& teeOutputPipeWrapper, NextOutputPipe const& nextOutputPipe)
{
    return tee_pipe<NextOutputPipe, TeeOutputPipe>(nextOutputPipe, teeOutputPipeWrapper.teeOutputPipe);
}
    
} // namespace pipes

#endif /* tee_hpp */
