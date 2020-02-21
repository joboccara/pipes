#ifndef PIPES_SORTED_INSERTER_HPP
#define PIPES_SORTED_INSERTER_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/optional.hpp"

#include <iterator>
#include <utility>

namespace pipes
{
    namespace detail
    {
        template<typename Container>
        using HasInserterWithNoPositionExpression = decltype(std::declval<Container>().insert(std::declval<typename Container::value_type>()));
    
        template<typename Container>
        constexpr bool HasInserterWithNoPosition = is_detected<HasInserterWithNoPositionExpression, Container>;
    }

template <typename Container>
class insert_iterator_with_no_position : public pipeline_base<insert_iterator_with_no_position<Container>>
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

    explicit insert_iterator_with_no_position (Container& container) : container_(&container), hint_(detail::nullopt) {}
    insert_iterator_with_no_position (Container& container, typename Container::iterator hint) : container_(&container), hint_(hint) {}
    
private:
    Container* container_;
    detail::optional<typename Container::iterator> hint_;
};

template <typename Container>
class insert_iterator_with_position : public pipeline_base<insert_iterator_with_position<Container>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        position_ = container_->insert(position_, FWD(value));
        ++position_;
    }

    insert_iterator_with_position (Container& container, typename Container::iterator position) : container_(&container), position_(position) {}
    
private:
    Container* container_;
    typename Container::iterator position_;
};

template<typename Container, bool>
struct insert_iterator_type
{
    using type = insert_iterator_with_no_position<Container>;
};

template<typename Container>
struct insert_iterator_type<Container, false>
{
    using type = insert_iterator_with_position<Container>;
};

template<typename Container, bool HasNoPosition>
using insert_iterator_t = typename insert_iterator_type<Container, HasNoPosition>::type;

template <typename Container>
auto insert(Container& container)
{
    return insert_iterator_t<Container, detail::HasInserterWithNoPosition<Container>>{container};
}

template <typename Container>
auto insert(Container& container, typename Container::iterator position)
{
    return insert_iterator_t<Container, detail::HasInserterWithNoPosition<Container>>{container, position};
}

} // namespace pipes

#endif // PIPES_SORTED_INSERTER_HPP
