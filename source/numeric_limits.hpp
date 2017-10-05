#pragma once
#include "type_traits.hpp"

#define FORCE_INLINE __attribute__((always_inline))

enum float_round_style
{
  round_indeterminate = -1,
  round_toward_zero = 0,
  round_to_nearest = 1,
  round_toward_infinity = 2,
  round_toward_neg_infinity = 3
};

enum float_denorm_style
{
  denorm_indeterminate = -1,
  denorm_absent = 0,
  denorm_present = 1
};

// wasm_numeric_limits base tamplte
template<class T, bool = type_traits::is_arithmetic<T>::value>
class wasm_numeric_limits
{
protected:
  typedef T type;

  static constexpr bool is_specialized = false;
  static constexpr T min() noexcept {return type();}
  static constexpr T max() noexcept {return type();}
  static constexpr T lowest() noexcept {return type();}

  static constexpr int digits = 0;
  static constexpr int digits10 = 0;
  static constexpr int max_digits10 = 0;
  static constexpr bool is_signed = false;
  static constexpr bool is_integer = false;
  static constexpr bool is_exact = false;
  static constexpr bool radix = 0;
  static constexpr T epsilon() noexcept {return type();}
  static constexpr T round_error() noexcept {return type();}

  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;

  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr bool has_denorm_loss = false;
  static constexpr T infinity() noexcept {return type();}
  static constexpr T quiet_NaN() noexcept { return type();}
  static constexpr T signaling_NaN() noexcept {return type();}
  static constexpr T denorm_min() noexcept {return type();}

  static constexpr bool is_iec559 = false;
  static constexpr bool is_bounded = false;
  static constexpr bool is_modulo = false;

  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;
  static constexpr float_round_style round_style = round_toward_zero;
};

template<class T, int __digits, bool _IsSigned>
struct compute_min
{
  static constexpr const T value = static_cast<T>(static_cast<T>(1) << __digits);
};

template<class T, int __digits>
struct compute_min<T, __digits, false>
{
  static constexpr const T value = static_cast<T>(0);
};

template<class T>
class wasm_numeric_limits<T, true>
{
protected:
  typedef T type;

  static constexpr const bool is_specialized = true;

  static constexpr const bool is_signed = type(-1) < type(0);
  static constexpr const int digits = static_cast<int>(sizeof(type) * __CHAR_BIT__ - is_signed);
  static constexpr const int digits10 = digits * 3 / 10;
  static constexpr const int max_digits10 = 0;
  static constexpr const type __min = compute_min<type, digits, is_signed>::value;
  static constexpr const type __max = is_signed ? type(type(~0) ^ __min) : type(~0);
  FORCE_INLINE static constexpr type min() noexcept {return __min;}
  FORCE_INLINE static constexpr type max() noexcept {return __max;}
  FORCE_INLINE static constexpr type lowest() noexcept {return min();}

  static constexpr const bool is_integer = true;
  static constexpr const bool is_exact = true;
  static constexpr const int radix = 2;
  FORCE_INLINE static constexpr type epsilon() noexcept {return type(0);}
  FORCE_INLINE static constexpr type round_error() noexcept {return type(0);}

  static constexpr const int min_exponent = 0;
  static constexpr const int min_exponent10 = 0;
  static constexpr const int max_exponent = 0;
  static constexpr const int max_exponent10 = 0;

  static constexpr const bool has_infinity = false;
  static constexpr const bool has_quiet_NaN = false;
  static constexpr const bool has_signaling_NaN = false;
  static constexpr const float_denorm_style has_denorm = denorm_absent;
  static constexpr const bool has_denorm_loss = false;
  FORCE_INLINE static constexpr type infinit() noexcept {return type(0);}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return type(0);}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return type(0);}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return type(0);}

  static constexpr const bool is_iec559 = false;
  static constexpr const bool is_bounded = true;
  static constexpr const bool is_modulo = !type_traits::is_signed<T>::value;
  // Since this header is explicitly intended for WebAssembly
  static constexpr const bool traps = true;
  static constexpr const bool tinyness_before = false;
  static constexpr const float_round_style round_style = round_toward_zero;
};

