#ifndef WRAPPER_H
#define WRAPPER_H
#include "Wrapper_Utils.h"

#define USING_WRAPPER(...)  using Base = __VA_ARGS__;  \
\
                            using Base::Base;    \
                            using Base::operator=;  \
\
                            using Base::t;  \
\
                            using Type     = typename Base::Type;  \
                            using BaseType = typename Base::BaseType;    \



namespace wrp
{


// Specialization for fundamental types. The 'std::enable_if' is only necessary here

namespace impl
{

template <typename T>
struct Wrapper
{
    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    explicit constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    //template <typename U = T>
	//explicit constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}



	//template <typename... Args>
	//explicit constexpr Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}



    template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	explicit constexpr Wrapper(U& u) : t(u) {}

    //template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	//explicit constexpr Wrapper(U&& u) : t(std::move(u)) {}

    template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
    explicit constexpr Wrapper(const U& u) : t(u) {}


	/*constexpr Wrapper(BaseType& u) : t(u) {}

	constexpr Wrapper(BaseType&& u) : t(std::move(u)) {}

    constexpr Wrapper(const BaseType& u) : t(u) {}*/



    template <typename U>
    explicit constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    //template <typename U>
	//explicit constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    explicit constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}



	template <typename U>
	explicit constexpr operator U() { return t; }

    template <typename U>
	explicit constexpr operator U() const { return t; }



    //constexpr operator BaseType&() { return t; }

