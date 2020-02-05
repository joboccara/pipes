#ifndef PIPES_SORTED_INSERTER_HPP
#define PIPES_SORTED_INSERTER_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/optional.hpp"

#include <iterator>

namespace pipes
{

template <typename Container>
class insert_iterator : public pipeline_base<insert_iterator<Container>>
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

    explicit insert_iterator (Container& container) : container_(&container), hint_(detail::nullopt) {}
    insert_iterator (Container& container, typename Container::iterator hint) : container_(&container), hint_(hint) {}
    
private:
    Container* container_;
    detail::optional<typename Container::iterator> hint_;
};

template <typename Container>
insert_iterator<Container> insert(Container& container)
{
    return insert_iterator<Container>(container);
}

template <typename Container>
insert_iterator<Container> insert(Container& container, typename Container::iterator hint)
{
    return insert_iterator<Container>(container, hint);
}

} // namespace pipes

#endif // PIPES_SORTED_INSERTER_HPP