template<>
class wasm_numeric_limits<bool, true>
{
protected:
  typedef bool type;

  static constexpr const bool is_specialized = true;

  static constexpr const bool is_signed = false;
  static constexpr const int digits = 1;
  static constexpr const int digits10 = 0;
  static constexpr const int max_digits10 = 0;
  static constexpr const type __min = false;
  static constexpr const type __max = true;
  FORCE_INLINE static constexpr type min() noexcept {return __min;}
  FORCE_INLINE static constexpr type max() noexcept {return __max;}
  FORCE_INLINE static constexpr type lowest() noexcept {return min();}

  static constexpr const bool is_integer = true;
  static constexpr const bool is_exact = true;
  static constexpr const int radix = 2;
  FORCE_INLINE static constexpr type epsilon() noexcept {return type(0);}
  FORCE_INLINE static constexpr type round_Error() noexcept {return type(0);}

  static constexpr const int min_exponent = 0;
  static constexpr const int min_exponent10 = 0;
  static constexpr const int max_exponent = 0;
  static constexpr const int max_exponent10 = 0;

  static constexpr const bool has_infinity = false;
  static constexpr const bool has_quiet_NaN = false;
  static constexpr const bool has_signaling_NaN = false;
  static constexpr const float_denorm_style has_Denorm = denorm_absent;
  static constexpr const bool has_denorm_loss = false;
  FORCE_INLINE static constexpr type infinity() noexcept {return type(0);}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return type(0);}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return type(0);}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return type(0);}

  static constexpr const bool is_iec559 = false;
  static constexpr const bool is_bounded = true;
  static constexpr const bool is_modulo = false;

  static constexpr const bool traps = false;
  static constexpr const bool tinyness_before = false;
  static constexpr const float_round_style round_style = round_toward_zero;
};

template<>
class wasm_numeric_limits<float, true>
{
protected:
  typedef float type;

  static constexpr const bool is_specialized = true;

  static constexpr const bool is_signed = true;
  static constexpr const int digits = __FLT_MANT_DIG__;
  static constexpr const int digits10 = __FLT_DIG__;
  static constexpr const int max_digits10 = 2+(digits * 20103l)/100000l;
  FORCE_INLINE static constexpr type min() noexcept {return __FLT_MIN__;}
  FORCE_INLINE static constexpr type max() noexcept {return __FLT_MAX__;}
  FORCE_INLINE static constexpr type lowest() noexcept {return -max();}

  static constexpr const bool is_integer = false;
  static constexpr const bool is_exact = false;
  static constexpr const int radix = __FLT_RADIX__;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __FLT_EPSILON__;}
  FORCE_INLINE static constexpr type round_Error() noexcept {return 0.5f;}

  static constexpr const int min_exponent = __FLT_MIN_EXP__;
  static constexpr const int min_exponent10 = __FLT_MIN_10_EXP__;
  static constexpr const int max_exponent = __FLT_MAX_EXP__;
  static constexpr const int max_exponent10 = __FLT_MAX_10_EXP__;

  static constexpr const bool has_infinity = true;
  static constexpr const bool has_quiet_NaN = true;
  static constexpr const bool has_signaling_NaN = true;
  static constexpr const float_denorm_style has_denorm = denorm_present;
  static constexpr const bool has_denorm_loss = false;
  FORCE_INLINE static constexpr type infinity() noexcept {return __builtin_huge_valf();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __builtin_nanf("");}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __builtin_nansf("");}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __FLT_DENORM_MIN__;}

  static constexpr const bool is_iec559 = true;
  static constexpr const bool is_bounded = true;
  static constexpr const bool is_modulo = false;

  static constexpr const bool traps = false;
  static constexpr const bool tinyness_before = false;
  static constexpr const float_round_style round_style = round_to_nearest;
};

template<>
class wasm_numeric_limits<double, true>
{
protected:
  typedef double type;

  static constexpr const bool is_specialized = true;

