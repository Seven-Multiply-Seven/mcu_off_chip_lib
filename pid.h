#ifndef __PID_H__
#   define __PID_H__

#include "common.h"

#ifdef __cplusplus
    extern "C" {
#endif /**< __CPLUSPLUS */

/**< 默认PID的输入参数类型是浮点型 */
#ifndef PID_ITYPE
#   define PID_ITYPE float
#endif
/**< 默认PID的输出参数类型是短整型 */
#ifndef PID_OTYPE
#   define PID_OTYPE uint16_t
#endif
/*------ 以下为控制宏，应当在本头文件前进行定义 ------*/
/**< 输出限幅 */
/* #define OUT_VAL_LIM */
/* 积分限幅 */
/* #define INT_VAL_LIM */
/**< 梯形积分 */
/* #define TRAPE_INT */
/**< 积分分离 */
/* #define INT_SEP */

typedef struct
{
    float Kp,Ki,Kd;
    PID_ITYPE pre_err, last_err, next_err;
    PID_ITYPE set_val;
    PID_OTYPE out_val;
    #if defined OUT_VAL_LIM
    PID_OTYPE out_max, out_min;
    #endif
    #ifdef INT_VAL_LIM
    PID_ITYPE int_abs_max;
    #endif
    #ifdef INT_SEP
    PID_ITYPE int_abs_sep;
    #endif
}pid_state;

static void pid_pos(pid_state* state)
{
#ifdef INT_SEP
    /**< 积分分离常数 */
    int index = 0;
    /**< 若当前偏差过大，则忽略积分作用（积分分离） */
    if( abs(state->next_err)<=state->int_abs_sep )
    {
        index = 1;
    }
#else
#   define index 1
#endif

#ifdef INT_VAL_LIM
    /**< 若当前积分值已达最值，则不再使其绝对值增大（积分限幅） */
    if( abs(state->pre_err)<=state->int_abs_max )
        state->pre_err += state->next_err;
    else
    {
        if( (state->pre_err<0 && state->next_err>0) || (state->pre_err>0 && state->next_err<0) )
            state->pre_err += state->next_err;
    }
#endif

#ifdef TRAPE_INT
    state->out_val = (state->Kp)*(state->next_err)+index*(state->Ki)*(state->pre_err/2.0f)\
    +(state->Kd)*(state->next_err-state->last_err);
#else
    state->out_val = (state->Kp)*(state->next_err)+index*(state->Ki)*(state->pre_err)\
    +(state->Kd)*(state->next_err-state->last_err);
#endif

    state->last_err = state->next_err;

#ifdef OUT_VAL_LIM
    state->out_val = min(state->out_val, state->out_max);
    state->out_val = max(state->out_val, state->out_min);
#endif

#ifdef index
#   undef index
#endif
}

static void pid_inc(pid_state* state)
{
#ifdef INT_SEP
    /**< 积分分离常数 */
    PID_ITYPE integral = 0;
    /**< 若当前偏差过大，则忽略积分作用（积分分离） */
    if( abs(state->next_err)<=state->int_abs_sep )
    {
        integral = state->next_err;
    }
#else
#   define integral (state->next_err)
#endif
    /**< 无需积分限幅 */
#ifdef TRAPE_INT
    state->out_val += (state->Kp)*(state->next_err-state->last_err)+(state->Ki)*(integral/2.0f)\
    +(state->Kd)*(state->next_err-2*state->last_err+state->pre_err);
#else
    state->out_val += (state->Kp)*(state->next_err-state->last_err)+(state->Ki)*integral\
    +(state->Kd)*(state->next_err-2*state->last_err+state->pre_err);
#endif

    state->pre_err = state->last_err;
    state->last_err = state->next_err;

#ifdef OUT_VAL_LIM
    state->out_val = min(state->out_val, state->out_max);
    state->out_val = max(state->out_val, state->out_min);
#endif

#ifdef integral
#   undef integral
#endif
}

/**< 输出限幅 */
#undef OUT_VAL_LIM
/**< 积分限幅 */
#undef INT_VAL_LIM
/**< 梯形积分 */
#undef TRAPE_INT
/**< 积分分离 */
#undef INT_SEP

#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __PID_H__ */
