/**
 *@file: binary_heap_sort.h
 *@author: SMS
 *@date: 2017/09/29
 *@brief: Generic method for binary heap sort,
        suitable for whole data types(i hope so).
        In my opinion, it suits most situations.
**/
#ifndef __BINARY_HEAP_SORT_H__
#   define __BINARY_HEAP_SORT_H__

#include "common.h"

#ifdef __cplusplus
    extern "C"{
#endif /* __CPLUSPLUS */

#ifndef HEAP_ELE
    #define HEAP_ELE int
#endif /* HEAP_ELE */
/* compare function. specially for complex data type */
#ifndef CAMPARE_FUNC_TYPE
#   define CAMPARE_FUNC_TYPE
typedef bool (*campare_func)(HEAP_ELE*, HEAP_ELE*) reentrant;
#endif /* CAMPARE_FUNC_TYPE */
/*
for example:
    bool large(int* a, int* b)
    {
        return *a>*b;
    }
    then this is a big heap
*/
static inline void __heap_swap(HEAP_ELE* a, HEAP_ELE* b)
{
    HEAP_ELE tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

static void __build_heap(HEAP_ELE a[], campare_func func, unsigned int bgn, unsigned int end)
{
    HEAP_ELE tmp = a[bgn];
    unsigned int i;

    for(i=(bgn<<1)+1; i<=end; bgn = i, i=(i<<1)+1)
    {
        if(i<end && !func(&a[i], &a[i+1]))
            i++;
        if(func(&tmp, &a[i]))
            break;
        a[bgn] = a[i];
        a[i] = tmp;
    }
}

static void heap_sort(HEAP_ELE a[], campare_func func, unsigned int len)
{
    unsigned int i=(len>>1) -1;
    /* overflow */
    for(; i<(unsigned int)-1; i--)
        __build_heap(a, func, i, len-1);

    for(i=len-1; i!=0; i--)
    {
        __heap_swap(&a[0], &a[i]);
        __build_heap(a, func, 0, i-1);
    }
}
#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __BINARY_HEAP_SORT_H__ */
