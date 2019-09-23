#ifndef PIPES_SWITCH_HPP
#define PIPES_SWITCH_HPP

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/base.hpp"

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
            auto const firstSatisfyingBranchIndex = detail::find_if(branches_, [&value](auto&& branch){ return branch.predicate(value); });
            if (firstSatisfyingBranchIndex < sizeof...(CaseBranches))
            {
                detail::perform(branches_, firstSatisfyingBranchIndex, [&value](auto&& branch){ send(FWD(value), branch.pipeline); });
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
    struct case_pipe
    {
        Predicate predicate_;
        explicit case_pipe(Predicate predicate) : predicate_(predicate){}
    };
    
    template<typename Predicate, typename Pipeline>
    auto operator>>= (case_pipe<Predicate> pipe, Pipeline&& pipeline)
    {
        return case_branch<Predicate, std::decay_t<Pipeline>>{pipe.predicate_, pipeline};
    }

    template<typename Predicate>
    case_pipe<Predicate> case_(Predicate&& predicate)
    {
        return case_pipe<Predicate>(std::forward<Predicate>(predicate));
    }
    
    auto const default_ = case_([](auto&&){ return true; });

} // namespace pipes

#endif /* PIPES_SWITCH_HPP */
