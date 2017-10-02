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
    整数转二进制串T
=======================================
*/
CR_API XCHAR*
#if     (TYPE_T == 16)
CR_FAW(int2bstr) (
#elif   (TYPE_T == 32)
CR_FAW(int2bstr32) (
#elif   (TYPE_T == 64)
CR_FAW(int2bstr64) (
#endif
  __CR_OT__ XCHAR   name[65],
  __CR_IN__ UINT_T  value
    )
{
    XCHAR   tmp;
    ufast_t idx, num, cnt = 0;

    do {
        name[cnt++] = s_hex2asc[value & 1];
        value >>= 1;
    } while (value != 0);

    name[cnt] = 0x00;
    num = cnt / 2;

    for (idx = 0; idx < num; idx++) {
        tmp = name[idx];
        name[idx] = name[cnt - 1 - idx];
        name[cnt - 1 - idx] = tmp;
    }
    return (name);
}

/*
=======================================
    整数转八进制串T
=======================================
*/
CR_API XCHAR*
#if     (TYPE_T == 16)
CR_FAW(int2ostr) (
#elif   (TYPE_T == 32)
CR_FAW(int2ostr32) (
#elif   (TYPE_T == 64)
CR_FAW(int2ostr64) (
#endif
  __CR_OT__ XCHAR   name[23],
  __CR_IN__ UINT_T  value
    )
{
    XCHAR   tmp;
    ufast_t idx, num, cnt = 0;

    do {
        name[cnt++] = s_hex2asc[value & 7];
        value >>= 3;
    } while (value != 0);

    name[cnt] = 0x00;
    num = cnt / 2;

    for (idx = 0; idx < num; idx++) {
        tmp = name[idx];
        name[idx] = name[cnt - 1 - idx];
        name[cnt - 1 - idx] = tmp;
    }
    return (name);
}

/*
=======================================
    整数转十进制串T
=======================================
*/
CR_API XCHAR*
#if     (TYPE_T == 16)
CR_FAW(int2dstr) (
#elif   (TYPE_T == 32)
CR_FAW(int2dstr32) (
#elif   (TYPE_T == 64)
CR_FAW(int2dstr64) (
#endif
  __CR_OT__ XCHAR   name[21],
  __CR_IN__ UINT_T  value,
  __CR_IN__ bool_t  sign
    )
{
    XCHAR   tmp;
    ufast_t idx, num, cnt = 0;

    if (sign) {
        if ((SINT_T)value < 0)
            value = 0 - value;
        else
            sign = FALSE;
    }

    do {
        name[cnt++] = s_hex2asc[value % 10];
        value /= 10;
    } while (value != 0);

    if (sign)
        name[cnt++] = CR_XC('-');
    name[cnt] = 0x00;
    num = cnt / 2;

    for (idx = 0; idx < num; idx++) {
        tmp = name[idx];
        name[idx] = name[cnt - 1 - idx];
        name[cnt - 1 - idx] = tmp;
    }
    return (name);
}

/*
=======================================
    整数转16进制串T
=======================================
*/
CR_API XCHAR*
#if     (TYPE_T == 16)
CR_FAW(int2hstr) (
#elif   (TYPE_T == 32)
CR_FAW(int2hstr32) (
#elif   (TYPE_T == 64)
CR_FAW(int2hstr64) (
#endif
  __CR_OT__ XCHAR   name[17],
  __CR_IN__ UINT_T  value
    )
{
    XCHAR   tmp;
    ufast_t idx, num, cnt = 0;

    do {
        name[cnt++] = s_hex2asc[value & 15];
        value >>= 4;
    } while (value != 0);

    name[cnt] = 0x00;
    num = cnt / 2;

    for (idx = 0; idx < num; idx++) {
        tmp = name[idx];
        name[idx] = name[cnt - 1 - idx];
        name[cnt - 1 - idx] = tmp;
    }
    return (name);
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
