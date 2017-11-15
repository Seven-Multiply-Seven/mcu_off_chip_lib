/**
  *@brief: This is a complete template for some
  digital filter methods. When using this file, you
  can cut it to suit your project.
**/
#ifndef __FILTER_H__
#   define __FILTER_H__

#include "common.h"

#ifdef __cplusplus
    extern "C"{
#endif /* __CPLUSPLUS */

#ifndef FILTER_GLOBAL
#   define FILTER_EXT extern
#else
#   define FILTER_EXT
#endif

/* precision control */
typedef double fparam_t;
/* sort function for mean filter */
typedef void(*sort_func)(fparam_t a[], size_t len);
/**
  *@KalmanFilterFormula:
  **@PredictNextStatus:
    X(n|n-1) = AX(n-1|n-1)+BU(n);
    @A:State transition matrix;
    @B:Extern control transition matrix;
    @X:Status matrix;
    @U:Extern control matrix;
    P(n|n-1) = AP(n-1|n-1)A^T+Q;
    @Q:Prediction noise covariance;
    @P:Prediction status covariance;
  **@CalculateBestEstimation:
    Kg(n) = P(n|n-1)H^T*(H*P(n|n-1)*H^T+R)^-1;
    @Kg:Kalman Gain;
    @H:Observation conversion matrix;
    @R:Observation noise covariance;
    X(n|n) = X(n|n-1)+Kg(n)(Z(n)-HX(n|n-1));
    @Z:Observations;
  **@UpdateStatusCovariance:
    P(n|n) = (I-Kg(n)H)P(n|n-1);
**/
typedef struct
{
    fparam_t A;
    fparam_t B;
    fparam_t X;
    fparam_t U;
    fparam_t Q;
    fparam_t P;
    fparam_t Kg;
    fparam_t H;
    fparam_t R;
    fparam_t Z;
}kalman1_state;

typedef struct
{
    fparam_t A[2][2];
    fparam_t B[2];      /* [B0; B1] */
    fparam_t X[2];      /* [X0; X1] */
    fparam_t U;         /* Suppose there is only a extern control */
    fparam_t Q[2];      /* [Q0, 0; 0, Q1] Generally, we consider the prediction noise covariance matrix as a diagonal matrix */
    fparam_t P[2][2];
    fparam_t Kg[2][2];
    fparam_t H[2];      /* [H0, 0; 0, H1] Generally, we suppose status are independent */
    fparam_t R[2];      /* [R0, 0; 0, R0] Generally, we consider the observation noise covariance matrix as a diagonal matrix */
    fparam_t Z[2];      /* [Z0; Z1] */
}kalman2_state;

FILTER_EXT void kalman1_filter(kalman1_state* state);
FILTER_EXT void kalman2_filter(kalman2_state* state);
FILTER_EXT fparam_t mean_filter(fparam_t a[], size_t len, unsigned int cut, sort_func func);

#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __FILTER_H__ */
