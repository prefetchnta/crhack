/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-13  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 字符串转浮点数模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    转实数T
=======================================
*/
CR_API REAL_T
#if     (TYPE_T == 16)
CR_FAW(str2real) (
#elif   (TYPE_T == 32)
CR_FAW(str2fp32) (
#elif   (TYPE_T == 64)
CR_FAW(str2fp64) (
#endif
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         skip
    )
{
    bool_t          sign;
    REAL_T          val = 0, p10;
    sint_t          count, exp = 0;
    sint_t          num_digits = 0;
    sint_t          num_decimals = 0;
    const XCHAR*    ret = str;

    /* 处理符号 */
    str = CR_FAW(skip_space)(str);
    if (*str == CR_XC('-'))
    {
        str++;
        sign = TRUE;
    }
    else
    if (*str == CR_XC('+'))
    {
        str++;
        sign = FALSE;
    }
    else
    {
        sign = FALSE;
    }

    /* 计算整数部分 */
    while (CR_FAW(is_digit)(*str))
    {
        num_digits++;
        val *= 10;
        val += 0x0F & *str++;
    }

    /* 计算小数部分 */
    if (*str == CR_XC('.'))
    {
        str++;
        while (CR_FAW(is_digit)(*str))
        {
            num_digits++;
            num_decimals++;
            val *= 10;
            val += 0x0F & *str++;
        }
        exp -= num_decimals;
    }

    if (num_digits == 0)
    {
        if (skip != NULL)
            *skip = 0;
        return ((REAL_T)0);
    }

    if (sign)
        val = -val;

    /* 处理科学计数法 */
    if (*str == CR_XC('e') || *str == CR_XC('E'))
    {
        str++;
        if (*str == CR_XC('-'))
        {
            str++;
            sign = TRUE;
        }
        else
        if (*str == CR_XC('+'))
        {
            str++;
            sign = FALSE;
        }
        else
        {
            sign = FALSE;
        }

        count = 0;
        while (CR_FAW(is_digit)(*str))
        {
            count *= 10;
            count += 0x0F & *str++;
        }

        if (sign)
            exp -= count;
        else
            exp += count;
    }

    p10 = 10;
    count = exp;
    if (count < 0)
        count = -count;

    /* 计算最后结果 */
    while (count != 0)
    {
        /* 这种计算方法误差最小 */
        if (count & 1)
        {
            if (exp < 0)
                val /= p10;
            else
                val *= p10;
        }
        p10 *= p10;
        count /= 2;
    }

    if (skip != NULL)
        *skip = (leng_t)(str - ret);
    return (val);
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
