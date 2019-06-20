#ifndef PIPES_CRTP_HPP
#define PIPES_CRTP_HPP

namespace pipes
{
namespace detail
{
    
template <typename T, template<typename> class crtpType>
struct crtp
{
    T& derived() { return static_cast<T&>(*this); }
    T const& derived() const { return static_cast<T const&>(*this); }
private:
    crtp(){}
    friend crtpType<T>;
};

}
}


#endif /* PIPES_CRTP_HPP */
