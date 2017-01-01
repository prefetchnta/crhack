/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-06  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库基本组模板 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    串大写T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_upr) (
  __CR_IO__ XCHAR*  str
    )
{
    XCHAR*  ret = str;

    while (*str != CR_XN(NIL)) {
        if (CR_FAW(is_lower)(*str))
            *str -= CR_XC('a') - CR_XC('A');
        str++;
    }
    return (ret);
}

/*
=======================================
    串小写T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_lwr) (
  __CR_IO__ XCHAR*  str
    )
{
    XCHAR*  ret = str;

    while (*str != CR_XN(NIL)) {
        if (CR_FAW(is_upper)(*str))
            *str += CR_XC('a') - CR_XC('A');
        str++;
    }
    return (ret);
}

/*
=======================================
    串翻转T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_flp) (
  __CR_IO__ XCHAR*  str
    )
{
    XCHAR   tmp;
    leng_t  idx;
    leng_t  len;

    len = CR_FAW(str_len)(str);
    for (idx = 0; idx < len / 2; idx++)
        CR_SWAP(str[idx], str[len - idx - 1], tmp);
    return (str);
}

/*
=======================================
    串去头T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_trimL) (
  __CR_IO__ XCHAR*  str
    )
{
    XCHAR*  hdr;

    hdr = CR_FAW(skip_space)(str);
    if (hdr != str)
        CR_FAW(str_cpy)(str, hdr);
    return (str);
}

/*
=======================================
    串去尾T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_trimR) (
  __CR_IO__ XCHAR*  str
    )
{
    leng_t  len;

    len = CR_FAW(str_len)(str);
    while (len != 0) {
        len -= 1;
        if (!CR_FAW(is_space)(str[len]))
            break;
        str[len] = CR_XN(NIL);
    }
    return (str);
}

/*
=======================================
    串去头尾T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_trim) (
  __CR_IO__ XCHAR*  str
    )
{
    CR_FAW(str_trimR)(str);
    CR_FAW(str_trimL)(str);
    return (str);
}

/*
=======================================
    串查找T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_str) (
  __CR_IN__ const XCHAR*    str1,
  __CR_IN__ const XCHAR*    str2
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strstr(str1, str2));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcsstr(str1, str2));

#else
    XCHAR*  t1;
    XCHAR*  t2;
    XCHAR*  cp;

    if (*str2 == CR_XN(NIL))
        return ((XCHAR*)str1);

    cp = (XCHAR*)str1;
    while (*cp != CR_XN(NIL))
    {
        t1 = cp;
        t2 = (XCHAR*)str2;
        while (*t1 != CR_XN(NIL) && *t2 != CR_XN(NIL) && *t1 == *t2)
            ++t1, ++t2;

        if (*t2 == CR_XN(NIL))
            return (cp);
        cp++;
    }
    return (NULL);
#endif
}

/*
=======================================
    串查找T (忽略大小写)
=======================================
*/
CR_API XCHAR*
CR_FAW(str_strI) (
  __CR_IN__ const XCHAR*    str1,
  __CR_IN__ const XCHAR*    str2
    )
{
    XCHAR*  t1;
    XCHAR*  t2;
    XCHAR*  cp;
    XCHAR   ff, ll;

    if (*str2 == CR_XN(NIL))
        return ((XCHAR*)str1);

    cp = (XCHAR*)str1;
    while (*cp != CR_XN(NIL))
    {
        t1 = cp;
        t2 = (XCHAR*)str2;
        while (*t1 != CR_XN(NIL) && *t2 != CR_XN(NIL))
        {
            ff = *t1;
            ll = *t2;
            if (CR_FAW(is_upper)(ff))
                ff += CR_XC('a') - CR_XC('A');
            if (CR_FAW(is_upper)(ll))
                ll += CR_XC('a') - CR_XC('A');

            if (ff != ll)
                break;
            t1++; t2++;
        }

        if (*t2 == CR_XN(NIL))
            return (cp);
        cp++;
    }
    return (NULL);
}

