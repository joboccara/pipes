#ifndef PIPES_FLATTEN_HPP
#define PIPES_FLATTEN_HPP

#include "pipes/operator.hpp"

#include "pipes/pipeline_base.hpp"
#include "pipes/helpers/FWD.hpp"
#include "pipes/helpers/warnings.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{
    template<typename TailPipeline>
    class flatten_pipeline : public pipeline_base<flatten_pipeline<TailPipeline>>
    {
    public:
        template<typename T>
        void onReceive(T&& inputCollection)
        {
            std::copy(begin(inputCollection), end(inputCollection), tailPipeline_);
        }
        
        explicit flatten_pipeline(TailPipeline tailPipeline) : tailPipeline_(tailPipeline) {}
        
    private:
        TailPipeline tailPipeline_;
        
    public: // but technical
        using base = pipeline_base<flatten_pipeline<TailPipeline>>;
        using base::operator=;
        flatten_pipeline& operator=(flatten_pipeline const& other)
        {
            tailPipeline_ = other.tailPipeline_;
            return *this;
        }
        flatten_pipeline& operator=(flatten_pipeline& other) { *this = const_cast<flatten_pipeline const&>(other); return *this; }
    };
    
    struct flatten_pipe
    {
        template<typename Pipeline>
        auto plug_to_pipeline(Pipeline&& pipeline) const
        {
            return flatten_pipeline<std::decay_t<Pipeline>>{pipeline};
        }
    };
    
    static auto constexpr flatten = flatten_pipe{};
} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_FLATTEN_HPP */
