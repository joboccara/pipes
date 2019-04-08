#ifndef output_demuxer_h
#define output_demuxer_h

#include "../helpers/meta.hpp"

namespace pipes
{
    
template<typename Predicate, typename Iterator>
struct demux_branch
{
    Predicate predicate;
    Iterator iterator;
    demux_branch(Predicate predicate, Iterator iterator) : predicate(predicate), iterator(iterator) {}
};

template<typename... DemuxBranches>
class output_demux_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    explicit output_demux_iterator(DemuxBranches const&... demuxBranches) : branches_(std::make_tuple(demuxBranches...)) {}
    output_demux_iterator& operator++() { return *this; }
    output_demux_iterator& operator++(int){ ++*this; return *this; }
    output_demux_iterator& operator*(){ return *this; }
    template<typename T>
    output_demux_iterator& operator=(T&& value)
    {
        auto const firstSatisfyingBranchIndex = detail::find_if(branches_, [&value](auto&& branch){ return branch.predicate(value); });
        if (firstSatisfyingBranchIndex < sizeof...(DemuxBranches))
        {
            detail::perform(branches_, firstSatisfyingBranchIndex, [&value](auto&& branch){ *branch.iterator = value; ++ branch.iterator; });
        }
        return *this;
    }
    
    output_demux_iterator& operator=(output_demux_iterator const&) = default;
    output_demux_iterator& operator=(output_demux_iterator&&) = default;
    output_demux_iterator(output_demux_iterator const&) = default;
    output_demux_iterator(output_demux_iterator&&) = default;
    
private:
    std::tuple<DemuxBranches...> branches_;
};

namespace output
{

template<typename... DemuxBranches>
output_demux_iterator<DemuxBranches...> demux(DemuxBranches const&... demuxBranches)
{
    return output_demux_iterator<DemuxBranches...>(demuxBranches...);
}

} // namespace output
    
template<typename Predicate>
class Demux_if
{
public:
    explicit Demux_if(Predicate predicate) : predicate_(std::move(predicate)) {}
    
    template<typename Iterator>
    auto send_to(Iterator&& iterator) const &
    {
        return demux_branch<Predicate, Iterator>(predicate_, std::forward<Iterator>(iterator));
    }
    
    template<typename Iterator>
    auto send_to(Iterator&& iterator) &&
    {
        return demux_branch<Predicate, Iterator>(std::move(predicate_), std::forward<Iterator>(iterator));
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
