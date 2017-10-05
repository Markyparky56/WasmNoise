#pragma once

namespace type_traits
{
  // integral_constant
  template<class T, T v> struct integral_constant
  {
    static constexpr const T value = v;
    typedef T value_type;
    typedef integral_constant type;
    constexpr operator value_type() const noexcept {return value;}
    constexpr value_type operator()() const noexcept {return value;}
  };
  template<class T, T v> constexpr const T integral_constant<T, v>::value;
  typedef integral_constant<bool, true> true_type;
  typedef integral_constant<bool, false> false_type;

  // bool_constant
  template<bool B> using bool_constant = integral_constant<bool, B>;
  #define BOOL_CONSTANT(_b) bool_constant<(_b)>
  typedef bool_constant<true> true_type;
  typedef bool_constant<false> false_type;

  // is_integral
  template<class T> struct is_integral                    : public false_type {};
  template<>        struct is_integral<bool>              : public true_type {};
  template<>        struct is_integral<char>              : public true_type {};
  template<>        struct is_integral<signed char>       : public true_type {};
  template<>        struct is_integral<unsigned char>     : public true_type {};
  template<>        struct is_integral<wchar_t>           : public true_type {};
  template<>        struct is_integral<short>             : public true_type {};
  template<>        struct is_integral<unsigned short>    : public true_type {};
  template<>        struct is_integral<int>               : public true_type {};
  template<>        struct is_integral<unsigned int>      : public true_type {};
  template<>        struct is_integral<long>              : public true_type {};
  template<>        struct is_integral<unsigned long>     : public true_type {};
  template<>        struct is_integral<long long>         : public true_type {};
  template<>        struct is_integral<unsigned long long> : public true_type {};

  // is_floating_point
  template<class T> struct is_floating_point              : public false_type{};
  template<>        struct is_floating_point<float>       : public true_type{};
  template<>        struct is_floating_point<double>      : public true_type{};
  template<>        struct is_floating_point<long double> : public true_type{};

  // is_arithmetic
  template<class T> struct is_arithmetic 
    : public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};

  // is_signed
  template<class T, bool = is_integral<T>::value>
  struct wasm_is_signed_impl : public BOOL_CONSTANT(T(-1) < T(0)) {};

  template<class T>
  struct wasm_is_signed_impl<T, false> : public true_type{}; // floating point

  template<class T, bool = is_arithmetic<T>::value>
  struct wasm_is_signed : public wasm_is_signed_impl<T> {};

  template<class T> struct wasm_is_signed<T, false> : public false_type {};

  template<class T> struct is_signed : public wasm_is_signed<T> {};
  template<class T> constexpr bool is_signed_v = is_signed<T>::value;

  // remove_const
  template<class T> struct remove_const           {typedef T type;};
  template<class T> struct remove_const<const T>  {typedef T type;};
  template<class T> using remove_const_t = typename remove_const<T>::type;

  // remove_volatile
  template<class T> struct remove_volatile          {typedef T type;};
  template<class T> struct remove_volatile<const T> {typedef T type;};
  template<class T> using remove_volatile_t =typename remove_volatile<T>::type;

  // remove_cv
  template<class T> struct remove_cv 
    {typedef typename remove_volatile<typename remove_const<T>::type>::type type;};
  template<class T> using remove_cv_t = typename remove_cv<T>::type;

  // conditional
  template<bool B, class If, class Then> struct conditional {typedef If type;};
  template<class If, class Then> struct conditional<false, If, Then> {typedef Then type;};
  template<bool B, class If, class Then> using conditional_t = typename conditional<B, If, Then>::type;
}