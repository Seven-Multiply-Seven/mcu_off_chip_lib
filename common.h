/****
    *@file:common.h
    *@brief:common head file.
    *@date: 10/30 2017
    *@author: sms, TongzhouXueshe of DMU, All rights reserved.
    *@attention: file encoding setting: UTF-8
****/
#ifndef __COMMON_H__
#   define __COMMON_H__

#include <stddef.h>
#include <limits.h>

#ifdef __cplusplus
    extern "C" {
#endif  /* __CPLUSPLUS */

#ifdef __STDC__
/* Some version of Standard C */
#if defined(__STDC__VERSION__)&&__STDC_VERSION__>=199901L
     /* C99 */
#   include <stdbool.h>
#   include <stdint.h>

#elif defined(__STDC_VERSION__)&&__STDC_VERSION__>=199409L
             /* C89 and Amendment 1 */
#   define inline
typedef enum
{
    false = 0,
    true = !false,
}bool;

#   if UCHAR_MAX == 0xFF
typedef signed char int8_t;
typedef unsigned char uint8_t;
#   elif UCHAR_MAX == 0xFFFF
typedef signed char int16_t;
typedef unsigned char uint16_t;
#   endif //UCHAR_MAX == 0xFF

#   if USHRT_MAX == 0xFFFF && UCHAR_MAX != 0xFFFF
typedef signed short int16_t;
typedef unsigned short uint16_t;
#   elif USHRT_MAX == 0xFFFFFFFF
typedef signed short int32_t;
typedef unsigned short uint32_t;
#   endif //USHRT_MAX == 0xFFFF && UCHAR_MAX != 0xFFFF

#   if UINT_MAX == 0xFFFF && USHRT_MAX != 0xFFFF
typedef signed int int16_t;
typedef unsigned int uint16_t;
#   elif UINT_MAX == 0xFFFFFFFF && USHRT_MAX != 0xFFFFFFFF
typedef signed int int32_t;
typedef unsigned int uint32_t;
#   elif UINT_MAX == 0xFFFFFFFFFFFFFFFF
typedef signed int int64_t;
typedef signed int uint64_t;
#   endif //UINT_MAX == 0xFFFF && USHRT_MAX != 0xFFFF

#   if ULONG_MAX == 0xFFFFFFFF && UINT_MAX != 0xFFFFFFFF && USHRT_MAX != 0xFFFFFFFF
typedef signed long int32_t;
typedef signed long uint32_t;
#   elif ULONG_MAX == 0xFFFFFFFFFFFFFFFF & UINT_MAX != 0xFFFFFFFFFFFFFFFF
typedef signed long int64_t;
typedef unsigned long uint64_t;
#   endif //ULONG_MAX == 0xFFFFFFFF && UINT_MAX != 0xFFFFFFFF && USHRT_MAX != 0xFFFFFFFF

#else
     /* C89 but not Amendment 1*/
#   define inline
typedef enum
{
    false = 0,
    true = !false,
}bool;

#   if UCHAR_MAX == 0xFF
typedef signed char int8_t;
typedef unsigned char uint8_t;
#   elif UCHAR_MAX == 0xFFFF
typedef signed char int16_t;
typedef unsigned char uint16_t;
#   endif /* UCHAR_MAX == 0xFF */

#   if USHRT_MAX == 0xFFFF && UCHAR_MAX != 0xFFFF
typedef signed short int16_t;
typedef unsigned short uint16_t;
#   elif USHRT_MAX == 0xFFFFFFFF
typedef signed short int32_t;
typedef unsigned short uint32_t;
#   endif /* USHRT_MAX == 0xFFFF && UCHAR_MAX != 0xFFFF */

#   if UINT_MAX == 0xFFFF && USHRT_MAX != 0xFFFF
typedef signed int int16_t;
typedef unsigned int uint16_t;
#   elif UINT_MAX == 0xFFFFFFFF && USHRT_MAX != 0xFFFFFFFF
typedef signed int int32_t;
typedef unsigned int uint32_t;
#   elif UINT_MAX == 0xFFFFFFFFFFFFFFFF
typedef signed int int64_t;
typedef signed int uint64_t;
#   endif /* UINT_MAX == 0xFFFF && USHRT_MAX != 0xFFFF */

#   if ULONG_MAX == 0xFFFFFFFF && UINT_MAX != 0xFFFFFFFF && USHRT_MAX != 0xFFFFFFFF
typedef signed long int32_t;
typedef signed long uint32_t;
#   elif ULONG_MAX == 0xFFFFFFFFFFFFFFFF & UINT_MAX != 0xFFFFFFFFFFFFFFFF
typedef signed long int64_t;
typedef unsigned long uint64_t;
#   endif /* ULONG_MAX == 0xFFFFFFFF && UINT_MAX != 0xFFFFFFFF && USHRT_MAX != 0xFFFFFFFF */

#endif /* defined(__STDC_VERSION__)&&__STDC_VERSION__>=199409L */

#else /* __STDC__not defined */
    /* Not Standard C */
#endif /* ifdef __STDC__ */

#if !(defined __C51__ || defined __CX51__)
#   define reentrant
#   define code
#endif


typedef void(*function_v_v)(void);
typedef void(*function_uc_v)(uint8_t);
typedef void(*function_us_v)(uint16_t);
typedef void(*function_ui_v)(uint32_t);
typedef void(*function_b_v)(bool);
typedef void(*function_str_v)(uint8_t*);
typedef bool(*function_v_b)(void);
typedef uint8_t(*function_v_uc)(void);
typedef uint8_t(*function_uc_uc)(uint8_t);

#define __JOIN(x, y) x##y
#define JOIN(x, y)  __JOIN(x, y)

#ifdef __GNUC__

#   define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#   define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))
#   define swap(a, b)               (__extension__          \
                                    ({                      \
                                    __auto_type _pa = &(a); \
                                    __auto_type _pb = &(b); \
                                    __auto_type _t = *_pa;  \
                                    (void)(_pa == _pb);     \
                                    *_pa = *_pb;            \
                                    *_pb = _t;              \
                                    })                      \
                                    )
