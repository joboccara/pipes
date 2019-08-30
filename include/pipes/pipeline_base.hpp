#ifndef PIPES_OUTPUT_ITERATOR_HPP
#define PIPES_OUTPUT_ITERATOR_HPP

#include "pipes/helpers/crtp.hpp"
#include "pipes/helpers/detect.hpp"
#include "pipes/helpers/FWD.hpp"

#include <iterator>

namespace pipes
{
    struct pipe_base {};
    
    template<typename Pipeline, typename T>
    void send(Pipeline& pipeline, T&& value)
    {
        pipeline.onReceive(FWD(value));
    }

    template<typename Pipeline>
    struct pipeline_proxy
    {
        template<typename T>
        pipeline_proxy& operator=(T&& input)
        {
            send(pipeline_, FWD(input));
            return *this;
        }
        
        explicit pipeline_proxy(Pipeline& pipeline) : pipeline_(pipeline){}
        
    private:
        Pipeline& pipeline_;
    };

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
        pipeline_proxy<Derived> operator*() { return pipeline_proxy<Derived>(this->derived()); }
    };

    template<typename HeadPipe, typename TailPipeline>
    class generic_pipeline : public pipeline_base<generic_pipeline<HeadPipe, TailPipeline>>
    {
    public:
        template<typename T>
        void onReceive(T&& input)
        {
            headPipe_.onReceive(FWD(input), tailPipeline_);
        }
        
        generic_pipeline(HeadPipe headPipe, TailPipeline tailPipeline) : headPipe_(headPipe), tailPipeline_(tailPipeline) {}
        
    private:
        HeadPipe headPipe_;
        TailPipeline tailPipeline_;
    };
    
    namespace detail
    {
        template<typename Pipe1, typename Pipe2>
        struct CompositePipe : public pipe_base
        {
            Pipe1 pipe1;
            Pipe2 pipe2;
            
            template<typename Pipe1_, typename Pipe2_>
            CompositePipe(Pipe1_&& pipe1, Pipe2_&& pipe2) : pipe1(FWD(pipe1)), pipe2(FWD(pipe2)){}
        };
    } // namespace detail
    
    template<typename HeadPipe, typename TailPipeline>
    auto make_generic_pipeline(HeadPipe&& headPipe, TailPipeline&& tailPipeline)
    {
        return generic_pipeline<std::decay_t<HeadPipe>, std::decay_t<TailPipeline>>{FWD(headPipe), FWD(tailPipeline)};
    }
    
    template<typename Pipe1, typename Pipe2, typename TailPipeline>
    auto make_generic_pipeline(detail::CompositePipe<Pipe1, Pipe2> compositePipe, TailPipeline&& tailPipeline)
    {
        return make_generic_pipeline(compositePipe.pipe1, make_generic_pipeline(compositePipe.pipe2, tailPipeline));
    }
    
} // namespace pipes

#endif /* PIPES_OUTPUT_ITERATOR_HPP */
