#ifndef __FUZZY_H__
#   define __FUZZY_H__

#include "common.h"

#ifdef __cplusplus
    extern "C"{
#endif /* __CPLUSPLUS */

/**< 默认输入参数类型为浮点型 */
#ifndef FUZZY_ITYPE
#   define FUZZY_ITYPE float
#endif

/**< 默认输出参数类型为短整型 */
#ifndef FUZZY_OTYPE
#   define FUZZT_OTYPE uint16_t
#endif

typedef struct
{
    /**<误差标志数组  */
    FUZZY_ITYPE*    err;
    /**< 误差变化率标志数组 */
    FUZZY_ITYPE*    errc;
    /**< 输出标志数组 */
    FUZZT_OTYPE*    out;
    /**< 输出规则表 */
    int8_t**        rule;
    /**< 输出规则表的列数，同时也是误差标志数组的大小 */
    uint8_t         rule_col;
    /**< 输出规则表的行数，同时也是误差变化率数组的大小 */
    uint8_t         rule_row;
}fuzzy_state;

static FUZZT_OTYPE fuzzy_ctrl(fuzzy_state* state, FUZZY_ITYPE err, FUZZY_ITYPE errc)
{
    /**< 定义误差隶属度 */
    float err_memship[2];
    /**< 定义误差变化率隶属度 */
    float errc_memship[2];
    /**< 定义输出隶属度 */
    float out_memship[2][2];
    /**< 定义误差模糊等级 */
    uint8_t err_lvl;
    /**< 定义误差变化率模糊等级 */
    uint8_t errc_lvl;
    /**< 定义输出模糊等级 */
    int8_t out_lvl[2][2];

    const uint8_t col = state->rule_col;
    const uint8_t row = state->rule_row;

    float t1, t2;
    /**< 模糊化：将误差按照规则映射到相应的模糊等级上，并按照三角形隶属函数计算其隶属度 */
    if( err>state->err[0] && err<state->err[col-1] )
    {
        int i;
        for( i=col-2;i>0;i-- )
        {
            if( err>=state->err[i] )
                break;
        }
        err_lvl = i;
        err_memship[0] = (err-state->err[i])/(state->err[i+1]-state->err[i]);
    }
    else
    {
        if( err>=state->err[col-1] )
        {
            err_lvl = col-2;
            err_memship[0] = 0.0f;
        }
        else
        {
            err_lvl = 0;
            err_memship[0] = 1.0f;
        }
    }
    err_memship[1] = 1.0f - err_memship[0];
    /**< 模糊化：将误差变化率按照规则映射到相应的模糊等级上，并按照三角形隶属函数计算其隶属度 */
    if( errc>state->errc[0] && errc<state->errc[row-1] )
    {
        int i;
        for( i=row-2;i>0;i-- )
        {
            if( errc>=state->errc[i] )
                break;
        }
        errc_lvl = i;
        errc_memship[0] = (errc-state->errc[i])/(state->errc[i+1]-state->errc[i]);
    }
    else
    {
        if( errc>=state->errc[row-1] )
        {
            errc_lvl = row-2;
            errc_memship[0] = 0.0f;
        }
        else
        {
            errc_lvl = 0;
            errc_memship[0] = 1.0f;
        }
    }
    errc_memship[1] = 1.0f - errc_memship[0];
    /**< 按照模糊规则表，计算输出的模糊等级并且按照最小模糊决策法计算对应的模糊隶属度 */
    out_lvl[0][0] = state->rule[errc_lvl][err_lvl++];
    out_lvl[0][1] = state->rule[errc_lvl++][err_lvl];
    out_lvl[1][1] = state->rule[errc_lvl][err_lvl--];
    out_lvl[1][0] = state->rule[errc_lvl][err_lvl];

    out_memship[0][0] = min(err_memship[0], errc_memship[0]);
    out_memship[0][1] = min(err_memship[0], errc_memship[1]);
    out_memship[1][1] = min(err_memship[1], errc_memship[1]);
    out_memship[1][0] = min(err_memship[1], errc_memship[0]);
    /**< 按照重心法反模糊，解出确定的输出 */
    t1 = out_memship[0][0] * state->out[out_lvl[0][0]]+\
         out_memship[0][1] * state->out[out_lvl[0][1]]+\
         out_memship[1][1] * state->out[out_lvl[1][1]]+\
         out_memship[1][0] * state->out[out_lvl[1][0]];

    t2 = out_memship[0][0]+out_memship[0][1]+\
         out_memship[1][1]+out_memship[1][0];

    return (FUZZT_OTYPE)(t1/t2);
}



#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __FUZZY_H__ */
