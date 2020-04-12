#ifndef REPLACE_HPP
#define REPLACE_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/meta.hpp"

#include <functional>

namespace pipes
{
    template<typename Container>
    class replace_pipeline : public pipeline_base<replace_pipeline<Container>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            container_.get().push_back(FWD(value));
        }

        explicit replace_pipeline(Container& container)
            : container_(container)
        {
            static_assert(detail::is_clearable_v<Container>, "Container must have a clear() method");
            container_.get().clear();
        }

    private:
        std::reference_wrapper<Container> container_;
    };

    template<typename Container>
    replace_pipeline<Container> replace(Container& container)
    {
        return replace_pipeline<Container>(container);
    }
} // namespace pipes

#endif /* REPLACE_HPP */
