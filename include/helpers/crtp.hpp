#ifndef crtp_hpp
#define crtp_hpp

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


#endif /* crtp_hpp */
