#ifndef BEGIN_HPP
#define BEGIN_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"

#include <iterator>
#include <utility>

namespace pipes
{
    template<typename Iterator>
    class override_pipeline : public pipeline_base<override_pipeline<Iterator>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            *iterator_ = FWD(value);
            ++iterator_;
        }
        
        explicit override_pipeline(Iterator iterator) : iterator_(iterator) {}
    private:
        Iterator iterator_;
    };
    
    template<typename Container>
    auto override(Container& container)
    {
        using std::begin;
        return override_pipeline<decltype(begin(std::declval<Container&>()))>{begin(container)};
    }

    template<typename Iterator, typename DataMember>
    class override_data_member_pipeline : public pipeline_base<override_data_member_pipeline<Iterator, DataMember>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            (*iterator_).*dataMember_ = FWD(value);
            ++iterator_;
        }
        
        override_data_member_pipeline(Iterator iterator, DataMember dataMember) : iterator_(iterator), dataMember_(dataMember) {}
    private:
        Iterator iterator_;
        DataMember dataMember_;
    };

    template<typename Container, typename DataMember>
    auto override(Container& container, DataMember dataMember)
    {
        using std::begin;
        return override_data_member_pipeline<decltype(begin(std::declval<Container&>())), DataMember>{begin(container), dataMember};
    }

    template<typename Iterator, typename Setter>
    class override_member_function_pipeline : public pipeline_base<override_member_function_pipeline<Iterator, Setter>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            ((*iterator_).*setter_)(FWD(value));
            ++iterator_;
        }
        
        override_member_function_pipeline(Iterator iterator, Setter setter) : iterator_(iterator), setter_(setter) {}
    private:
        Iterator iterator_;
        Setter setter_;
    };

    template<typename Container, typename Ret, typename Object, typename Value>
    auto override(Container& container, Ret (Object::* setter)(Value))
    {
        using std::begin;
        return override_member_function_pipeline<decltype(begin(std::declval<Container&>())), decltype(setter)>{begin(container), setter};
    }
}
#endif /* BEGIN_HPP */