/*
=======================================
    串复制T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_cpy) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strcpy(dst, src));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcscpy(dst, src));

#else
    XCHAR*  ret = dst;

    while ((*dst++ = *src++) != CR_XN(NIL));

    return (ret);
#endif
}

/*
=======================================
    串定长复制T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_cpyN) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src,
  __CR_IN__ leng_t          len
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strncpy(dst, src, len));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcsncpy(dst, src, len));

#else
    XCHAR*  ret = dst;

    while (len != 0 &&
          (*dst++ = *src++) != CR_XN(NIL))
        len--;

    if (len != 0) {
        while (--len != 0)
            *dst++ = CR_XN(NIL);
    }
    return (ret);
#endif
}

/*
=======================================
    串连接T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_cat) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strcat(dst, src));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcscat(dst, src));

#else
    XCHAR*  ret = dst;

    while (*dst != CR_XN(NIL))
        dst++;

    while ((*dst++ = *src++) != CR_XN(NIL));

    return (ret);
#endif
}

/*
=======================================
    串定长连接T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_catN) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src,
  __CR_IN__ leng_t          len
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strncat(dst, src, len));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcsncat(dst, src, len));

#else
    XCHAR*  ret = dst;

    while (*dst != CR_XN(NIL))
        dst++;

    for (; len != 0; len--) {
        if ((*dst++ = *src++) != CR_XN(NIL))
            return (ret);
    }
    *dst = CR_XN(NIL);
    return (ret);
#endif
}

/*
=======================================
    串比较T
=======================================
*/
CR_API sint_t
CR_FAW(str_cmp) (
  __CR_IN__ const XCHAR*    str1,
  __CR_IN__ const XCHAR*    str2
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strcmp(str1, str2));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcscmp(str1, str2));

#else
    while (*str1 == *str2 && *str2 != CR_XN(NIL))
        ++str1, ++str2;
    return (*str1 - *str2);
#endif
}

/*
=======================================
    串定长比较T
=======================================
*/
CR_API sint_t
CR_FAW(str_cmpN) (
  __CR_IN__ const XCHAR*    str1,
  __CR_IN__ const XCHAR*    str2,
  __CR_IN__ leng_t          len
    )
{
#if    !defined(_CR_NO_STDC_) && \
        defined(_CR_BUILD_ANSI_)
    return (strncmp(str1, str2, len));

#elif   defined(_CR_HAVE_WCS_) && \
        defined(_CR_BUILD_WIDE_)
    return (wcsncmp(str1, str2, len));

#else
    for (; len != 0; len--) {
        if (*str1 == CR_XN(NIL) || *str1 != *str2)
            return (*str1 - *str2);
        str1++; str2++;
    }
    return (0x0000);
#endif
}

/*
=======================================
    串比较T (忽略大小写)
=======================================
*/
CR_API sint_t
CR_FAW(str_cmpI) (
  __CR_IN__ const XCHAR*    str1,
  __CR_IN__ const XCHAR*    str2
    )
{
    XCHAR   ff, ll;

    do {
        ff = *str1++;
        ll = *str2++;
        if (CR_FAW(is_upper)(ff))
            ff += CR_XC('a') - CR_XC('A');
        if (CR_FAW(is_upper)(ll))
            ll += CR_XC('a') - CR_XC('A');
    } while (ff != CR_XN(NIL) && ff == ll);

    return (ff - ll);
}

/*
=======================================
    查找一个字符T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_chr) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ XCHAR           ch
    )
{
    while (*str != CR_XN(NIL) && *str != ch)
        str++;
    return ((*str == ch) ? (XCHAR*)str : NULL);
}

/*
=======================================
    查找两个字符T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_chr2) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ XCHAR           ch1,
  __CR_IN__ XCHAR           ch2
    )
{
    while (*str != CR_XN(NIL) && *str != ch1 && *str != ch2)
        str++;
    return ((*str == ch1 || *str == ch2) ? (XCHAR*)str : NULL);
}

/*
=======================================
    查找一个字符T (支持转义字符)
=======================================
*/
CR_API XCHAR*
CR_FUW(str_esc_chr) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ XCHAR           ch
    )
{
    while (*str != CR_XN(NIL) && *str != ch)
    {
        /* 跳过转义 */
        if (*str++ == CR_XC('\\'))
            if (*str++ == CR_XC('\0'))
                return (NULL);
    }
    return ((*str == ch) ? (XCHAR*)str : NULL);
}

/*
=======================================
    查找两个字符T (支持转义字符)
=======================================
*/
CR_API XCHAR*
CR_FUW(str_esc_chr2) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ XCHAR           ch1,
  __CR_IN__ XCHAR           ch2
    )
{
    while (*str != CR_XN(NIL) && *str != ch1 && *str != ch2)
    {
        /* 跳过转义 */
        if (*str++ == CR_XC('\\'))
            if (*str++ == CR_XC('\0'))
                return (NULL);
    }
    return ((*str == ch1 || *str == ch2) ? (XCHAR*)str : NULL);
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