  static constexpr const bool is_signed = true;
  static constexpr const int digits = __DBL_MANT_DIG__;
  static constexpr const int digits10 = __DBL_DIG__;
  static constexpr const int max_digits10 = 2+(digits * 30103l)/100000l;
  FORCE_INLINE static constexpr type min() noexcept {return __DBL_MIN__;}
  FORCE_INLINE static constexpr type max() noexcept {return __DBL_MAX__;}
  FORCE_INLINE static constexpr type lowest() noexcept {return -max();}

  static constexpr const bool is_integer = false;
  static constexpr const bool is_exact = false;
  static constexpr const int radix = __FLT_RADIX__;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __DBL_EPSILON__;}
  FORCE_INLINE static constexpr type round_error() noexcept {return 0.5;}

  static constexpr const int min_exponent = __DBL_MIN_EXP__;
  static constexpr const int min_exponent10 = __DBL_MIN_10_EXP__;
  static constexpr const int max_exponent = __DBL_MAX_EXP__;
  static constexpr const int max_exponent10 = __DBL_MAX_10_EXP__;

  static constexpr const bool has_infinity = true;
  static constexpr const bool has_quiet_NaN = true;
  static constexpr const bool has_signaling_NaN = true;
  static constexpr const float_denorm_style has_denorm = denorm_present;
  static constexpr const bool  has_denorm_loss = false;
  FORCE_INLINE static constexpr type infinity() noexcept {return __builtin_huge_val();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __builtin_nan("");}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __builtin_nans("");}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __DBL_DENORM_MIN__;}

  static constexpr const bool is_iec559 = true;
  static constexpr const bool is_bounded = true;
  static constexpr const bool is_modulo = false;
  
  static constexpr const bool traps = false;
  static constexpr const bool tinyness_before = false;
  static constexpr const float_round_style round_style = round_to_nearest;
};

template<>
class wasm_numeric_limits<long double, true>
{
protected:
  typedef long double type;

  static constexpr const bool is_specialized = true;

  static constexpr const bool is_signed = true;
  static constexpr const int digits = __LDBL_MANT_DIG__;
  static constexpr const int digits10 = __LDBL_DIG__;
  static constexpr const int max_digits10 = 2+(digits * 30103l)/100000l;
  FORCE_INLINE static constexpr type min() noexcept {return __LDBL_MIN__;}
  FORCE_INLINE static constexpr type max() noexcept {return __LDBL_MAX__;}
  FORCE_INLINE static constexpr type lowest() noexcept {return -max();}

  static constexpr const bool is_integer = false;
  static constexpr const bool is_exact = false;
  static constexpr const int radix = __FLT_RADIX__;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __LDBL_EPSILON__;}
  FORCE_INLINE static constexpr type round_Error() noexcept {return 0.5f;}

  static constexpr const int min_exponent = __LDBL_MIN_EXP__;
  static constexpr const int min_exponent10 = __LDBL_MIN_10_EXP__;
  static constexpr const int max_exponent = __LDBL_MAX_EXP__;
  static constexpr const int max_exponent10 = __LDBL_MAX_10_EXP__;

  static constexpr const bool has_infinity = true;
  static constexpr const bool has_quiet_NaN = true;
  static constexpr const bool has_signaling_NaN = true;
  static constexpr const float_denorm_style has_denorm = denorm_present;
  static constexpr const bool has_denorm_loss = false;
  FORCE_INLINE static constexpr type infinity() noexcept {return __builtin_huge_vall();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __builtin_nanl("");}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __builtin_nansl("");}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __LDBL_DENORM_MIN__;}

  static constexpr const bool is_iec559 = true;
  static constexpr const bool is_bounded = true;
  static constexpr const bool is_modulo = false;
  
  static constexpr const bool traps = false;
  static constexpr const bool tinyness_before = false;
  static constexpr const float_round_style round_style = round_to_nearest;
};

