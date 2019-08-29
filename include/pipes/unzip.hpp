#ifndef PIPES_UNZIP_HPP
#define PIPES_UNZIP_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"

#include <tuple>
#include <type_traits>

namespace pipes
{

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

template<typename... TailPipelines>
class unzip_pipeline : public pipeline_base<unzip_pipeline<TailPipelines...>>
{
public:
    template<typename Tuple>
    void onReceive(Tuple&& values)
    {
        detail::for_each2([](auto&& value, auto&& tailPipe) { send(tailPipe, value); }, FWD(values), tailPipes_);
    }
    
    explicit unzip_pipeline(TailPipelines... tailPipes) : tailPipes_(tailPipes...) {}
    
private:
    std::tuple<TailPipelines...> tailPipes_;
    
public: // but technical
    using base = pipeline_base<unzip_pipeline<TailPipelines...>>;
    using base::operator=;
    unzip_pipeline& operator=(unzip_pipeline const& other)
    {
        tailPipes_ = other.tailPipes_;
        return *this;
    }
    unzip_pipeline& operator=(unzip_pipeline& other) { *this = const_cast<unzip_pipeline const&>(other); return *this; }
};

PIPES_DISABLE_WARNING_POP
    
template<typename... TailPipelines>
unzip_pipeline<TailPipelines...> unzip(TailPipelines... tailPipes)
{
    return unzip_pipeline<std::decay_t<TailPipelines>...>(tailPipes...);
}
    
} // namespace pipes

#endif /* PIPES_UNZIP_HPP */
