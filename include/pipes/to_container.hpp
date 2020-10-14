#ifndef TO_CONTAINER_HPP
#define TO_CONTAINER_HPP

#include "pipes/base.hpp"
#include "pipes/helpers/FWD.hpp"

#include <functional>
#include <iostream>

namespace pipes
{
    template<typename Container>
    class to_ : public pipeline_base<to_<Container>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            container_.push_back(FWD(value));
        }
        
		auto move_reduced_value_from()
		{
			return std::exchange(container_, {});
		}
    private:
		Container container_;
    };

}

#endif /* TO_CONTAINER_HPP */
