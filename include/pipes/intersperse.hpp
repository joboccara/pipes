#ifndef PIPES_INTERSPERSE_HPP
#define PIPES_INTERSPERSE_HPP

#include "pipes/helpers/FWD.hpp"

#include "pipes/base.hpp"
#include "pipes/operator.hpp"
#include "pipes/send.hpp"

namespace pipes
{
    template<typename Delimiter>
    class intersperse_pipe : public pipe_base
    {
    public:
        template<typename T, typename TailPipeline>
        void onReceive(T&& value, TailPipeline& tailPipeline)
        {
            if (alreadyReceivedAValue_)
            {
                send(delimiter_, tailPipeline);
            }
            send(FWD(value), tailPipeline);
            alreadyReceivedAValue_ = true;
        }
        
        explicit intersperse_pipe(Delimiter delimiter) : delimiter_(std::move(delimiter)){}
        
    private:
        Delimiter delimiter_;
        bool alreadyReceivedAValue_ = false;
    };
    
    template<typename Delimiter>
    auto intersperse(Delimiter delimiter)
    {
        return intersperse_pipe<Delimiter>{delimiter};
    }
}

#endif /* PIPES_INTERSPERSE_HPP */
