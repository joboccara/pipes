#ifndef PIPES_UNZIP_HPP
#define PIPES_UNZIP_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"

#include <tuple>

namespace pipes
{

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

template<typename... NextPipes>
class unzip_pipe : public pipeline_base<unzip_pipe<NextPipes...>>
{
public:
    template<typename Tuple>
    void onReceive(Tuple&& values)
    {
        detail::for_each2([](auto&& value, auto&& nextPipe) { send(nextPipe, value); }, FWD(values), nextPipes_);
    }
    
    explicit unzip_pipe(NextPipes... nextPipes) : nextPipes_(nextPipes...) {}
    
private:
    std::tuple<NextPipes...> nextPipes_;
    
public: // but technical
    using base = pipeline_base<unzip_pipe<NextPipes...>>;
    using base::operator=;
    unzip_pipe& operator=(unzip_pipe const& other)
    {
        nextPipes_ = other.nextPipes_;
        return *this;
    }
    unzip_pipe& operator=(unzip_pipe& other) { *this = const_cast<unzip_pipe const&>(other); return *this; }
};

PIPES_DISABLE_WARNING_POP
    
template<typename... NextPipes>
unzip_pipe<NextPipes...> unzip(NextPipes... nextPipes)
{
    return unzip_pipe<NextPipes...>(nextPipes...);
}
    
} // namespace pipes

#endif /* PIPES_UNZIP_HPP */
