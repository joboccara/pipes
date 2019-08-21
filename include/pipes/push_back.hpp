#ifndef PUSH_BACK_HPP
#define PUSH_BACK_HPP

#include "pipes/operator.hpp"

#include "pipes/output_iterator.hpp"
#include "pipes/helpers/FWD.hpp"

#include <functional>

namespace pipes
{
    template<typename Container>
    class push_back_pipeline : public OutputIteratorBase<push_back_pipeline<Container>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            container_.get().push_back(value);
        }
        
        explicit push_back_pipeline(Container& container) : container_(container) {}
        
    private:
        std::reference_wrapper<Container> container_;
        
    public: // but technical
        using base = OutputIteratorBase<push_back_pipeline<Container>>;
        using base::operator=;
        push_back_pipeline& operator=(push_back_pipeline const& other)
        {
            container_ = other.container_;
            return *this;
        }
        push_back_pipeline& operator=(push_back_pipeline& other) { *this = const_cast<push_back_pipeline const&>(other); return *this; }
    };
    
    template<typename Container>
    push_back_pipeline<Container> push_back(Container& container)
    {
        return push_back_pipeline<Container>(container);
    }
}

#endif /* PUSH_BACK_HPP */