// Actual usable numeric_limits class
template<class T>
class numeric_limits : private wasm_numeric_limits<typename type_traits::remove_cv<T>::type>
{
  typedef wasm_numeric_limits<typename type_traits::remove_cv<T>::type> __base;
  typedef typename __base::type type;
public:
  static constexpr const bool is_specialized = __base::is_specialized;
  FORCE_INLINE static constexpr type min() noexcept {return __base::min();}
  FORCE_INLINE static constexpr type max() noexcept {return __base::max();}
  FORCE_INLINE static constexpr type lowest() noexcept {return __base::lowest();}

  static constexpr const int digits = __base::digits;
  static constexpr const int digits10 = __base::digits10;
  static constexpr const int max_digits10 = __base::max_digits10;
  static constexpr const bool is_signed = __base::is_signed;
  static constexpr const bool is_integer = __base::is_integer;
  static constexpr const bool is_exact = __base::is_exact;
  static constexpr const int radix = __base::radix;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __base::epsilon();}
  FORCE_INLINE static constexpr type round_error() noexcept {return __base::round_error();}

  static constexpr const int min_exponent = __base::min_exponent;
  static constexpr const int min_exponent10 = __base::min_exponent10;
  static constexpr const int max_exponent = __base::max_exponent;
  static constexpr const int max_exponent10 = __base::max_exponent10;

  static constexpr const bool has_infinity = __base::has_infinity;
  static constexpr const bool has_quiet_NaN = __base::has_quiet_NaN;
  static constexpr const bool has_signaling_NaN = __base::has_signaling_NaN;
  static constexpr const float_denorm_style has_denorm = __base::has_denorm;
  static constexpr const bool has_denorm_loss = __base::has_denorm_loss;
  FORCE_INLINE static constexpr type infinity() noexcept {return __base::infinity();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __base::quiet_NaN();}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __base::signaling_NaN();}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __base::denorm_min();}

  static constexpr const bool is_iec559 = __base::is_iec559;
  static constexpr const bool is_bounded = __base::is_bounded;
  static constexpr const bool is_modulo = __base::is_modulo;

  static constexpr const bool traps = __base::traps;
  static constexpr const bool tinyness_before = __base::tinyness_before;
  static constexpr const float_round_style round_style = __base::round_style;
};

template<class T> constexpr const bool numeric_limits<T>::is_specialized;
template<class T> constexpr const int numeric_limits<T>::digits;
template<class T> constexpr const int numeric_limits<T>::digits10;
template<class T> constexpr const int numeric_limits<T>::max_digits10;
template<class T> constexpr const bool numeric_limits<T>::is_signed;
template<class T> constexpr const bool numeric_limits<T>::is_integer;
template<class T> constexpr const bool numeric_limits<T>::is_exact;
template<class T> constexpr const int numeric_limits<T>::radix;
template<class T> constexpr const int numeric_limits<T>::min_exponent;
template<class T> constexpr const int numeric_limits<T>::min_exponent10;
template<class T> constexpr const int numeric_limits<T>::max_exponent;
template<class T> constexpr const int numeric_limits<T>::max_exponent10;
template<class T> constexpr const bool numeric_limits<T>::has_infinity;
template<class T> constexpr const bool numeric_limits<T>::has_quiet_NaN;
template<class T> constexpr const bool numeric_limits<T>::has_signaling_NaN;
template<class T> constexpr const float_denorm_style numeric_limits<T>::has_denorm;
template<class T> constexpr const bool numeric_limits<T>::has_denorm_loss;
template<class T> constexpr const bool numeric_limits<T>::is_iec559;
template<class T> constexpr const bool numeric_limits<T>::is_bounded;
template<class T> constexpr const bool numeric_limits<T>::is_modulo;
template<class T> constexpr const bool numeric_limits<T>::traps;
template<class T> constexpr const bool numeric_limits<T>::tinyness_before;
template<class T> constexpr const float_round_style numeric_limits<T>::round_style;

