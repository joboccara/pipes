#ifndef MOVE_HPP
#define MOVE_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/FWD.hpp"

#include <algorithm>

namespace pipes
{
    
    template<typename TailPipeline>
    class move_pipeline : public pipeline_base<move_pipeline<TailPipeline>>
    {
    public:
        template<typename T>
        void onReceive(T&& value)
        {
            send(tailPipeline_, FWD(value));
        }
        
        explicit move_pipeline(TailPipeline tailPipeline) : tailPipeline_(tailPipeline) {}
        
    private:
        TailPipeline tailPipeline_;
        
    public: // but technical
        using base = pipeline_base<move_pipeline<TailPipeline>>;
        using base::operator=;
        move_pipeline& operator=(move_pipeline const& other)
        {
            tailPipeline_ = other.tailPipeline_;
            return *this;
        }
        move_pipeline& operator=(move_pipeline& other) { *this = const_cast<move_pipeline const&>(other); return *this; }
    };
    
    class move_pipe
    {
    public:
        template<typename Pipeline>
        auto plug_to_pipeline(Pipeline&& pipeline) const
        {
            return move_pipeline<std::remove_reference_t<Pipeline>>{pipeline};
        }
    };
    
    static constexpr move_pipe move;
    
    template<typename MoveableRange, typename TailPipeline>
    void operator>>= (MoveableRange&& range, move_pipeline<TailPipeline> movePipeline)
    {
        std::move(begin(range), end(range), movePipeline);
    }
    
} // namespace pipes

#endif /* MOVE_HPP */
