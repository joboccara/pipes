#ifndef PIPES_TAKE_HPP
#define PIPES_TAKE_HPP

namespace pipes
{
    class take_pipe : public pipe_base
    {
    public:
        template<typename... Values, typename TailPipeline>
        void onReceive(Values&&... values, TailPipeline&& tailPipeline)
        {
            if (nbTaken_ < nbToTake_)
            {
                send(FWD(values)..., tailPipeline);
                ++nbTaken_;
            }
        }
        
        explicit take_pipe(size_t nbToTake_) : nbToTake_{nbToTake_}, nbTaken_{0} {}
        
    private:
        size_t nbToTake_;
        size_t nbTaken_;
    };
    
    inline take_pipe take(size_t nbToTake)
    {
        return take_pipe{nbToTake};
    }
    
} // namespace pipes

#endif /* PIPES_TAKE_HPP */
