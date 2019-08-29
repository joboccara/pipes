#ifndef PIPES_SWITCH_HPP
#define PIPES_SWITCH_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/pipeline_base.hpp"

namespace pipes
{
    
template<typename Predicate, typename Pipeline>
struct case_branch
{
    detail::assignable<Predicate> predicate;
    Pipeline pipeline;
    case_branch(Predicate predicate, Pipeline pipeline) : predicate(predicate), pipeline(pipeline) {}
};

template<typename... CaseBranches>
class switch_pipeline : public pipeline_base<switch_pipeline<CaseBranches...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        auto const firstSatisfyingBranchIndex = detail::find_if(branches_, [&value](auto&& branch){ return branch.predicate(FWD(value)); });
        if (firstSatisfyingBranchIndex < sizeof...(CaseBranches))
        {
            detail::perform(branches_, firstSatisfyingBranchIndex, [&value](auto&& branch){ send(branch.pipeline, FWD(value)); });
        }
    }

    explicit switch_pipeline(CaseBranches const&... caseBranches) : branches_(std::make_tuple(caseBranches...)) {}
    
private:
    std::tuple<CaseBranches...> branches_;
};

template<typename... CaseBranches>
switch_pipeline<CaseBranches...> switch_(CaseBranches const&... caseBranches)
{
    return switch_pipeline<CaseBranches...>(caseBranches...);
}

template<typename Predicate>
class case_pipe
{
public:
    template<typename Pipeline>
    auto plug_to_pipeline(Pipeline&& pipeline) const
    {
        return case_branch<Predicate, std::remove_reference_t<Pipeline>>{predicate_, pipeline};
    }
    
    explicit case_pipe(Predicate predicate) : predicate_(predicate){}
    
private:
    Predicate predicate_;
};

template<typename Predicate>
case_pipe<Predicate> case_(Predicate&& predicate)
{
    return case_pipe<Predicate>(std::forward<Predicate>(predicate));
}
    
auto const default_ = case_([](auto&&){ return true; });

} // namespace pipes

#endif /* PIPES_SWITCH_HPP */
