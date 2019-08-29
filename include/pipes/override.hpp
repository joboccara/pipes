#ifndef BEGIN_HPP
#define BEGIN_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/warnings.hpp"

#include <iterator>
#include <utility>

namespace pipes
{
    PIPES_DISABLE_WARNING_PUSH
    PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED
    
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
        
    public: // but technical
        using base = pipeline_base<override_pipeline<Iterator>>;
        using base::operator=;
        override_pipeline& operator=(override_pipeline const& other)
        {
            iterator_ = other.iterator_;
            return *this;
        }
        override_pipeline& operator=(override_pipeline& other) { *this = const_cast<override_pipeline const&>(other); return *this; }
    };
    
    PIPES_DISABLE_WARNING_POP
    
    template<typename Container>
    auto override(Container& container)
    {
        using std::begin;
        return override_pipeline<decltype(begin(std::declval<Container&>()))>{begin(container)};
    }
}
#endif /* BEGIN_HPP */
