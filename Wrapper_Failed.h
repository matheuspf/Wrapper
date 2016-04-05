#ifndef WRAPPER_H
#define WRAPPER_H
#include "Wrapper_Utils.h"


template <typename T>
struct Operations
{
    /*template <typename U>
    friend decltype(auto) operator+ (const Wrapper<T>& w, const Wrapper<U>& z) { return w.t + z.t; }

    template <typename U>
    friend decltype(auto) operator- (const Wrapper<T>& w, const Wrapper<U>& z) { return w.t - z.t; }

    template <typename U>
    friend decltype(auto) operator* (const Wrapper<T>& w, const Wrapper<U>& z) { return w.t * z.t; }*/
};

//------------------------------------------------------------------


template <typename T, int N>
struct Operations<cv::Vec<T, N>>
{
    template <typename U>//, typename = std::enable_if_t<T::channels == U::channels && impl::IsVec<T>::value && impl::IsVec<U>::value>>
    friend decltype(auto) operator+ (Wrapper<T>& v, const Wrapper<U>& u)
    {
        impl::TypePrec_t<std::decay_t<T>, std::decay_t<U>> r;

        for(int i = 0; i < N; ++i)
            r = v[i] + u[i];

        return r;
    }

    template <typename U>//, typename = std::enable_if_t<T::channels == U::channels && impl::IsVec<T>::value && impl::IsVec<U>::value>>
    friend decltype(auto) operator- (Wrapper<T>& v, const Wrapper<U>& u)
    {
        impl::TypePrec_t<std::decay_t<T>, std::decay_t<U>> r;

        for(int i = 0; i < N; ++i)
            r = v[i] - u[i];

        return r;
    }

    template <typename V = cv::Vec<T, N>, typename U = cv::Vec<T, N>>//, typename = std::enable_if_t<T::channels == U::channels && impl::IsVec<T>::value && impl::IsVec<U>::value>>
    friend decltype(auto) operator* (const Wrapper<V>& v, const Wrapper<U>& u)
    {
        double r{};
        //impl::TypePrec_t<typename T::value_type, typename U::value_type> r{};

        for(int i = 0; i < N; ++i)
            r += v[i] * u[i];

        return r;
    }

    /*template <typename U>//, typename = std::enable_if_t<impl::IsVec<T>::value && impl::IsVec<U>::value>>
    friend decltype(auto) operator* (Wrapper<T>& v, const U& u)
    {
        impl::TypePrec_t<std::decay_t<T>, std::decay_t<U>> r;

        for(int i = 0; i < N; ++i)
            r[i] = v[i] * u;

        return r;
    }*/
};


//===============================================================================


namespace wrp
{


template <typename T>
struct Wrapper : Operations<std::decay_t<T>>
{
    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    template <typename U = T>
	constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}



	constexpr Wrapper(BaseType& t) : t(t) {}
	constexpr Wrapper(BaseType&& t) : t(std::move(t)) {}
    constexpr Wrapper(const BaseType& t) : t(t) {}

    template <typename U>
    constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    template <typename U>
	constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}


    //constexpr operator Type& () { return t; }
	constexpr operator Type () const { return t; }


	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = x; return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }




    decltype(auto) operator[] (int p) { return at(p); }

    template <typename U = T>
    decltype(auto) at (int, std::enable_if_t<std::is_fundamental<U>::value>* = nullptr) { return t; }

    template <typename U = T>
    decltype(auto) at (int p, std::enable_if_t<!std::is_fundamental<U>::value>* = nullptr) { return t[p]; }


    decltype(auto) operator[] (int p) const { return at(p); }

    template <typename U = T>
    const BaseType& at (int, std::enable_if_t<std::is_fundamental<U>::value>* = nullptr) const { return t; }

    template <typename U = T>
    decltype(auto) at (int p, std::enable_if_t<!std::is_fundamental<U>::value>* = nullptr) const { return t[p]; }




    decltype(auto) operator << (const BaseType& u) const { return (BaseType)t << u; }



//--------------------------------------------------------------------------------------------------


    template <typename U>
    struct SumType { using Type = Wrapper<decltype(std::declval<BaseType>() + std::declval<std::decay_t<U>>())>; };

    template <typename U>
    struct SumType<Wrapper<U>> { using Type = Wrapper<decltype(std::declval<BaseType>() + std::declval<std::decay_t<U>>())>; };

    template <typename U>
    using SumType_t = typename SumType<U>::Type;



    /*template <typename U>
    decltype(auto) operator += (const U& u) { t = t + u; return *this; }

    template <typename U>
    decltype(auto) operator += (const Wrapper<U>& w) { t = t + w.t; return *this; }


    template <typename U>
    friend SumType_t<U> operator + (const Wrapper& w, const Wrapper<U>& z) { return SumType_t<U>(w.t + z.t); }

    template <typename U>
    friend SumType_t<U> operator + (const Wrapper& w, const U& u) { return SumType_t<U>(w.t + u); }

    template <typename U>
    friend SumType_t<U> operator + (const BaseType& t, const Wrapper<U>& w) { return SumType_t<U>(t + w.t); }*/


//--------------------------------------------------------------------------------------------------------

    /*template <typename U>
    decltype(auto) operator -= (const U& u) { t = t - u; return *this; }

    template <typename U>
    decltype(auto) operator -= (const Wrapper<U>& w) { t = t - w.t; return *this; }


    template <typename U>
    friend SumType_t<U> operator - (const Wrapper& w, const Wrapper<U>& z) { return SumType_t<U>(w.t - z.t); }

    template <typename U>
    friend SumType_t<U> operator - (const Wrapper& w, const U& u) { return SumType_t<U>(w.t - u); }

    template <typename U>
    friend SumType_t<U> operator - (const BaseType& t, const Wrapper<U>& w) { return SumType_t<U>(t - w.t); }*/


//--------------------------------------------------------------------------------------------------



    //template <typename U>
    //struct MultType { using Type = Wrapper<decltype(std::declval<BaseType>() * std::declval<std::decay_t<U>>())>; };

    //template <typename U>
    //struct MultType<Wrapper<U>> { using Type = Wrapper<decltype(std::declval<BaseType>() * std::declval<std::decay_t<U>>())>; };

    template <typename U>
    struct MultType { using Type = std::decay_t<decltype(Wrapper<BaseType>() * Wrapper<std::decay_t<U>>())>; };

    template <typename U>
    using MultType_t = typename MultType<U>::Type;



    /*template <typename U>
	decltype(auto) operator *= (const U& u) { t = t * u; return *this; }

	template <typename U>
	decltype(auto) operator *= (const Wrapper<U>& w) { t = t * w.t; return *this; }


    template <typename U>
    friend MultType_t<U> operator * (const Wrapper& w, const Wrapper<U>& z) { return MultType_t<U>(w.t * z.t); }

    template <typename U>
    friend MultType_t<U> operator * (const Wrapper& w, const U& u) { return MultType_t<U>(w.t * u); }

    template <typename U>
    friend MultType_t<U> operator * (const BaseType& t, const Wrapper<U>& w) { return MultType_t<U>(t * w.t); }*/

    //template <typename V, typename U>
    //friend Wrapper<V> operator* (const Wrapper<V>& w, const Wrapper<U>& z);


};

//template <typename V, typename U>
//std::decay_t<decltype(Wrapper<std::decay_t<V>>() * Wrapper<std::decay_t<U>>())> operator* (const Wrapper<V>& w, const Wrapper<U>& z) { Wrapper<V>::MultType_t<U> r(w); r *= z; return r; }

}   // namespace wrp


#endif // WRAPPER_H
