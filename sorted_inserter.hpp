#ifndef SORTED_INSERTER_HPP
#define SORTED_INSERTER_HPP

#include <iterator>
#include "optional.hpp"

template <typename Container>
class sorted_insert_iterator : public std::iterator<std::output_iterator_tag,void,void,void,void>
{
protected:
  Container* container_;
    fluent::optional<typename Container::iterator> hint_;

public:
  using container_type = Container;
  explicit sorted_insert_iterator (Container& container)
    : container_(&container), hint_(fluent::nullopt) {}
  sorted_insert_iterator (Container& container, typename Container::iterator hint)
    : container_(&container), hint_(hint) {}
  sorted_insert_iterator<Container>& operator= (const typename Container::value_type& value)
    {
        if (hint_)
            container_->insert(*hint_,value);
        else
            container_->insert(value);
        return *this;
    }
  sorted_insert_iterator<Container>& operator= (typename Container::value_type&& value)
    {
        if (hint_)
            container_->insert(hint_,std::move(value));
        else
            container_->insert(std::move(value));
        return *this;
    }
  sorted_insert_iterator<Container>& operator* () { return *this; }
  sorted_insert_iterator<Container>& operator++ () { return *this; }
  sorted_insert_iterator<Container> operator++ (int) { return *this; }
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

#endif