template<class T>
class numeric_limits<const T> : private numeric_limits<T>
{
  typedef numeric_limits<T> __base;
  typedef T type;
public:
  static constexpr const bool is_specialized = __base::is_specialized;
  FORCE_INLINE static constexpr type min() noexcept {return __base::min();}
  FORCE_INLINE static constexpr type max() noexcept {return __base::max();}
  FORCE_INLINE static constexpr type lowest() noexcept {return __base::lowest();}

  static constexpr const int digits = __base::digits;
  static constexpr const int digits10 = __base::digits10;
  static constexpr const int max_digits10 = __base::max_digits10;
  static constexpr const bool is_signed = __base::is_signed;
  static constexpr const bool is_integer = __base::is_integer;
  static constexpr const bool is_exact = __base::is_exact;
  static constexpr const int radix = __base::radix;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __base::epsilon();}
  FORCE_INLINE static constexpr type round_error() noexcept {return __base::round_error();}

  static constexpr const int min_exponent = __base::min_exponent;
  static constexpr const int min_exponent10 = __base::min_exponent10;
  static constexpr const int max_exponent = __base::max_exponent;
  static constexpr const int max_exponent10 = __base::max_exponent10;

  static constexpr const bool has_infinity = __base::has_infinity;
  static constexpr const bool has_quiet_NaN = __base::has_quiet_NaN;
  static constexpr const bool has_signaling_NaN = __base::has_signaling_NaN;
  static constexpr const float_denorm_style has_denorm = __base::has_denorm;
  static constexpr const bool has_denorm_loss = __base::has_denorm_loss;
  FORCE_INLINE static constexpr type infinity() noexcept {return __base::infinity();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __base::quiet_NaN();}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __base::signaling_NaN();}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __base::denorm_min();}

  static constexpr const bool is_iec559 = __base::is_iec559;
  static constexpr const bool is_bounded = __base::is_bounded;
  static constexpr const bool is_modulo = __base::is_modulo;

  static constexpr const bool traps = __base::traps;
  static constexpr const bool tinyness_before = __base::tinyness_before;
  static constexpr const float_round_style round_style = __base::round_style;
};

template<class T> constexpr const bool numeric_limits<const T>::is_specialized;
template<class T> constexpr const int numeric_limits<const T>::digits;
template<class T> constexpr const int numeric_limits<const T>::digits10;
template<class T> constexpr const int numeric_limits<const T>::max_digits10;
template<class T> constexpr const bool numeric_limits<const T>::is_signed;
template<class T> constexpr const bool numeric_limits<const T>::is_integer;
template<class T> constexpr const bool numeric_limits<const T>::is_exact;
template<class T> constexpr const int numeric_limits<const T>::radix;
template<class T> constexpr const int numeric_limits<const T>::min_exponent;
template<class T> constexpr const int numeric_limits<const T>::min_exponent10;
template<class T> constexpr const int numeric_limits<const T>::max_exponent;
template<class T> constexpr const int numeric_limits<const T>::max_exponent10;
template<class T> constexpr const bool numeric_limits<const T>::has_infinity;
template<class T> constexpr const bool numeric_limits<const T>::has_quiet_NaN;
template<class T> constexpr const bool numeric_limits<const T>::has_signaling_NaN;
template<class T> constexpr const float_denorm_style numeric_limits<const T>::has_denorm;
template<class T> constexpr const bool numeric_limits<const T>::has_denorm_loss;
template<class T> constexpr const bool numeric_limits<const T>::is_iec559;
template<class T> constexpr const bool numeric_limits<const T>::is_bounded;
template<class T> constexpr const bool numeric_limits<const T>::is_modulo;
template<class T> constexpr const bool numeric_limits<const T>::traps;
template<class T> constexpr const bool numeric_limits<const T>::tinyness_before;
template<class T> constexpr const float_round_style numeric_limits<const T>::round_style;

template<class T>
class numeric_limits<volatile T> : private numeric_limits<T>
{
  typedef numeric_limits<T> __base;
  typedef T type;
public:
  static constexpr const bool is_specialized = __base::is_specialized;
  FORCE_INLINE static constexpr type min() noexcept {return __base::min();}
  FORCE_INLINE static constexpr type max() noexcept {return __base::max();}
  FORCE_INLINE static constexpr type lowest() noexcept {return __base::lowest();}

