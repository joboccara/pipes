#ifndef PIPES_OUTPUT_ITERATOR_HPP
#define PIPES_OUTPUT_ITERATOR_HPP

#include "pipes/helpers/crtp.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <iterator>

namespace pipes
{
    
template<typename Pipeline, typename T>
void send(Pipeline& pipeline, T&& value)
{
    pipeline.onReceive(FWD(value));
}

template<typename Derived>
struct pipeline_base : detail::crtp<Derived, pipeline_base>
{
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    Derived& operator++() { return this->derived(); }
    Derived& operator++(int){ ++this->derived(); return this->derived(); }
    Derived& operator*() { return this->derived(); }
    
    template<typename T>
    Derived& operator=(T&& input)
    {
        send(this->derived(), FWD(input));
        return this->derived();
    }
};

} // namespace pipes

#endif /* PIPES_OUTPUT_ITERATOR_HPP */
