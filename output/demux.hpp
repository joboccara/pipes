#ifndef output_demuxer_h
#define output_demuxer_h

#include "../helpers/meta.hpp"
#include "../helpers/named_type.hpp"

namespace fluent
{
    
namespace detail
{
    template<typename Predicate>
    using ExecuteOnFirst_Predicate = detail::NamedType<Predicate, struct ExecuteOnFirst_Predicate_Tag>;
    
    template<typename Function>
    using ExecuteOnFirst_Function = detail::NamedType<Function, struct ExecuteOnFirst_Function_Tag>;
    
    template <typename Predicate, typename Function>
    struct Executor_on_first_that_satisfies_predicate
    {
        Executor_on_first_that_satisfies_predicate(ExecuteOnFirst_Predicate<Predicate> p, ExecuteOnFirst_Function<Function> f) : p_(p.get()), f_(f.get()) {}
        Predicate p_;
        Function f_;
        bool hasExecutedAlready = false;
        
        template<typename T>
        void operator()(T&& value)
        {
            if (!hasExecutedAlready)
            {
                if (p_(value))
                {
                    f_(value);
                    hasExecutedAlready = true;
                }
            }
        }
    };
    
    template<typename Predicate, typename Function>
    Executor_on_first_that_satisfies_predicate<Predicate, Function> make_executor_on_first_that_satisfies_predicate(ExecuteOnFirst_Predicate<Predicate> p, ExecuteOnFirst_Function<Function> f)
    {
        return Executor_on_first_that_satisfies_predicate<Predicate, Function>(p, f);
    }
    
    template <typename Tuple, typename Predicate, typename Function>
    void execute_on_first_that_satisfies_predicate(Tuple&& tuple, ExecuteOnFirst_Predicate<Predicate> p, ExecuteOnFirst_Function<Function> f)
    {
        auto executor_on_first_that_satisfies_predicate = detail::make_executor_on_first_that_satisfies_predicate(p, f);
        apply(executor_on_first_that_satisfies_predicate, tuple);
    }
    
} // namespace detail

template<typename Predicate, typename Iterator>
struct demux_branch
{
    using iterator_type = Iterator;
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
        execute_on_first_that_satisfies_predicate(branches_,
                                                  detail::make_named<detail::ExecuteOnFirst_Predicate>([&value](auto&& branch){ return branch.predicate(value); }),
                                                  detail::make_named<detail::ExecuteOnFirst_Function>([&value](auto&& branch){ *branch.iterator = value; ++branch.iterator; } ));
        
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
    Demux_if(Predicate predicate) : predicate_(std::move(predicate)) {}
    
    template<typename Iterator>
    auto sendTo(Iterator&& iterator) const &
    {
        return demux_branch<Predicate, Iterator>(predicate_, std::forward<Iterator>(iterator));
    }
    
    template<typename Iterator>
    auto sendTo(Iterator&& iterator) &&
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

} // namespace fluent

#endif /* output_demuxer_h */
