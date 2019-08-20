#ifndef PIPES_UNZIP_HPP
#define PIPES_UNZIP_HPP

#include "pipes/operator.hpp"

#include "pipes/output_iterator.hpp"
#include "pipes/helpers/meta.hpp"

#include <tuple>

namespace pipes
{
template<typename... NextPipes>
class unzip_pipe : public OutputIteratorBase<unzip_pipe<NextPipes...>>
{
public:
    template<typename Tuple>
    void onReceive(Tuple&& values)
    {
        detail::for_each2([](auto&& value, auto&& nextPipe) { send(nextPipe, value); }, values, nextPipes_);
    }
    
    explicit unzip_pipe(NextPipes... nextPipes) : nextPipes_(nextPipes...) {}
    
private:
    std::tuple<NextPipes...> nextPipes_;
    
public: // but technical
    using base = OutputIteratorBase<unzip_pipe<NextPipes...>>;
    using base::operator=;
    unzip_pipe& operator=(unzip_pipe const& other)
    {
        nextPipes_ = other.nextPipes_;
        return *this;
    }
    unzip_pipe& operator=(unzip_pipe& other) { *this = const_cast<unzip_pipe const&>(other); return *this; }
};

template<typename... NextPipes>
unzip_pipe<NextPipes...> unzip(NextPipes... nextPipes)
{
    return unzip_pipe<NextPipes...>(nextPipes...);
}
    
} // namespace pipes

#endif /* PIPES_UNZIP_HPP */
