#ifndef PIPES_join_HPP
#define PIPES_join_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/FWD.hpp"

namespace pipes
{
    template<typename TailPipeline>
    class join_pipeline : public pipeline_base<join_pipeline<TailPipeline>>
    {
    public:
        template<typename T>
        void onReceive(T&& inputCollection)
        {
            std::copy(begin(inputCollection), end(inputCollection), tailPipeline_);
        }
        
        explicit join_pipeline(TailPipeline tailPipeline) : tailPipeline_(tailPipeline) {}
        
    private:
        TailPipeline tailPipeline_;
    };
    
    struct join_pipe
    {
        template<typename Pipeline>
        auto plug_to_pipeline(Pipeline&& pipeline) const
        {
            return join_pipeline<std::decay_t<Pipeline>>{pipeline};
        }
    };
    
    static auto constexpr join = join_pipe{};
} // namespace pipes

#endif /* PIPES_join_HPP */
