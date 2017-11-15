#define FILTER_GLOBAL
#include "filter.h"

/***
 *@brief: one dimension Kalman filter.
 *@param:
        state: the pointer of filter state structure.
        measure_val: measured value.
 *@retval:
        NONE.
 *@attention:
        INIT THE STRUCTURE FIRST!
***/
void kalman1_filter(kalman1_state* state)
{
    /* X(n|n-1) = AX(n-1|n-1)+BU(n) */
    state->X *= state->A;
    state->X += state->B * state->U;
    /* P(n|n-1) = AP(n-1|n-1)A^T+Q */
    state->P *= state->A * state->A;
    state->P += state->Q;
    /* Kg(n) = P(n|n-1)H^T*(H*P(n|n-1)*H^T+R)^-1 */
    state->Kg = state->P * state->H / (state->H * state->P * state->H * state->R);
    /* X(n|n) = X(n|n-1)+Kg(n)(Z(n)-HX(n|n-1)) */
    state->X += state->Kg * (state->Z - state->H * state->X);
    /* P(n|n) = (I-Kg(n)H)P(n|n-1) */
    state->P *= 1 - state->Kg * state->H;
}
/***
 *@brief: two dimension Kalman filter.
 *@param:
        state: the pointer of filter state structure.
        measure_val: measured value.
 *@retval:
        NONE.
 *@attention:
        Generally speaking, A usually look like this:
        [1, sample time; 0, 1]
        INIT THE STRUCTURE FIRST!
***/
void kalman2_filter(kalman2_state* state)
{
    fparam_t tmp0,tmp1,tmp2,tmp3;
    /* calculate first, and assignment last! */
    /* X(n|n-1) = AX(n-1|n-1)+BU(n) */
    tmp0 = state->A[0][0] * state->X[0] + state->A[0][1] * state->X[1] + state->B[0] * state->U;
    tmp1 = state->A[1][0] * state->X[0] + state->A[1][1] * state->X[1] + state->B[1] * state->U;

    state->X[0] = tmp0;
    state->X[1] = tmp1;
    /* P(n|n-1) = AP(n-1|n-1)A^T+Q */
/*
 [ q0 + conj(a00)*(a00*p00 + a01*p10) + conj(a01)*(a00*p01 + a01*p11),      conj(a10)*(a00*p00 + a01*p10) + conj(a11)*(a00*p01 + a01*p11)]
 [      conj(a00)*(a10*p00 + a11*p10) + conj(a01)*(a10*p01 + a11*p11), q1 + conj(a10)*(a10*p00 + a11*p10) + conj(a11)*(a10*p01 + a11*p11)]
*/
    tmp0 = state->Q[0] + state->A[0][0] * ( state->A[0][0] * state->P[0][0] + state->A[0][1] * state->P[1][0] )\
                     + state->A[0][1] * ( state->A[0][0] * state->P[0][1] + state->A[0][1] * state->P[1][1] );
    tmp1 = state->A[1][0] * ( state->A[0][0] * state->P[0][0] + state->A[0][1] * state->P[1][0] )\
                     + state->A[1][1] * ( state->A[0][0] * state->P[0][1] + state->A[0][1] * state->P[1][1] );
    tmp2 = state->A[0][0] * ( state->A[1][0] * state->P[0][0] + state->A[1][1] * state->P[1][0] )\
                     + state->A[0][1] * ( state->A[1][0] * state->P[0][1] + state->A[1][1] * state->P[1][1] );
    tmp3 = state->Q[1] + state->A[1][0] * ( state->A[1][0] * state->P[0][0] + state->A[1][1] * state->P[1][0] )\
                     + state->A[1][1] * ( state->A[1][0] * state->P[0][1] + state->A[1][1] * state->P[1][1] );
    /* updating */
    state->P[0][0] = tmp0;
    state->P[0][1] = tmp1;
    state->P[1][0] = tmp2;
    state->P[1][1] = tmp3;
    /* Kg(n) = P(n|n-1)H^T*(H*P(n|n-1)*H^T+R)^-1 */
/* (h0*(p00*r1 + h1^2*p00*p11 - h1^2*p01*p10))/(r0*r1 + h0^2*p00*r1 + h1^2*p11*r0 + h0^2*h1^2*p00*p11 - h0^2*h1^2*p01*p10) */
    state->Kg[0][0] = (state->H[0]*(state->P[0][0]*state->R[1]+state->H[1]*state->H[1]*(state->P[0][0]*state->P[1][1]-state->P[0][1]*state->P[1][0])))\
                       /(state->R[0]*state->R[1]+state->H[0]*state->H[0]*state->P[0][0]*state->R[1]+state->H[1]*state->H[1]*state->P[1][1]*state->R[0]\
                       +state->H[0]*state->H[0]*state->H[1]*state->H[1]*(state->P[0][0]*state->P[1][1]-state->P[0][1]*state->P[1][0]));
/* (h1*(p11*r0 + h0^2*p00*p11 - h0^2*p01*p10))/(r0*r1 + h0^2*p00*r1 + h1^2*p11*r0 + h0^2*h1^2*p00*p11 - h0^2*h1^2*p01*p10) */
    state->Kg[1][1] = (state->H[1]*(state->P[1][1]*state->R[0]+state->H[0]*state->H[0]*(state->P[0][0]*state->P[1][1]-state->P[0][1]*state->P[1][0])))\
                       /(state->R[0]*state->R[1]+state->H[0]*state->H[0]*state->P[0][0]*state->R[1]+state->H[1]*state->H[1]*state->P[1][1]*state->R[0]\
                       +state->H[0]*state->H[0]*state->H[1]*state->H[1]*(state->P[0][0]*state->P[1][1]-state->P[0][1]*state->P[1][0]));
/* (h1*p01*r0)/(r0*r1 + h0^2*p00*r1 + h1^2*p11*r0 + h0^2*h1^2*p00*p11 - h0^2*h1^2*p01*p10) */
    state->Kg[0][1] = (state->H[1]*state->P[0][1]*state->R[0])/(state->R[0]*state->R[1]+state->H[0]*state->H[0]*state->P[0][0]*state->R[1]\
                      +state->H[1]*state->H[1]*state->P[1][1]*state->R[0]+state->H[0]*state->H[0]*state->H[1]*state->H[1]\
                      *(state->P[0][0]*state->P[1][1]-state->P[0][1]*state->P[1][0]));
/* (h0*p10*r1)/(r0*r1 + h0^2*p00*r1 + h1^2*p11*r0 + h0^2*h1^2*p00*p11 - h0^2*h1^2*p01*p10) */
    state->Kg[1][0] = (state->H[0]*state->P[1][0]*state->R[1])/(state->R[0]*state->R[1]+state->H[0]*state->H[0]*state->P[0][0]*state->R[1]\
                      +state->H[1]*state->H[1]*state->P[1][1]*state->R[0]+state->H[0]*state->H[0]*state->H[1]*state->H[1]\
                      *(state->P[0][0]*state->P[1][1]-state->P[0][1]*state->P[1][0]));
    /* X(n|n) = X(n|n-1)+Kg(n)(Z(n)-HX(n|n-1)) */
/* x0 + kg00*(z0 - h0*x0) + kg01*(z1 - h1*x1) */
    tmp0 = state->X[0]+state->Kg[0][0]*(state->Z[0]-state->H[0]*state->X[0])+state->Kg[0][1]*(state->Z[1]-state->H[1]*state->X[1]);
/* x1 + kg10*(z0 - h0*x0) + kg11*(z1 - h1*x1) */
    tmp1 = state->X[1]+state->Kg[1][0]*(state->Z[0]-state->H[0]*state->X[0])+state->Kg[1][1]*(state->Z[1]-state->H[1]*state->X[1]);
    state->X[0] = tmp0;
    state->X[1] = tmp1;
    /* P(n|n) = (I-Kg(n)H)P(n|n-1) */
/* [ - p00*(h0*kg00 - 1) - h1*kg01*p10, - p01*(h0*kg00 - 1) - h1*kg01*p11] */
/* [ - p10*(h1*kg11 - 1) - h0*kg10*p00, - p11*(h1*kg11 - 1) - h0*kg10*p01] */
    tmp0 = -state->P[0][0]*(state->H[0]*state->Kg[0][0]-1)-state->H[1]*state->Kg[0][1]*state->P[1][0];
    tmp1 = -state->P[0][1]*(state->H[0]*state->Kg[0][0]-1)-state->H[1]*state->Kg[0][1]*state->P[1][1];
    tmp2 = -state->P[1][0]*(state->H[1]*state->Kg[1][1]-1)-state->H[0]*state->Kg[1][0]*state->P[0][0];
    tmp3 = -state->P[1][1]*(state->H[1]*state->Kg[1][1]-1)-state->H[0]*state->Kg[1][0]*state->P[0][1];
    state->P[0][0] = tmp0;
    state->P[0][1] = tmp1;
    state->P[1][0] = tmp2;
    state->P[1][1] = tmp3;
}

fparam_t mean_filter(fparam_t a[], size_t len, unsigned int cut, sort_func func)
{
    fparam_t retval = 0;
    size_t i;
    func(a, len);
    for(i=cut; i<len-cut*2; i++)
    {
        retval += a[i];
    }
    return retval/(len-cut*2);
}