	//constexpr operator BaseType() const { return t; }



	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = std::move(x); return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    decltype(auto) operator << (const BaseType& u) { return (BaseType)t << u; }

};

}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


// Defined as a variadic template so it can be specialized in any necessary way

template <typename...>
struct Wrapper;


// Main specialization. Allows for instantiations like 'wrp::Wrapper<Type>', where 'Type' may have any qualifier

template <typename T>
struct Wrapper<T> : public Wrapper<T, std::decay_t<T>>
{
    USING_WRAPPER(Wrapper<T, std::decay_t<T>>)
};


//===================================================================================

//GAMBIRA

template <class T, typename B>
struct Wrapper<T, Wrapper<B>> //: public std::conditional_t<std::is_reference<T>::value,
                                                          //Wrapper<std::decay_t<U>, std::decay_t<U>>,
                                                          //Wrapper<std::decay_t<U>,  std::decay_t<U>>>
{
    /*using BaseWrapper = std::conditional_t<std::is_reference<T>::value,
                  Wrapper<std::decay_t<U>, std::decay_t<U>>,
                  Wrapper<std::decay_t<U>,  std::decay_t<U>>>;

    USING_WRAPPER(BaseWrapper);*/

    //=====================================================================

    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    explicit constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    //template <typename U = T>
	//explicit constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}



	//template <typename... Args>
	//explicit constexpr Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}



    template <typename U>
	explicit constexpr Wrapper(U& u) : t(u) {}

    //template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	//explicit constexpr Wrapper(U&& u) : t(std::move(u)) {}

    template <typename U>
    explicit constexpr Wrapper(const U& u) : t(u) {}


	/*constexpr Wrapper(BaseType& u) : t(u) {}

	constexpr Wrapper(BaseType&& u) : t(std::move(u)) {}

    constexpr Wrapper(const BaseType& u) : t(u) {}*/



    /*template <typename U>
    explicit constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    template <typename U>
	explicit constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    explicit constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}*/



	template <typename U>
	explicit constexpr operator U() { return t; }

    template <typename U>
	explicit constexpr operator U() const { return t; }


    /*template <typename U>
	explicit constexpr operator Wrapper<U>() { return Wrapper<U>(t); }

    template <typename U>
	explicit constexpr operator Wrapper<U>() const { return Wrapper<U>(t); }*/



    constexpr operator BaseType&() { return t; }

	constexpr operator BaseType() const { return t; }


    /*constexpr operator Wrapper<BaseType&>() { return Wrapper<BaseType&>(t); }

	constexpr operator Wrapper<BaseType>() const { return Wrapper<BaseType>(t); }*/





	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = std::move(x); return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    /*template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }*/



    decltype(auto) operator << (const BaseType& u) { return (BaseType)t << u; }



//================================================================================



    template <typename U>
    struct OpType { using Type = Wrapper<decltype(std::declval<BaseType>() * std::declval<std::decay_t<U>>())>; };

    template <typename U>
    struct OpType<Wrapper<U>> { using Type = typename OpType<U>::Type; };

    template <typename U>
    using OpType_t = typename OpType<U>::Type;



#define DEFAULT_OP(OP)  \
\
template <typename U>   \
decltype(auto) operator OP ## = (const U& u) { t OP ## = u; return *this; }    \
\
template <typename U>   \
decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this += w.t; }     \
\
template <typename U>   \
friend decltype(auto) operator OP (const Wrapper& w, const Wrapper<U>& z) { OpType_t<U> r(w); r OP ## = z.t; return r; }     \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<U>::value>>   \
friend decltype(auto) operator OP (const Wrapper& w, const U& u) { OpType_t<U> r(w); r OP ## = u; return r; }    \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<U>::value>>   \
friend decltype(auto) operator OP (const U& u, const Wrapper& w) { OpType_t<U> r(w); r OP ## = u; return r; }


    DEFAULT_OP(+);
    DEFAULT_OP(-);
    DEFAULT_OP(*);
    DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP

};

//GAMBIRA

//=================================================================================




template <typename T>
struct Wrapper<T, std::enable_if_t<std::is_fundamental<std::decay_t<T>>::value, std::decay_t<T>>> //: public impl::Wrapper<T>
{
    //USING_WRAPPER(impl::Wrapper<T>)


//=====================================================================

    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    explicit constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    //template <typename U = T>
	//explicit constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}



	//template <typename... Args>
	//explicit constexpr Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}



    template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	explicit constexpr Wrapper(U& u) : t(u) {}

    //template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	//explicit constexpr Wrapper(U&& u) : t(std::move(u)) {}

    template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
    explicit constexpr Wrapper(const U& u) : t(u) {}


	/*constexpr Wrapper(BaseType& u) : t(u) {}

	constexpr Wrapper(BaseType&& u) : t(std::move(u)) {}

    constexpr Wrapper(const BaseType& u) : t(u) {}*/



    template <typename U>
    explicit constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    //template <typename U>
	//explicit constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    explicit constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}



	template <typename U>
	explicit constexpr operator U() { return t; }

    template <typename U>
	explicit constexpr operator U() const { return t; }


    template <typename U>
	explicit constexpr operator Wrapper<U>() { return Wrapper<U>(t); }

    template <typename U>
	explicit constexpr operator Wrapper<U>() const { return Wrapper<U>(t); }



    constexpr operator BaseType&() { return t; }

	constexpr operator BaseType() const { return t; }


    constexpr operator Wrapper<BaseType&>() { return Wrapper<BaseType&>(t); }

	constexpr operator Wrapper<BaseType>() const { return Wrapper<BaseType>(t); }





	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = std::move(x); return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>&& w) { t = w.t; return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    decltype(auto) operator << (const BaseType& u) { return (BaseType)t << u; }


    void swap (Wrapper& w)
    {
        std::cout << "PQP";
        BaseType aux(w.t);
        w.t = t;
        t = aux;
    }



//================================================================================



    template <typename U>
    struct OpType { using Type = Wrapper<decltype(std::declval<BaseType>() * std::declval<std::decay_t<U>>())>; };

    template <typename U>
    struct OpType<Wrapper<U>> { using Type = typename OpType<U>::Type; };

    template <typename U>
    using OpType_t = typename OpType<U>::Type;



#define DEFAULT_OP(OP)  \
\
template <typename U>   \
decltype(auto) operator OP ## = (const U& u) { t OP ## = u; return *this; }    \
\
template <typename U>   \
decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this += w.t; }     \
\
template <typename U>   \
friend decltype(auto) operator OP (const Wrapper& w, const Wrapper<U>& z) { OpType_t<U> r(w); r OP ## = z.t; return r; }     \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<U>::value>>   \
friend decltype(auto) operator OP (const Wrapper& w, const U& u) { OpType_t<U> r(w); r OP ## = u; return r; }    \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<U>::value>>   \
friend decltype(auto) operator OP (const U& u, const Wrapper& w) { OpType_t<U> r(w); r OP ## = u; return r; }


    DEFAULT_OP(+);
    DEFAULT_OP(-);
    DEFAULT_OP(*);
    DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP

};


//========================================================================================



template <typename T, typename V, int N>
struct Wrapper<T, cv::Vec<V, N>> //: public impl::Wrapper<T>
{
    //USING_WRAPPER(impl::Wrapper<T>)

    //=====================================================================

    using Type = T;     // Type may have any qualifier

    using BaseType = std::decay_t<T>;
    //using BaseType = std::remove_reference_t<T>;    // Here 'BaseType' can have const qualifier



	Type t;        // The only storage of the class



    // For non references, the default initialization is called. For references, the default initialization sets
    // the variable 't' to a empty variable of type 'Type'

    template <typename U = T>
    explicit constexpr Wrapper (std::enable_if_t<!std::is_reference<U>::value>* = nullptr) : t{} {}

    //template <typename U = T>
	//explicit constexpr Wrapper (std::enable_if_t<std::is_reference<U>::value>* = nullptr) : t(impl::lval<BaseType>()) {}



	//template <typename... Args>
	//explicit constexpr Wrapper (Args&&... args) : t(std::forward<Args>(args)...) {}



    //template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	//explicit constexpr Wrapper(U& u) : t(u) {}

    //template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
	//explicit constexpr Wrapper(U&& u) : t(std::move(u)) {}

    //template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>
    //explicit constexpr Wrapper(const U& u) : t(u) {}


	constexpr Wrapper(BaseType& u) : t(u) {}

	//constexpr Wrapper(BaseType&& u) : t(std::move(u)) {}

    constexpr Wrapper(const BaseType& u) : t(u) {}



    template <typename U>
    explicit constexpr Wrapper(Wrapper<U>& w) : t(w.t) {}

    //template <typename U>
	//explicit constexpr Wrapper(Wrapper<U>&& w) : t(std::move(w.t)) {}

    template <typename U>
    explicit constexpr Wrapper(const Wrapper<U>& w) : t(w.t) {}



	template <typename U>
	explicit constexpr operator U() { return t; }

    template <typename U>
	explicit constexpr operator U() const { return t; }


    template <typename U>
	explicit constexpr operator Wrapper<U>() { return Wrapper<U>(t); }

    template <typename U>
	explicit constexpr operator Wrapper<U>() const { return Wrapper<U>(t); }



    constexpr operator BaseType&() { return t; }

	constexpr operator BaseType() const { return t; }


    constexpr operator Wrapper<BaseType&>() { return Wrapper<BaseType&>(t); }

	constexpr operator Wrapper<BaseType>() const { return Wrapper<BaseType>(t); }





	decltype(auto) operator = (BaseType& x) { t = x; return *this; }
	decltype(auto) operator = (BaseType&& x) { t = std::move(x); return *this; }
    decltype(auto) operator = (const BaseType& x) { t = x; return *this; }

    template <typename U>
    decltype(auto) operator = (Wrapper<U>& w) { t = w.t; return *this; }

    //template <typename U>
    //decltype(auto) operator = (Wrapper<U>&& w) { t = std::move(w.t); return *this; }

    template <typename U>
    decltype(auto) operator = (const Wrapper<U>& w) { t = w.t; return *this; }



    decltype(auto) operator << (const BaseType& u) { return (BaseType)t << u; }




    decltype(auto) operator [] (int p) { return t[p]; }

    const decltype(auto) operator [] (int p) const { return t[p]; }


//================================================================================








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
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value && !impl::IsVec<std::decay_t<U>>::value>>   \
decltype(auto) operator OP ## = (const U& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u; return *this; }    \
\
template <typename U>   \
decltype(auto) operator OP ## = (const cv::Vec<U, N>& u) { for(int i = 0; i < N; ++i) t[i] OP ## = u[i]; return *this; }   \
\
template <typename U>   \
decltype(auto) operator OP ## = (const Wrapper<U>& w) { return *this OP ## = w.t; }     \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>   \
friend decltype(auto) operator OP (const Wrapper& w, const U& u) { OpType_t<U> r(w); r OP ## = u; return r; }    \
\
template <typename U, typename = std::enable_if_t<!impl::IsWrapper<std::decay_t<U>>::value>>   \
friend decltype(auto) operator OP (const U& u, const Wrapper& w) { OpType_t<U> r(w); r OP ## = u; return r; }    \
\
template <typename U>   \
friend decltype(auto) operator OP (const Wrapper& w, const Wrapper<U>& u) { return w OP u.t; }    \


    DEFAULT_OP(+);
    DEFAULT_OP(-);
    DEFAULT_OP(*);
    DEFAULT_OP(/);


#undef OP_TYPE
#undef DEFAULT_OP



    template <typename U>
    friend double operator * (const Wrapper& w, const cv::Vec<U, N>& u)
    {
        decltype(std::decay_t<V>() * std::decay_t<U>()) r{};

        for(int i = 0; i < N; ++i)
            r += w.t[i] * u[i];

        return r;
    }

    template <typename U>
    friend double operator * (const cv::Vec<U, N>& u, const Wrapper& w) { return w * u; }

};


}   // namespace wrp


#endif // WRAPPER_H
