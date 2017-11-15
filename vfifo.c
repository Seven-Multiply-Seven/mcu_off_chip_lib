#define VFIFO_GLOBAL
#include "vfifo.h"

extern void* memcpy(void*, const void*, size_t);

void vfifo_init(vfifo* fifo, void* buff, unsigned int buff_sz)
{
    if(!is_power_of_2(buff_sz))
        buff_sz = roundup_pow_of_2(buff_sz);
    fifo->buff = buff;
    fifo->buff_sz = buff_sz;
    fifo->in = fifo->out = 0;
}

unsigned int vfifo_put(vfifo* fifo, const void* buff, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->buff_sz - fifo->in + fifo->out);
    l = min(len, fifo->buff_sz - (fifo->in & (fifo->buff_sz-1)));

    memcpy((char*)fifo->buff+(fifo->in&(fifo->buff_sz-1)), buff, l);
    memcpy((char*)fifo->buff+l, fifo->buff, len-l);

    fifo->in += len;

    return len;
}

unsigned int vfifo_get(vfifo* fifo, void* buff, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->in - fifo->out);
    l = min(len, fifo->buff_sz - (fifo->out & (fifo->buff_sz-1)));

    memcpy(buff, (char*)fifo->buff+(fifo->out&(fifo->buff_sz-1)), l);
    memcpy((char*)buff+l, fifo->buff, len-l);

    fifo->out += len;

    return len;
}

unsigned int vfifo_used(vfifo* fifo)
{
    return fifo->in - fifo->out;
}
