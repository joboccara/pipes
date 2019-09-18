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
}
#endif /* BEGIN_HPP */
