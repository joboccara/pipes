#ifndef PIPES_SWITCH_HPP
#define PIPES_SWITCH_HPP

#include "helpers/assignable.hpp"
#include "helpers/meta.hpp"
#include "helpers/warnings.hpp"
#include "output_iterator.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
    
template<typename Predicate, typename OutputPipe>
struct case_branch
{
    detail::assignable<Predicate> predicate;
    OutputPipe outputPipe;
    case_branch(Predicate predicate, OutputPipe outputPipe) : predicate(predicate), outputPipe(outputPipe) {}
};

template<typename... CaseBranches>
class switch_pipe : public OutputIteratorBase<switch_pipe<CaseBranches...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        auto const firstSatisfyingBranchIndex = detail::find_if(branches_, [&value](auto&& branch){ return branch.predicate(value); });
        if (firstSatisfyingBranchIndex < sizeof...(CaseBranches))
        {
            detail::perform(branches_, firstSatisfyingBranchIndex, [&value](auto&& branch){ send(branch.outputPipe, value); });
        }
    }

    explicit switch_pipe(CaseBranches const&... caseBranches) : branches_(std::make_tuple(caseBranches...)) {}
    
private:
    std::tuple<CaseBranches...> branches_;
    
public: // but technical
    using OutputIteratorBase<switch_pipe<CaseBranches...>>::operator=;
    switch_pipe& operator=(switch_pipe const& other)
    {
        branches_ = other.branches_;
        return *this;
    }
    switch_pipe& operator=(switch_pipe& other) { *this = const_cast<switch_pipe const&>(other); return *this; }
};

template<typename... CaseBranches>
switch_pipe<CaseBranches...> switch_(CaseBranches const&... caseBranches)
{
    return switch_pipe<CaseBranches...>(caseBranches...);
}

template<typename Predicate>
class Case_
{
public:
    explicit constexpr Case_(Predicate const& predicate) : predicate_(predicate) {}
    Predicate const& get() const {return predicate_;}
private:
    Predicate const& predicate_;
};

template<typename Predicate>
Case_<Predicate> case_(Predicate&& predicate)
{
    return Case_<Predicate>(std::forward<Predicate>(predicate));
}
    
auto const default_ = case_([](auto&&){ return true; });

template<typename Predicate, typename OutputPipe>
case_branch<Predicate, OutputPipe> operator>>=(Case_<Predicate> const& casePredicate, OutputPipe outputPipe)
{
    return case_branch<Predicate, OutputPipe>{casePredicate.get(), outputPipe};
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_SWITCH_HPP */
