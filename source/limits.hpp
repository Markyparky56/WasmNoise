#pragma once

// Integral limits 
#define CHAR_BIT          8                       // Number of bits in a char
#define SCHAR_MIN         (-128)                  // Minimum signed char value
#define SCHAR_MAX         127                     // Maximum signed char value
#define UCHAR_MAX         0xff                    // Maximum unsigned char value

#define SHRT_MIN          (-32768)                // Minimum signed short value
#define SHRT_MAX          32767                   // Maximum signed short value
#define USHRT_MAX         0xffff                  // Maximum unsigned short value
#define INT_MIN           (-2147483647 - 1)       // Minimum signed int value
#define INT_MAX           2147483647              // Maximum signed int value
#define UINT_MAX          0xffffffff              // Maximum unsigned int value
#define LONG_MIN          (-2147483647 - 1)       // Minimum signed long value
#define LONG_MAX          0xffffffff              // Maximum signed long value
#define ULONG_MAX         0xffffffffUL            // Maximum unsigned long value
#define LLONG_MAX         9223372036854775807i64  // Maximum signed long long int value
#define LLONG_MIN         (-9223372036854775807i64 - 1) // Minimum signed long long value
#define ULLONG_MAX        0xffffffffffffffffui64  // Maximum unsigned long long int value

// Floating point limits
#define DBL_DECIMAL_DIG   17                      // # of decimal digits of rounding precision
#define DBL_DIG           15                      // # of decimal digits of precision
#define DBL_EPSILON       2.2204460492503131e-016 // smallest such that 1.0+DBL_EPSILON != 1.0
#define DBL_HAS_SUBNORM   1                       // type does support subnormal numbers
#define DBL_MANT_DIG      53                      // # of bits in mantissa
#define DBL_MAX           1.7976931348623158e+308 // max value
#define DBL_MAX_10_EXP    308                     // max decimal exponent
#define DBL_MAX_EXP       1024                    // max binary exponent
#define DBL_MIN           2.2250738585072014e-308 // min positive (normalised?) value
#define DBL_MIN_10_EXP    (-307)                  // min decimal exponent
#define DBL_MIN_EXP       (-1021)                 // min binary exponent
#define DBL_RADIX        2                       // exponent radix
#define DBL_TRUE_MIN      4.9406564584124654e-324 // min positive value

#define FLT_DECIMAL_DIG   9                       // # of decimal digits of rounding precision
#define FLT_DIG           6                       // # of decimal digits of precision
#define FLT_EPSILON       1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
#define FLT_SUBNORM       1                       // type does support subnormal numbers
#define FLT_GUARD         0
#define FLT_MANT_DIG      24                      // # of bits in mantissa
#define FLT_MAX           3.402823466e+38F        // max value
#define FLT_MAX_10_EXP    38                      // max decimal exponent
#define FLT_MAX_EXP       128                     // max binary exponent
#define FLT_MIN           1.175494351e-38F        // min normalised positive value
#define FLT_MIN_10_EXP    (-37)                   // min decimal exponent
#define FLT_MIN_EXP       (-125)                  // min binary exponent
#define FLT_NORMALIZE     0
#define FLT_RADIX         2                       // exponent radix
#define FLT_TRUE_MIN      1.401298464e-45F        // min positive value

#define LDBL_DIG          DBL_DIG                 // # of decimal digits of precision
#define LDBL_EPSILON      DBL_EPSILON             // smallest such that 1.0+LDBL_EPSILON != 1.0
#define LDBL_SUB_SUBNORM  DBL_HAS_SUBNORM         // type does support subnormal numbers
#define LDBL_MANT_DIG     DBL_MANT_DIG            // # of bits in mantissa
#define LDBL_MAX          DBL_MAX                 // max value
#define LDBL_MAX_10_EXP   DBL_MAX_10_EXP          // max decimal exponent
#define LDBL_MAX_EXP      DBL_MAX_EXP             // max binary exponent
#define LDBL_MIN          DBL_MIN                 // min normalised positive value
#define LDBL_MIN_10_EXP   DBL_MIN_10_EXP          // min decimal exponent
#define LDBL_MIN_EXP      DBL_MIN_EXP             // min binary exponent
#define LDBL_RADIX       _DBL_RADIX              // exponent radix
#define LDBL_TRUE_MIN     DBL_TRUE_MIN            // min positive value

