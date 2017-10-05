#pragma once

// integral_constant
template<class T, T v> struct integral_constant;
typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

// bool_constant
using bool_constant = integral_constant<bool, B>;
typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

// is_integral
template<class T> struct is_integral                    : public false_type {};
template<>        struct is_integral<bool>              : public true_type {};
template<>        struct is_integral<char>              : public true_type {};
template<>        struct is_integral<signed char>       : public true_type {};
template<>        struct is_integral<unsigned char>     : public true_type {};
template<>        struct is_integral<whcar_t>           : public true_type {};
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
template<class T, bool is_integral_<T>::value>
struct is_signed_impl : public bool_constant(T(-1) < T(0)) {};

template<class T>
struct is_signed_impl<T, false> : public true_type{} // floating point

template<class T, bool is_arithmetic<T>::value>
struct is_signed : public is_signed_imlp<T> {};

template<class T> struct is_signed<T, false> : public false_type {};

// remove_const
template<class T> struct remove_const           {typedef T type;}
template<class T> struct remove_const<const T>  {typedef T type;}
template<class T> using remove_const_t = typename remove_const<T>::type;

// remove_volatile
template<class T> struct remove_volatile          {typedef T type;}
template<class T> struct remove_volatile<const T> {typedef T type;}
template<class T> using remove_volatile_t =typename remove_volatile<T>::type;

// remove_cv
template<class T> struct remove_cv 
  {typedef typename remove_volatile<typename remove_const<T>::type>::type type;};
template<class T> using remove_cv_t = typename remove_cv<T>::type;

// conditional
template<bool B, class If, class Then> struct conditional {typedef If type;};
template<class If, class Then> struct conditional<false, If, Then> {typedef Then type;};
template<bool B, class If, class Then> using conditional_t = typename constional<B, If, Then>::type;