#   define min(a, b)                (__extension__              \
                                    ({                          \
                                    const __auto_type _a = (a); \
                                    const __auto_type _b = (b); \
                                    (void)(&_a == &_b);         \
                                    _a > _b ? _b:_a;            \
                                    })                          \
                                    )
#   define max(a, b)                (__extension__              \
                                    ({                          \
                                    const __auto_type _a = (a); \
                                    const __auto_type _b = (b); \
                                    (void)(&_a == &_b);         \
                                    _a > _b ? _a:_b;            \
                                    })                          \
                                    )
#   define abs(x)                   (__extension__              \
                                    ({                          \
                                    const __auto_type _x = (x); \
                                    _x>0?_x:-_x;                \
                                    })                          \
                                    )
#   define container_of(ptr, type, member)             \
                                    (__extension__     \
                                    ({                 \
                                    const typeof( ((type *)0)->member ) *__mptr = (ptr);\
                                    (type *)( (char *)__mptr - offsetof(type,member) );})\
                                    )
#   define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#   define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#   define ARRAY_SIZE(array)        (__extension__                     \
                                    (sizeof((array))/sizeof((array[0]))\
                                    +__must_be_array(array))           \
                                    )

#else

#   define __attribute__(x)
#   define BUILD_BUG_ON_ZERO(e) (sizeof(struct JOIN(__static_assert_at_, __LINE__) { int:-!!(e); }))
#   define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct JOIN(__static_assert_at_, __LINE__) { int:-!!(e); }))

#   define swap(a, b)           do{                                             \
                                    char __t[sizeof(a)];                        \
                                    BUILD_BUG_ON_ZERO(sizeof(a) == sizeof(b));  \
                                    memcpy(__t, &(a), sizeof(a));               \
                                    memcpy(&(a), &(b), sizeof(b));              \
                                    memcpy(&(a), __t, sizeof(a));               \
                                }while(0)
#   define min(a, b)            ((a)>(b)?(b):(a))
#   define max(a, b)            ((a)<(b)?(b):(a))
#   define abs(a)               ((a)>0?(a):(-a))
#   define container_of(ptr, type, member)                                      \
                                ((type *)( (char *)ptr - offsetof(type,member) ))
#   define ARRAY_SIZE(array)    (sizeof(array)/sizeof( (array)[0] ))

#endif /* __GNUC__ */

static inline bool is_power_of_2(uint32_t n) __attribute__((unused));
static inline bool is_power_of_2(uint32_t n)
{
    if(n<1)
        return false;
    return (bool)((n&(n-1)) == 0);
}

static inline uint32_t fls(uint32_t x) __attribute__((unused));
static inline uint32_t fls(uint32_t x)
{
    uint32_t pos;
    uint32_t i;

    if(0 != x)
    {
        for(i = (x>>1), pos = 0; i != 0; ++pos)
            i >>= 1;
    }
    else
        pos = (uint32_t)-1;
    return ++pos;
}

static inline uint32_t roundup_pow_of_2(uint32_t x) __attribute__((unused));
static inline uint32_t roundup_pow_of_2(uint32_t x)
{
    if(x)
        return 1UL << fls(x-1);
    else
        return 0;
}

#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __COMMON_H__ */
