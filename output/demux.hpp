#ifndef output_demuxer_h
#define output_demuxer_h

#include "../helpers/meta.hpp"
#include "../output_iterator.hpp"

namespace pipes
{
    
template<typename Predicate, typename OutputPipe>
struct demux_branch
{
    Predicate predicate;
    OutputPipe outputPipe;
    demux_branch(Predicate predicate, OutputPipe outputPipe) : predicate(predicate), outputPipe(outputPipe) {}
};

template<typename... DemuxBranches>
class demux_pipe : public OutputIteratorBase<demux_pipe<DemuxBranches...>>
{
public:
    explicit demux_pipe(DemuxBranches const&... demuxBranches) : branches_(std::make_tuple(demuxBranches...)) {}

    template<typename T>
    void onReceive(T&& value)
    {
        auto const firstSatisfyingBranchIndex = detail::find_if(branches_, [&value](auto&& branch){ return branch.predicate(value); });
        if (firstSatisfyingBranchIndex < sizeof...(DemuxBranches))
        {
            detail::perform(branches_, firstSatisfyingBranchIndex, [&value](auto&& branch){ send(branch.outputPipe, value); });
        }
    }
    
public: // but technical
    using OutputIteratorBase<demux_pipe<DemuxBranches...>>::operator=;
    friend OutputIteratorBase<demux_pipe<DemuxBranches...>>;

private:
    std::tuple<DemuxBranches...> branches_;
};

namespace output
{

template<typename... DemuxBranches>
demux_pipe<DemuxBranches...> demux(DemuxBranches const&... demuxBranches)
{
    return demux_pipe<DemuxBranches...>(demuxBranches...);
}

} // namespace output
    
template<typename Predicate>
class Demux_if
{
public:
    explicit Demux_if(Predicate predicate) : predicate_(std::move(predicate)) {}
    
    template<typename OutputPipe>
    auto send_to(OutputPipe&& outputPipe) const &
    {
        return demux_branch<Predicate, OutputPipe>(predicate_, std::forward<OutputPipe>(outputPipe));
    }
    
    template<typename OutputPipe>
    auto send_to(OutputPipe&& outputPipe) &&
    {
        return demux_branch<Predicate, OutputPipe>(std::move(predicate_), std::forward<OutputPipe>(outputPipe));
    }
    
private:
    Predicate predicate_;
};

template<typename Predicate>
Demux_if<Predicate> demux_if(Predicate&& predicate)
{
    return Demux_if<Predicate>(std::forward<Predicate>(predicate));
}

} // namespace pipes

#endif /* output_demuxer_h */
