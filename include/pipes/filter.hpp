#ifndef PIPES_FILTER_HPP
#define PIPES_FILTER_HPP

#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"
#include "pipes/output_iterator.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
template<typename Predicate, typename NextPipe>
class filter_pipe : public OutputIteratorBase<filter_pipe<Predicate, NextPipe>>
{
public:    
    template<typename T>
    void onReceive(T const& value)
    {
        if (predicate_(value))
        {
            send(nextPipe_, value);
        }
    }

    explicit filter_pipe(Predicate predicate, NextPipe nextPipe) : predicate_(predicate), nextPipe_(nextPipe) {}
    
private:
    detail::assignable<Predicate> predicate_;
    NextPipe nextPipe_;

public: // but technical
    using base = OutputIteratorBase<filter_pipe<Predicate, NextPipe>>;
    using base::operator=;
    filter_pipe& operator=(filter_pipe const& other)
    {
        predicate_ = other.predicate_;
        nextPipe_ = other.nextPipe_;
        return *this;
    }
    filter_pipe& operator=(filter_pipe& other) { *this = const_cast<filter_pipe const&>(other); return *this; }
};

template<typename Predicate>
struct FilterPredicateWrapper{ Predicate const& predicate; };

template<typename Predicate>
FilterPredicateWrapper<Predicate> filter(Predicate const& predicate)
{
    return FilterPredicateWrapper<Predicate>{predicate};
}

template<typename Predicate, typename NextPipe>
filter_pipe<Predicate, NextPipe> operator>>= (FilterPredicateWrapper<Predicate> const& filterPredicateWrapper, NextPipe const& nextPipe)
{
    return filter_pipe<Predicate, NextPipe>{filterPredicateWrapper.predicate, nextPipe};
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_FILTER_HPP */
