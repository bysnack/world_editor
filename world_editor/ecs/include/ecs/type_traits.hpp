#pragma once

#include <functional>
#include <type_traits>

namespace meta {

template <template <class...> class Class, class T>
inline constexpr bool is_v = false;

template <template <class...> class Class, class... Us>
inline constexpr bool is_v<Class, Class<Us...>> = true;

template <template <class...> class Class, class T>
using is_t = std::integral_constant<bool, is_v<Class, T>>;

template <class... T> struct ts {};

template <class Functor>
struct functor_signature
    : functor_signature<decltype(&std::decay_t<Functor>::operator())> {};

template <typename ret_t, typename class_t, typename... args_t>
struct functor_signature<ret_t (class_t::*)(args_t...)> {
  using type = ts<ret_t, args_t...>;
};

template <typename ret_t, typename class_t, typename... args_t>
struct functor_signature<ret_t (class_t::*)(args_t...) const>
    : functor_signature<ret_t (class_t::*)(args_t...)> {};

template <typename ret_t, typename class_t, typename... args_t>
struct functor_signature<ret_t (class_t::*)(args_t...) const noexcept>
    : functor_signature<ret_t (class_t::*)(args_t...)> {};

template <typename T>
struct function_traits : public functor_signature<decltype(&T::operator())> {};

template <typename ret_t, typename class_t, typename... args_t>
struct function_traits<ret_t (class_t::*)(args_t...)> {
  using type = ts<ret_t, class_t, args_t...>;
};

template <typename ret_t, typename class_t, typename... args_t>
struct function_traits<ret_t (class_t::*)(args_t...) const>
    : function_traits<ret_t (class_t::*)(args_t...)> {};

template <typename ret_t, typename class_t, typename... args_t>
struct function_traits<ret_t (class_t::*)(args_t...) const noexcept>
    : function_traits<ret_t (class_t::*)(args_t...)> {};

template <typename ret_t, typename... args_t>
struct function_traits<ret_t (*)(args_t...)> {
  using type = ts<ret_t, args_t...>;
};

template <typename ret_t, typename... args_t>
struct function_traits<ret_t(args_t...)>
    : function_traits<ret_t (*)(args_t...)> {};

template <typename ret_t, typename... args_t>
struct function_traits<ret_t (&)(args_t...)>
    : function_traits<ret_t (*)(args_t...)> {};

// Traits for std::function objects
template <typename T>
struct function_traits<std::function<T>> : function_traits<T> {};

template <class T> using function_traits_t = typename function_traits<T>::type;

template <int N, class List> struct at;

template <int N, template <class...> class List, class T, class... Ts>
struct at<N, List<T, Ts...>> : at<N - 1, List<Ts...>> {};

template <template <class...> class List, class T, class... Ts>
struct at<0, List<T, Ts...>> {
  using type = T;
};

template <int N, class List> using at_t = typename at<N, List>::type;

} // namespace meta
