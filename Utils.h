#ifndef WRAPPER_UTILS_H
#define WRAPPER_UTILS_H
#include <iostream>
#include <type_traits>

#define USING_OPENCV


#ifdef USING_OPENCV

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#endif



namespace wrp
{

template <typename...>
struct Wrapper;


namespace impl
{

struct NullType {};     // Empty class for convenience

template <class>
struct PrintType;       // "Prints" type of variable at compile time.


template <typename T>
inline std::decay_t<T>& lval ()     // Simple function for efficiently returning a reference to a generic variable in
{                                   // the case of default initialization of a wrapper of an reference, like in std::vector
    static T t{};

    return t;
}


template <bool...>
struct And;

template <bool B1, bool... Bs>
struct And<B1, Bs...> : public And<Bs...> {};

template <bool... Bs>
struct And<false, Bs...> : public std::false_type {};

template <>
struct And<true> : public std::true_type {};




template <typename T, typename U>
struct TypePrec
{
    using Type = std::decay_t<decltype(std::declval<std::decay_t<T>>() * std::declval<std::decay_t<U>>())>;
};

template <typename T, typename U, int N>
struct TypePrec<cv::Vec<T, N>, cv::Vec<U, N>>
{
    using Type = typename TypePrec<T, U>::Type;
};

template <typename T, typename U>
using TypePrec_t = typename TypePrec<T, U>::Type;


template <typename>
struct IsWrapper : std::false_type {};

template <typename... Args>
struct IsWrapper<Wrapper<Args...>> : std::true_type {};


#ifdef USING_OPENCV

template <typename>
struct IsVec : std::false_type {};

template <typename T, int N>
struct IsVec<cv::Vec<T, N>> : std::true_type {};

#endif


}   // namespace impl


}   // namespace wrp



#endif // WRAPPER_UTILS_H
