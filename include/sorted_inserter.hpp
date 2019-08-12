#ifndef PIPES_SORTED_INSERTER_HPP
#define PIPES_SORTED_INSERTER_HPP

#include <iterator>
#include "optional.hpp"
#include "output_iterator.hpp"
#include "helpers/warnings.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template <typename Container>
class sorted_insert_iterator : public OutputIteratorBase<sorted_insert_iterator<Container>>
{
public:
    template<typename T>
    void onReceive(T const& value)
    {
        if (hint_)
            container_->insert(*hint_,value);
        else
            container_->insert(value);
    }

    explicit sorted_insert_iterator (Container& container) : container_(&container), hint_(detail::nullopt) {}
    sorted_insert_iterator (Container& container, typename Container::iterator hint) : container_(&container), hint_(hint) {}
    
protected:
    Container* container_;
    detail::optional<typename Container::iterator> hint_;
    
public: // but technical
    using OutputIteratorBase<sorted_insert_iterator<Container>>::operator=;
    sorted_insert_iterator& operator=(sorted_insert_iterator const& other)
    {
        container_ = other.container_;
        hint_ = other.hint_;
        return *this;
    }
    sorted_insert_iterator& operator=(sorted_insert_iterator& other){ *this = const_cast<sorted_insert_iterator const&>(other); return *this; }
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

PIPES_DISABLE_WARNING_POP

#endif // PIPES_SORTED_INSERTER_HPP
