#ifndef WRAPPER_H
#define WRAPPER_H
#include "Utils.h"

#define USING_WRAPPER(...)  using Base = __VA_ARGS__;  \
\
                            using Base::Base;    \
                            using Base::operator =;  \
\
                            using Base::t;  \
\
                            using Type     = typename Base::Type;  \
                            using BaseType = typename Base::BaseType;    \



namespace wrp
{

// Defined as a variadic template, so it can be specialized in any necessary way

template <typename...>
struct Wrapper;



namespace impl
{

// Base class for inheritance

template <typename T>
struct Wrapper
{
    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // Default constructor only allowed if Type is not a reference

    template <typename U = T, typename = std::enable_if_t<!std::is_reference<U>::value>>
    constexpr Wrapper () : t{} {}


    // Delegating constructor only if Type is not a reference

	template <typename... Args, typename U = T, typename = std::enable_if_t<!std::is_reference<U>::value>>
	constexpr Wrapper (Args&&... args) : t{std::forward<Args>(args)...} {}


    constexpr Wrapper (BaseType& b) : t{b} {}

    constexpr Wrapper (BaseType&& b) : t{std::move(b)} {}

    constexpr Wrapper (const BaseType& b) : t{b} {}



    template <typename... Args>
    constexpr Wrapper (::wrp::Wrapper<Args...>& w) : Wrapper(w.t) {}

    template <typename... Args>
    constexpr Wrapper (::wrp::Wrapper<Args...>&& w) : Wrapper(std::move(w.t)) {}

    template <typename... Args>
    constexpr Wrapper (const ::wrp::Wrapper<Args...>& w) : Wrapper(w.t) {}



    /*constexpr operator BaseType& () { return t; }

    constexpr operator BaseType&& () { return t; }

    constexpr operator const BaseType& () { return t; }*/

    constexpr operator Type () { return t; }



    template <typename U>
    explicit constexpr operator U () { return U(t); }




    decltype(auto) operator = (BaseType& b) { t = b; return *this; }

    decltype(auto) operator = (BaseType&& b) { t = std::move(b); return *this; }

    decltype(auto) operator = (const BaseType& b) { t = b; return *this; }


    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    friend decltype(auto) operator << (std::ostream& out, const Wrapper& w) { return out << w.t; }


    template <typename U>
    decltype(auto) operator [] (U&& u) { return t[std::forward<U>(u)]; }

    template <typename U>
    const decltype(auto) operator [] (U&& u) const { return t[std::forward<U>(u)]; }

};

}

//===============================================================================================



// Main specialization. Allows for instantiations like 'wrp::Wrapper<Type>', where 'Type' may have any qualifier

template <typename T>
struct Wrapper<T> : public Wrapper<T, std::decay_t<T>>
{
    USING_WRAPPER(Wrapper<T, std::decay_t<T>>)
};


//========================================================================================


// Specialization for fundamental types. The 'std::enable_if' is only necessary here

template <typename T>
struct Wrapper<T, std::enable_if_t<std::is_fundamental<std::decay_t<T>>::value, std::decay_t<T>>> : public impl::Wrapper<T>
{
    USING_WRAPPER(impl::Wrapper<T>)
};




//========================================================================================

// std::array

template <typename T, typename V, std::size_t N>
struct Wrapper<T, std::array<V, N>> : public impl::Wrapper<T>
{
    USING_WRAPPER(impl::Wrapper<T>)


    template <typename U>
	constexpr Wrapper (const std::array<U, N>& u) { std::copy(u.begin(), u.end(), t.begin()); }




    template <typename>
    struct IsArray : std::false_type {};

    template <typename U, std::size_t M>
    struct IsArray<std::array<U, M>> : std::true_type {};

    template <class U>
    static constexpr bool isArray = IsArray<U>::value;



    template <typename U>
    struct OpType { using Type = Wrapper<std::array<decltype(std::declval<std::decay_t<V>>() * std::declval<std::decay_t<U>>()), N>>; };

    template <typename U>
    struct OpType<Wrapper<U>> { using Type = typename OpType<U>::Type; };

    template <typename U>
    struct OpType<std::array<U, N>> { using Type = typename OpType<U>::Type; };

    template <typename U>
    using OpType_t = typename OpType<U>::Type;



#define DEFAULT_OP(OP)  \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const U& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u; return *this; }    \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const std::array<U, N>& v) { for(int i = 0; i < N; ++i) t[i] OP ## = v[i]; return *this; }   \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this OP ## = w.t; }     \
\
    template <typename U>   \
    decltype(auto) operator OP (const U& u) const { OpType_t<U> r(t); r OP ## = u; return r; }    \
\
    template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>   \
    friend decltype(auto) operator OP (const U& u, const Wrapper& w) { return w * u; }


DEFAULT_OP(+);
DEFAULT_OP(-);
DEFAULT_OP(*);
DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP


    template <typename U>
    double operator * (const std::array<U, N>& u) const
    {
        decltype(std::decay_t<V>() * std::decay_t<U>()) r{};

        for(int i = 0; i < N; ++i)
            r += t[i] * u[i];

        return r;
    }

    template <typename U, typename = std::enable_if_t<isArray<std::decay_t<U>>>>
    double operator * (const Wrapper<U>& w) const { return *this * w.t; }

};




//========================================================================================

// cv::Vec

//#define USING_OPENCV
#ifdef USING_OPENCV

template <typename T, typename V, int N>
struct Wrapper<T, cv::Vec<V, N>> : public impl::Wrapper<T>
{
    USING_WRAPPER(impl::Wrapper<T>)



    template <typename U>
    struct OpType { using Type = Wrapper<cv::Vec<decltype(std::declval<std::decay_t<V>>() * std::declval<std::decay_t<U>>()), N>>; };

    template <typename U>
    struct OpType<Wrapper<U>> { using Type = typename OpType<U>::Type; };

    template <typename U>
    struct OpType<cv::Vec<U, N>> { using Type = typename OpType<U>::Type; };

    template <typename U>
    using OpType_t = typename OpType<U>::Type;



#define DEFAULT_OP(OP)  \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const U& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u; return *this; }    \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const cv::Vec<U, N>& v) { for(int i = 0; i < N; ++i) t[i] OP ## = v[i]; return *this; }   \
\
    template <typename U>   \
    decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this OP ## = w.t; }     \
\
    template <typename U>   \
    decltype(auto) operator OP (const U& u) const { OpType_t<U> r(t); r OP ## = u; return r; }    \
\
    template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>   \
    friend decltype(auto) operator OP (const U& u, const Wrapper& w) { return w * u; }


DEFAULT_OP(+);
DEFAULT_OP(-);
DEFAULT_OP(*);
DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP


    template <typename U>
    double operator * (const cv::Vec<U, N>& u) const
    {
        decltype(std::decay_t<V>() * std::decay_t<U>()) r{};

        for(int i = 0; i < N; ++i)
            r += t[i] * u[i];

        return r;
    }

    template <typename U, typename = std::enable_if_t<impl::IsVec<std::decay_t<U>>::value>>
    double operator * (const Wrapper<U>& w) const { return *this * w.t; }

};

#endif  // USING_OPENCV


}   // namespace wrp


#endif // WRAPPER_H