  static constexpr const int digits = __base::digits;
  static constexpr const int digits10 = __base::digits10;
  static constexpr const int max_digits10 = __base::max_digits10;
  static constexpr const bool is_signed = __base::is_signed;
  static constexpr const bool is_integer = __base::is_integer;
  static constexpr const bool is_exact = __base::is_exact;
  static constexpr const int radix = __base::radix;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __base::epsilon();}
  FORCE_INLINE static constexpr type round_error() noexcept {return __base::round_error();}

  static constexpr const int min_exponent = __base::min_exponent;
  static constexpr const int min_exponent10 = __base::min_exponent10;
  static constexpr const int max_exponent = __base::max_exponent;
  static constexpr const int max_exponent10 = __base::max_exponent10;

  static constexpr const bool has_infinity = __base::has_infinity;
  static constexpr const bool has_quiet_NaN = __base::has_quiet_NaN;
  static constexpr const bool has_signaling_NaN = __base::has_signaling_NaN;
  static constexpr const float_denorm_style has_denorm = __base::has_denorm;
  static constexpr const bool has_denorm_loss = __base::has_denorm_loss;
  FORCE_INLINE static constexpr type infinity() noexcept {return __base::infinity();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __base::quiet_NaN();}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __base::signaling_NaN();}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __base::denorm_min();}

  static constexpr const bool is_iec559 = __base::is_iec559;
  static constexpr const bool is_bounded = __base::is_bounded;
  static constexpr const bool is_modulo = __base::is_modulo;

  static constexpr const bool traps = __base::traps;
  static constexpr const bool tinyness_before = __base::tinyness_before;
  static constexpr const float_round_style round_style = __base::round_style;
};

template<class T> constexpr const bool numeric_limits<volatile T>::is_specialized;
template<class T> constexpr const int numeric_limits<volatile T>::digits;
template<class T> constexpr const int numeric_limits<volatile T>::digits10;
template<class T> constexpr const int numeric_limits<volatile T>::max_digits10;
template<class T> constexpr const bool numeric_limits<volatile T>::is_signed;
template<class T> constexpr const bool numeric_limits<volatile T>::is_integer;
template<class T> constexpr const bool numeric_limits<volatile T>::is_exact;
template<class T> constexpr const int numeric_limits<volatile T>::radix;
template<class T> constexpr const int numeric_limits<volatile T>::min_exponent;
template<class T> constexpr const int numeric_limits<volatile T>::min_exponent10;
template<class T> constexpr const int numeric_limits<volatile T>::max_exponent;
template<class T> constexpr const int numeric_limits<volatile T>::max_exponent10;
template<class T> constexpr const bool numeric_limits<volatile T>::has_infinity;
template<class T> constexpr const bool numeric_limits<volatile T>::has_quiet_NaN;
template<class T> constexpr const bool numeric_limits<volatile T>::has_signaling_NaN;
template<class T> constexpr const float_denorm_style numeric_limits<volatile T>::has_denorm;
template<class T> constexpr const bool numeric_limits<volatile T>::has_denorm_loss;
template<class T> constexpr const bool numeric_limits<volatile T>::is_iec559;
template<class T> constexpr const bool numeric_limits<volatile T>::is_bounded;
template<class T> constexpr const bool numeric_limits<volatile T>::is_modulo;
template<class T> constexpr const bool numeric_limits<volatile T>::traps;
template<class T> constexpr const bool numeric_limits<volatile T>::tinyness_before;
template<class T> constexpr const float_round_style numeric_limits<volatile T>::round_style;

template<class T>
class numeric_limits<const volatile T> : private numeric_limits<T>
{
  typedef numeric_limits<T> __base;
  typedef T type;
public:
  static constexpr const bool is_specialized = __base::is_specialized;
  FORCE_INLINE static constexpr type min() noexcept {return __base::min();}
  FORCE_INLINE static constexpr type max() noexcept {return __base::max();}
  FORCE_INLINE static constexpr type lowest() noexcept {return __base::lowest();}

