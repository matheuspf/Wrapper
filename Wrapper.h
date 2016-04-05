#ifndef WRAPPER_H
#define WRAPPER_H
#include "Wrapper_Utils.h"


namespace wrp
{

// Defined as a variadic template so it can be specialized in any necessary way

template <typename...>
struct Wrapper;


// Main specialization. Allows for instantiations like 'wrp::Wrapper<Type>', where 'Type' may have any qualifier

template <typename T>
struct Wrapper<T> : public Wrapper<T, std::decay_t<T>>
{
    using Wrapper<T, std::decay_t<T>>::Wrapper;
};



// Specialization for fundamental types. The 'std::enable_if' is only necessary here

template <typename T>
struct Wrapper<T, std::enable_if_t<std::is_fundamental<std::decay_t<T>>::value, std::decay_t<T>>>
{
    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    explicit constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    template <typename U = T>
	explicit constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}


    template <typename U>
	explicit constexpr Wrapper(U& u) : t(u) {}

    template <typename U>
	explicit constexpr Wrapper(U&& u) : t(std::move(u)) {}

    template <typename U>
    explicit constexpr Wrapper(const U& u) : t(u) {}


    template <typename U>
    explicit constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    template <typename U>
	explicit constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    explicit constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}



	template <typename U>
	explicit constexpr operator U() { return t; }

    template <typename U>
	explicit constexpr operator U() const { return t; }



	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = std::move(x); return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    decltype(auto) operator[] (int) { return t; }

    decltype(auto) operator[] (int) const { return t; }


    decltype(auto) operator << (const BaseType& u) { return (BaseType)t << u; }



//--------------------------------------------------------------------------------------------------



#define OP_TYPE(OP, T) Wrapper<decltype(std::declval<BaseType>() OP std::declval<std::decay_t<T>>())>


/*#define DEFAULT_OP(OP)  \
\
template <typename U>   \
decltype(auto) operator OP ## = (const U& u) { t OP ## = u; return *this; }    \
\
template <typename U>   \
decltype(auto) operator OP ## = (const Wrapper<U>& w) { t OP ## = w.t; return *this; }     \
\
template <typename U>   \
friend OP_TYPE(OP, U) operator OP (const Wrapper& w, const U& u) { OP_TYPE(OP, U) r(w); r OP ## = u; return r; }    \
\
template <typename U>   \
friend OP_TYPE(OP, U) operator OP (const U& u, const Wrapper& w) { OP_TYPE(OP, U) r(w); r OP ## = u; return r; }*/

#define DEFAULT_OP(OP)  \
\
template <typename U>   \
decltype(auto) operator OP ## = (const U& u) { t OP ## = u; return *this; }    \
\
template <typename U>   \
decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this += w.t; }     \
\
template <typename U>   \
friend OP_TYPE(OP, U) operator OP (const Wrapper& w, const Wrapper<U>& z) { OP_TYPE(OP, U) r(w); r OP ## = z.t; return r; }     \
\
template <typename U>   \
friend OP_TYPE(OP, U) operator OP (const Wrapper& w, const U& u) { OP_TYPE(OP, U) r(w); r OP ## = u; return r; }    \
\
template <typename U>   \
friend OP_TYPE(OP, U) operator OP (const U& u, const Wrapper& w) { OP_TYPE(OP, U) r(w); r OP ## = u; return r; }


    DEFAULT_OP(+);
    DEFAULT_OP(-);
    DEFAULT_OP(*);
    DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP


    /*template <typename U>
	decltype(auto) operator *= (const U& u) { t *= u; return *this; }

	template <typename U>
	decltype(auto) operator *= (const Wrapper<U>& w) { t *= w.t; return *this; }


    template <typename U>
    friend MultType_t<U> operator * (const Wrapper& w, const U& u) { MultType_t<U> r(w); r *= u; return r; }

    template <typename U>
    friend MultType_t<U> operator * (const U& u, const Wrapper& w) { MultType_t<U> r(w); r *= u; return r; }*/

};



//=============================================================================================================


template <typename T, typename V, int N>
struct Wrapper<T, cv::Vec<V, N>>
{
    using Type = T;     // Type may have any qualifier

    using BaseType = cv::Vec<V, N>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    explicit constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    template <typename U = T>
	explicit constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}


    template <typename U>
	explicit constexpr Wrapper(U& u) : t(u) {}

    template <typename U>
	explicit constexpr Wrapper(U&& u) : t(std::move(u)) {}

    template <typename U>
    explicit constexpr Wrapper(const U& u) : t(u) {}


    template <typename U>
    explicit constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    template <typename U>
	explicit constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    explicit constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}



	template <typename U>
	explicit constexpr operator U() { return t; }

    template <typename U>
	explicit constexpr operator U() const { return t; }



	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = std::move(x); return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    decltype(auto) operator[] (int p) { return t[p]; }

    decltype(auto) operator[] (int p) const { return t[p]; }


    decltype(auto) operator << (const BaseType& u) { return (BaseType)t << u; }



//=========================================================================================

#define OP_TYPE(OP, T) Wrapper<cv::Vec<decltype(std::declval<V>() OP std::declval<std::decay_t<T>>()), N>>


#define DEFAULT_OP(OP)  \
\
template <typename U>   \
decltype(auto) operator OP ## = (const U& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u; return *this; }    \
\
\
template <typename U>   \
decltype(auto) operator OP ## = (const cv::Vec<U, N>& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u[i]; return *this; }   \
\
template <typename U>   \
decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this OP ## = w.t; }     \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<U>::value>>   \
friend Wrapper<cv::Vec<double, N>> operator OP (const Wrapper& w, const U& u) { Wrapper<cv::Vec<double, N>> r(w); r OP ## = u; return r; }    \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<U>::value>>   \
friend Wrapper<cv::Vec<double, N>> operator OP (const U& u, const Wrapper& w) { Wrapper<cv::Vec<double, N>> r(w); r OP ## = u; return r; }    \
\
template <typename U>   \
friend decltype(auto) operator OP (const Wrapper& w, const Wrapper<U>& u) { return w OP u.t; }    \


    DEFAULT_OP(+);
    DEFAULT_OP(-);
    DEFAULT_OP(*);



    template <typename U>
    friend double operator * (const Wrapper& w, const cv::Vec<U, N>& u)
    {
        //decltype(std::decay_t<V>() * std::decay_t<U>()) r{};
        double r{};

        for(int i = 0; i < N; ++i)
            r += w.t[i] * u[i];

        return r;
    }

    template <typename U>
    friend double operator * (const cv::Vec<U, N>& u, const Wrapper& w) { return w * u; }


#undef OP_TYPE
#undef DEFAULT_OP

};


}   // namespace wrp


#endif // WRAPPER_H
