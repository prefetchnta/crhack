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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 字符串转整数模板 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    整数转16进制串T
=======================================
*/
CR_API XCHAR*
#if     (TYPE_T == 16)
CR_FAW(int2str) (
#elif   (TYPE_T == 32)
CR_FAW(int2str32) (
#elif   (TYPE_T == 64)
CR_FAW(int2str64) (
#endif
  __CR_OT__ XCHAR   name[32],
  __CR_IN__ UINT_T  value
    )
{
#if defined(_CR_ORDER_LE_)
    UINT_T  temp;

#if     (TYPE_T == 16)
    #if defined(_CR_SYS16_)
    temp = xchg_int16u((int16u)value);
    #else   /* (_CR_SYS32_) */
            /* (_CR_SYS64_) */
    temp = xchg_int32u((int32u)value);
    #endif
#elif   (TYPE_T == 32)
    temp = xchg_int32u((int32u)value);
#elif   (TYPE_T == 64)
    temp = xchg_int64u((int64u)value);
#endif
    return (CR_FAW(hex2str)(name, &temp, sizeof(UINT_T)));

#else   /* (_CR_ORDER_BE_) */
    return (CR_FAW(hex2str)(name, &value, sizeof(UINT_T)));
#endif
}

/*
=======================================
    转十进制数T
=======================================
*/
CR_API SINT_T
#if     (TYPE_T == 16)
CR_FAW(str2int) (
#elif   (TYPE_T == 32)
CR_FAW(str2int32) (
#elif   (TYPE_T == 64)
CR_FAW(str2int64) (
#endif
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         skip
    )
{
    bool_t          sign;
    SINT_T          val = 0;
    const XCHAR*    ret = str;

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

    while (CR_FAW(is_digit)(*str))
    {
        val *= 10;
        val += 0x0F & *str++;
    }

    if (skip != NULL)
        *skip = (leng_t)(str - ret);
    return (sign ? -val : val);
}

/*
=======================================
    转二进制数T
=======================================
*/
CR_API UINT_T
#if     (TYPE_T == 16)
CR_FAW(str2bin) (
#elif   (TYPE_T == 32)
CR_FAW(str2bin32) (
#elif   (TYPE_T == 64)
CR_FAW(str2bin64) (
#endif
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         skip
    )
{
    UINT_T          val = 0;
    const XCHAR*    ret = str;

    str = CR_FAW(skip_space)(str);

    while (*str >= CR_XC('0') &&
           *str <= CR_XC('1')) {
        val <<= 1;
        val |= 0x0F & *str++;
    }

    if (skip != NULL)
        *skip = (leng_t)(str - ret);
    return (val);
}

/*
=======================================
    转八进制数T
=======================================
*/
CR_API UINT_T
#if     (TYPE_T == 16)
CR_FAW(str2oct) (
#elif   (TYPE_T == 32)
CR_FAW(str2oct32) (
#elif   (TYPE_T == 64)
CR_FAW(str2oct64) (
#endif
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         skip
    )
{
    UINT_T          val = 0;
    const XCHAR*    ret = str;

    str = CR_FAW(skip_space)(str);

    while (*str >= CR_XC('0') &&
           *str <= CR_XC('7')) {
        val <<= 3;
        val |= 0x0F & *str++;
    }

    if (skip != NULL)
        *skip = (leng_t)(str - ret);
    return (val);
}

/*
=======================================
    转16进制数T
=======================================
*/
CR_API UINT_T
#if     (TYPE_T == 16)
CR_FAW(str2hex) (
#elif   (TYPE_T == 32)
CR_FAW(str2hex32) (
#elif   (TYPE_T == 64)
CR_FAW(str2hex64) (
#endif
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         skip
    )
{
    UINT_T          val = 0;
    const XCHAR*    ret = str;

    str = CR_FAW(skip_space)(str);
    for (;;)
    {
        if (CR_FAW(is_digit)(*str))
        {
            val <<= 4;
            val |= (0x0F & *str++);
        }
        else
        if (CR_FAW(is_xnumb)(*str))
        {
            val <<= 4;
            val |= (0x0F & *str++) + 9;
        }
        else
        {
            break;
        }
    }

    if (skip != NULL)
        *skip = (leng_t)(str - ret);
    return (val);
}

/*
=======================================
    转 C 风格整数T
=======================================
*/
CR_API UINT_T
#if     (TYPE_T == 16)
CR_FAW(str2intx) (
#elif   (TYPE_T == 32)
CR_FAW(str2intx32) (
#elif   (TYPE_T == 64)
CR_FAW(str2intx64) (
#endif
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         skip
    )
{
    UINT_T          val;
    leng_t          nnn;
    const XCHAR*    ret = str;

    str = CR_FAW(skip_space)(str);
    if (*str == CR_XC('0'))
    {
        str++;
        switch (*str)
        {
#if     (TYPE_T == 16)
            case CR_XC('b'): val = CR_FAW(str2bin  )(++str, &nnn); break;
            case CR_XC('x'): val = CR_FAW(str2hex  )(++str, &nnn); break;
            default:         val = CR_FAW(str2oct  )(  str, &nnn); break;
#elif   (TYPE_T == 32)
            case CR_XC('b'): val = CR_FAW(str2bin32)(++str, &nnn); break;
            case CR_XC('x'): val = CR_FAW(str2hex32)(++str, &nnn); break;
            default:         val = CR_FAW(str2oct32)(  str, &nnn); break;
#elif   (TYPE_T == 64)
            case CR_XC('b'): val = CR_FAW(str2bin64)(++str, &nnn); break;
            case CR_XC('x'): val = CR_FAW(str2hex64)(++str, &nnn); break;
            default:         val = CR_FAW(str2oct64)(  str, &nnn); break;
#endif
        }

        if (skip != NULL)
            *skip = (leng_t)(str + nnn - ret);
        return (val);
    }

    if (CR_FAW(is_digit)(*str) ||
       (*str == CR_XC('-')) || (*str == CR_XC('+')))
    {
#if     (TYPE_T == 16)
        val = CR_FAW(str2int  )(str, &nnn);
#elif   (TYPE_T == 32)
        val = CR_FAW(str2int32)(str, &nnn);
#elif   (TYPE_T == 64)
        val = CR_FAW(str2int64)(str, &nnn);
#endif
        if (skip != NULL)
            *skip = (leng_t)(str + nnn - ret);
        return (val);
    }

#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(str, CR_XS("true"), 4) == 0 &&
#else
    if (CR_FAW(chr_cmp)(str, s_wcs_true, 4) == 0 &&
#endif
        CR_FAW(is_break)(str[4])) {
        if (skip != NULL)
            *skip = (leng_t)(str + 4 - ret);
        return (TRUE);
    }

#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(str, CR_XS("false"), 5) == 0 &&
#else
    if (CR_FAW(chr_cmp)(str, s_wcs_false, 5) == 0 &&
#endif
        CR_FAW(is_break)(str[5])) {
        if (skip != NULL)
            *skip = (leng_t)(str + 5 - ret);
        return (FALSE);
    }

    /* 非整数数值 */
    if (skip != NULL)
        *skip = 0;
    return ((UINT_T)0);
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
