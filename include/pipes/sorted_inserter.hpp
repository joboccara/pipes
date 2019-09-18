#ifndef PIPES_SORTED_INSERTER_HPP
#define PIPES_SORTED_INSERTER_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/optional.hpp"

#include <iterator>

namespace pipes
{

template <typename Container>
class sorted_insert_iterator : public pipeline_base<sorted_insert_iterator<Container>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        if (hint_)
            container_->insert(*hint_,FWD(value));
        else
            container_->insert(FWD(value));
    }

    explicit sorted_insert_iterator (Container& container) : container_(&container), hint_(detail::nullopt) {}
    sorted_insert_iterator (Container& container, typename Container::iterator hint) : container_(&container), hint_(hint) {}
    
private:
    Container* container_;
    detail::optional<typename Container::iterator> hint_;
};

template <typename Container>
sorted_insert_iterator<Container> sorted_inserter(Container& container)
{
    return sorted_insert_iterator<Container>(container);
}

template <typename Container>
sorted_insert_iterator<Container> sorted_inserter(Container& container, typename Container::iterator hint)
{
    return sorted_insert_iterator<Container>(container, hint);
}

} // namespace pipes

#endif // PIPES_SORTED_INSERTER_HPP
