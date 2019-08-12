#ifndef PIPES_FUNNEL_HPP
#define PIPES_FUNNEL_HPP

namespace pipes
{

struct Funnel {};
const Funnel funnel{};

template<typename Pipe>
class pipe_entrance
{
public:
    explicit pipe_entrance(Pipe pipe) : pipe_(pipe) {}
    Pipe get() const { return pipe_; }
private:
    Pipe pipe_;
};

template<typename Pipe>
pipe_entrance<Pipe> operator>>=(Funnel, Pipe pipe)
{
    return pipe_entrance<Pipe>(pipe);
}

template<typename Range, typename Pipe>
void operator>>=(Range&& range, pipe_entrance<Pipe> const& pipeEntrance)
{
    std::copy(begin(range), end(range), pipeEntrance.get());
}

} // namespace pipes

#endif /* PIPES_FUNNEL_HPP */
