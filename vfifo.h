#ifndef __VFIFO_H__
#   define __VFIFO_H__

#include "common.h"

#ifdef __cplusplus
    extern "C" {
#endif /* __CPLUSPLUS */

#ifndef VFIFO_GLOBAL
#   define VFIFO_EXT extern
#else
#   define VFIFO_EXT
#endif

typedef struct
{
    size_t          buff_sz;
    void*           buff;
    unsigned int    in, out;
}vfifo;

void vfifo_init(vfifo* fifo, void* buff, unsigned int buff_sz);
unsigned int vfifo_put(vfifo* fifo, const void* buff, unsigned int len);
unsigned int vfifo_get(vfifo* fifo, void* buff, unsigned int len);
unsigned int vfifo_used(vfifo* fifo);

#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __VFIFO_H__ */
