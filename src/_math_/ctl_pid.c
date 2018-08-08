/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2018-08-07  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack PID 控制函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*
=======================================
    PID 控制开始
=======================================
*/
CR_API void_t
pid_init (
  __CR_OT__ sCTL_PID*   pid,
  __CR_IN__ double      start
    )
{
    pid->I = 0;
    pid->O = start;
    pid->tots = 0;
    pid->E[0] = 0;
    pid->E[1] = 0;
    pid->E[2] = 0;
}

/*
=======================================
    PID 增量型计算
=======================================
*/
CR_API double
pid_delta (
  __CR_IO__ sCTL_PID*   pid,
  __CR_IN__ double      input
    )
{
    double  delta;

    pid->I = input;
    pid->E[2] = pid->E[1];
    pid->E[1] = pid->E[0];
    pid->E[0] = input - pid->O;
    delta = pid->E[0] - pid->E[1];
    delta = pid->Kp * delta + pid->Ki * pid->E[0] +
            pid->Kd * (delta - pid->E[1] + pid->E[2]);
    pid->O += delta;
    return (pid->O);
}

/*
=======================================
    PID 全功能计算
=======================================
*/
CR_API double
pid_full (
  __CR_IO__ sCTL_PID*   pid,
  __CR_IN__ double      input
    )
{
    double  ki, err;

    pid->I = input;
    pid->E[2] = pid->E[1];
    pid->E[1] = pid->E[0];
    pid->E[0] = input - pid->O;
    err = CR_ABS(pid->E[0]);
    if (pid->O > pid->umax) {
        if (err > pid->gate) {
            ki = 0;
        }
        else {
            if (pid->E[0] < 0)
                pid->tots += pid->E[0];
            ki = pid->Ki;
        }
    }
    else
    if (pid->O < pid->umin) {
        if (err > pid->gate) {
            ki = 0;
        }
        else {
            if (pid->E[0] > 0)
                pid->tots += pid->E[0];
            ki = pid->Ki;
        }
    }
    else {
        if (err > pid->gate) {
            ki = 0;
        }
        else {
            pid->tots += pid->E[0];
            ki = pid->Ki;
        }
    }
    pid->O = pid->Kp * pid->E[0] + ki * pid->tots +
             pid->Kd * (pid->E[0] - pid->E[1]);
    return (pid->O);
}

/*
=======================================
    PID 自定义积分项
=======================================
*/
CR_API double
pid_custom (
  __CR_IO__ sCTL_PID*   pid,
  __CR_IN__ double      input,
  __CR_IN__ double      (*ki_func)(sCTL_PID*, double)
    )
{
    double  ki, err;

    pid->I = input;
    pid->E[2] = pid->E[1];
    pid->E[1] = pid->E[0];
    pid->E[0] = input - pid->O;
    err = CR_ABS(pid->E[0]);
    if (err > pid->gate) {
        ki = 0;
    }
    else {
        pid->tots += pid->E[0];
        ki = ki_func(pid, err) * pid->Ki;
    }
    pid->O = pid->Kp * pid->E[0] + ki * pid->tots +
             pid->Kd * (pid->E[0] - pid->E[1]);
    return (pid->O);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