#define DECIMAL_DIG       DBL_DECIMAL_DIG

// __*__ variants
#define __CHAR_BIT__      CHAR_BIT

#define __FLT_MANT_DIG__    FLT_MANT_DIG
#define __FLT_DIG__         FLT_DIG
#define __FLT_RADIX__       FLT_RADIX
#define __FLT_MIN_EXP__     FLT_MIN_EXP
#define __FLT_MIN_10_EXP__  FLT_MIN_10_EXP
#define __FLT_MAX_EXP__     FLT_MAX_EXP
#define __FLT_MAX_10_EXP__  FLT_MAX_10_EXP
#define __FLT_MIN__         FLT_MIN
#define __FLT_MAX__         FLT_MAX
#define __FLT_EPSILON__     FLT_EPSILON
#define __FLT_DENORM_MUN__  1.40129846432481707092e-45F

#define __DBL_MANT_DIG__    DBL_MANT_DIH
#define __DBL_DIG__         DBL_DIG
#define __DBL_RADIX__       DBL_RADIX
#define __DBL_MIN_EXP__     DBL_MIN_EXP
#define __DBL_MIN_10_EXP__  DBL_MIN_10_EXP
#define __DBL_MAX_EXP__     DBL_MAX_EXP
#define __DBL_MAX_10_EXP__  DBL_MAX_10_EXP
#define __DBL_MIN__         DBL_MIN
#define __DBL_MAX__         DBL_MAX
#define __DBL_EPSILON__     DBL_EPSILON
#define __DBL_DENORM_MIN__  double(4.94065645841246544177e-324L)

#define __LDBL_MANT__DIG__  LDBL_MANT_DIG
#define __LDBL_DIG__        LDBL_DIG
#define __LDBL_RADIX__      LDBL_RADIX
#define __LDBL_MIN_EXP__    LDBL_MIN_EXP
#define __LDBL_MIN_10_EXP__ LDBL_MIN_10_EXP
#define __LDBL_MAX_EXP__    LDBL_MAX_EXP
#define __LDBL_MAX_10_EXP__ LDBL_MAX_10_EXP
#define __LDBL_MIN__        LDBL_MIN
#define __LDBL_MAX__        LDBL_MAX
#define __LDBL_EPSILON__    LDB_EPSILON
#define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L //given that LDBL is defined as just DBL above this looks wrong

// TODO: Check if clang supports __builtin_* funcs

// // INFINITY, HUGE_VAL, etc.
// #define HUGE_ENUF 1e+300 // Must overflow
// #define INFINITY ((float)(HUGE_ENUF*HUGE_ENUF))
// #define HUGE_VAL ((double)INFINITY)
// #define HUGEVALF ((float)INFINITY)
// #define HUGEVALL ((long double)INFINITY)
// #define NAN      ((float)(INFINITY*0.0f))

// // Quiet NaNs
// static const unsigned int _QNAN_F = 0x7fc00000;
// #define NANF (*((float *)(&_QNAN_F)))
// static const unsigned int _QNAN_LDBL128[4] = {0x7ff80000, 0x0, 0x0, 0x0};
// #define NANL (*((long double *)(&_QNAN_LDBL128)))

// // Signaling NaNs
// static const unsigned int _SNAN_F = 0x7f855555;
// #define NANSF (*((float *)(&_SNAN_F)))
// static const unsigned int _SNAN_D[2] = {0x7ff55555, 0x55555555};
// #define NANS (*((double *)(&_SNAN_D)))
// static const unsigned int _SNAN_LDBL128[4] = {0x7ff55555, 0x55555555, 0x0, 0x0};
// #define NANSL (*((long double *)(&_SNAN_LDBL128)))

// // __builtin macros
// #define __builtin_huge_val()      HUGE_VAL
// #define __builtin_hugevalf()      HUGE_VALF
// #define __builtin_hugevall()      HUGE_VALL
// #define __builtin_nan(__dummy)    NAN
// #define __builtin_nanf(__dummy)   NANF
// #define __builtin_nanl(__dummy)   NANL
// #define __builtin_nans(__dummy)   NANS
// #define __builtin_nansf(__dummy)  NANSF
// #define __builtin_nansl(__dummy)  NANSL




