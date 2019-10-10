#ifndef PIPES_REPEAT_HPP
#define PIPES_REPEAT_HPP

#include "pipes/operator.hpp"

#include "pipes/base.hpp"
#include "pipes/helpers/assignable.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{

class repeat : public pipe_base
{
public:
  template<typename Value, typename TailPipeline>
  void onReceive(Value&& value, TailPipeline&& tailPipeline)
  {
    for (uint64_t i = 0; i < n_; ++i) {
      send(value, tailPipeline);
    }
    send(FWD(value), tailPipeline);
  }

  explicit repeat(size_t n) : n_(n) {}

private:
  size_t n_;
};

} // namespace pipes

#endif /* PIPES_REPEAT_HPP */