  static constexpr const int digits = __base::digits;
  static constexpr const int digits10 = __base::digits10;
  static constexpr const int max_digits10 = __base::max_digits10;
  static constexpr const bool is_signed = __base::is_signed;
  static constexpr const bool is_integer = __base::is_integer;
  static constexpr const bool is_exact = __base::is_exact;
  static constexpr const int radix = __base::radix;
  FORCE_INLINE static constexpr type epsilon() noexcept {return __base::epsilon();}
  FORCE_INLINE static constexpr type round_error() noexcept {return __base::round_error();}

  static constexpr const int min_exponent = __base::min_exponent;
  static constexpr const int min_exponent10 = __base::min_exponent10;
  static constexpr const int max_exponent = __base::max_exponent;
  static constexpr const int max_exponent10 = __base::max_exponent10;

  static constexpr const bool has_infinity = __base::has_infinity;
  static constexpr const bool has_quiet_NaN = __base::has_quiet_NaN;
  static constexpr const bool has_signaling_NaN = __base::has_signaling_NaN;
  static constexpr const float_denorm_style has_denorm = __base::has_denorm;
  static constexpr const bool has_denorm_loss = __base::has_denorm_loss;
  FORCE_INLINE static constexpr type infinity() noexcept {return __base::infinity();}
  FORCE_INLINE static constexpr type quiet_NaN() noexcept {return __base::quiet_NaN();}
  FORCE_INLINE static constexpr type signaling_NaN() noexcept {return __base::signaling_NaN();}
  FORCE_INLINE static constexpr type denorm_min() noexcept {return __base::denorm_min();}

  static constexpr const bool is_iec559 = __base::is_iec559;
  static constexpr const bool is_bounded = __base::is_bounded;
  static constexpr const bool is_modulo = __base::is_modulo;

  static constexpr const bool traps = __base::traps;
  static constexpr const bool tinyness_before = __base::tinyness_before;
  static constexpr const float_round_style round_style = __base::round_style;
};

template<class T> constexpr const bool numeric_limits<const volatile T>::is_specialized;
template<class T> constexpr const int numeric_limits<const volatile T>::digits;
template<class T> constexpr const int numeric_limits<const volatile T>::digits10;
template<class T> constexpr const int numeric_limits<const volatile T>::max_digits10;
template<class T> constexpr const bool numeric_limits<const volatile T>::is_signed;
template<class T> constexpr const bool numeric_limits<const volatile T>::is_integer;
template<class T> constexpr const bool numeric_limits<const volatile T>::is_exact;
template<class T> constexpr const int numeric_limits<const volatile T>::radix;
template<class T> constexpr const int numeric_limits<const volatile T>::min_exponent;
template<class T> constexpr const int numeric_limits<const volatile T>::min_exponent10;
template<class T> constexpr const int numeric_limits<const volatile T>::max_exponent;
template<class T> constexpr const int numeric_limits<const volatile T>::max_exponent10;
template<class T> constexpr const bool numeric_limits<const volatile T>::has_infinity;
template<class T> constexpr const bool numeric_limits<const volatile T>::has_quiet_NaN;
template<class T> constexpr const bool numeric_limits<const volatile T>::has_signaling_NaN;
template<class T> constexpr const float_denorm_style numeric_limits<const volatile T>::has_denorm;
template<class T> constexpr const bool numeric_limits<const volatile T>::has_denorm_loss;
template<class T> constexpr const bool numeric_limits<const volatile T>::is_iec559;
template<class T> constexpr const bool numeric_limits<const volatile T>::is_bounded;
template<class T> constexpr const bool numeric_limits<const volatile T>::is_modulo;
template<class T> constexpr const bool numeric_limits<const volatile T>::traps;
template<class T> constexpr const bool numeric_limits<const volatile T>::tinyness_before;
template<class T> constexpr const float_round_style numeric_limits<const volatile T>::round_style;
