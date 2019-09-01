#ifndef PIPES_PIPES_ASSEMBLY
#define PIPES_PIPES_ASSEMBLY



namespace pipes
{
    namespace detail
    {

        template<typename HeadPipe, typename TailPipeline>
        class generic_pipeline : public pipeline_base<generic_pipeline<HeadPipe, TailPipeline>>
        {
        public:
            template<typename... Ts>
            void onReceive(Ts&&... inputs)
            {
                headPipe_.template onReceive<Ts...>(FWD(inputs)..., tailPipeline_);
            }
            
            generic_pipeline(HeadPipe headPipe, TailPipeline tailPipeline) : headPipe_(headPipe), tailPipeline_(tailPipeline) {}
            
        private:
            HeadPipe headPipe_;
            TailPipeline tailPipeline_;
        };

        template<typename Pipe1, typename Pipe2>
        struct CompositePipe : public pipe_base
        {
            Pipe1 pipe1;
            Pipe2 pipe2;
            
            template<typename Pipe1_, typename Pipe2_>
            CompositePipe(Pipe1_&& pipe1, Pipe2_&& pipe2) : pipe1(FWD(pipe1)), pipe2(FWD(pipe2)){}
        };

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
        
    } // namespace detail
} // namespace pipes

#endif /* PIPES_PIPES_ASSEMBLY */
