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
/*  >>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库文件组模板 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    路径名统一T
=======================================
*/
CR_API XCHAR*
CR_FAW(path_unique) (
  __CR_IO__ XCHAR*  str
    )
{
    XCHAR*  ret = str;

    while (*str != CR_XN(NIL)) {
        if (CR_FAW(is_slash)(*str))
            *str = CR_XN(CR_SLASH);
        str++;
    }
    return (ret);
}

/*
=======================================
    分割文件名路径T
=======================================
*/
CR_API XCHAR**
CR_FAW(path_split) (
  __CR_IO__ XCHAR*  str,
  __CR_OT__ uint_t* count
    )
{
    XCHAR** lst;
    uint_t  idx = 1;
    uint_t  cnt = 1;
    XCHAR*  tmp = str;

    /* 计算目录个数 */
    while (*tmp != CR_XN(NIL)) {
        if (CR_FAW(is_slash)(*tmp))
            cnt++;
        tmp++;
    }

    lst = mem_talloc(cnt, XCHAR*);
    if (lst == NULL)
        return (NULL);
    lst[0] = str;

    while (*str != CR_XN(NIL)) {
        if (CR_FAW(is_slash)(*str)) {
            *str = CR_XN(NIL);
            lst[idx++] = str + 1;
        }
        str++;
    }

    if (count != NULL)
        *count = cnt;
    return ((XCHAR**)lst);
}

/*
=======================================
    添加路径名T
=======================================
*/
CR_API XCHAR*
CR_FAW(path_append) (
  __CR_IN__ const XCHAR*    path1,
  __CR_IN__ const XCHAR*    path2
    )
{
    XCHAR*  temp;
    leng_t  leng;
    leng_t  n1 = CR_FAW(str_len)(path1);
    leng_t  n2 = CR_FAW(str_len)(path2);
    bool_t  ended = TRUE;

    leng = n1 + n2 + 1;
    if (n1 != 0 &&
       !CR_FAW(is_slash)(path1[n1 - 1])) {
        leng++;
        ended = FALSE;
    }

    temp = CR_FAW(str_alloc)(leng);
    if (temp == NULL)
        return (NULL);

    CR_FAW(chr_cpy)(temp, path1, n1);
    if (!ended)
        temp[n1++] = CR_XN(CR_SLASH);
    CR_FAW(chr_cpy)(temp + n1, path2, n2 + 1);
    return (temp);
}

/*
=======================================
    抽出文件路径T (包括最后的斜杠)
=======================================
*/
CR_API XCHAR*
CR_FAW(path_extract) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src
    )
{
    XCHAR*  ret = dst;
    leng_t  len = CR_FAW(str_len)(src);

    for (; len != 0; len--) {
        if (CR_FAW(is_slash)(src[len - 1]))
            break;
    }

    for (; len != 0; len--)
        *dst++ = *src++;

    *dst = CR_XN(NIL);
    return (ret);
}

/*
=======================================
    文件名统一T
=======================================
*/
CR_API XCHAR*
CR_FAW(flname_unique) (
  __CR_IO__ XCHAR*  str
    )
{
    XCHAR*  ret = str;

    while (*str != CR_XN(NIL)) {
        if (CR_FAW(is_slash)(*str))
            *str = CR_XN(CR_SLASH);
        else
        if (CR_FAW(is_upper)(*str))
            *str += CR_XC('a') - CR_XC('A');
        str++;
    }
    return (ret);
}

/*
=======================================
    抽出文件名和扩展名T
=======================================
*/
CR_API XCHAR*
CR_FAW(flname_extract) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src
    )
{
    leng_t  len = CR_FAW(str_len)(src);

    for (; len != 0; len--) {
        if (CR_FAW(is_slash)(src[len - 1]))
            break;
    }
    return (CR_FAW(str_cpy)(dst, src + len));
}

/*
=======================================
    文件名比较T
=======================================
*/
CR_API sint_t
CR_FAW(flname_compare) (
  __CR_IN__ const XCHAR*    str1,
  __CR_IN__ const XCHAR*    str2
    )
{
    XCHAR   ff, ll;

    do {
        ff = *str1++;
        ll = *str2++;

        if (CR_FAW(is_slash)(ff))
            ff = CR_XN(CR_SLASH);
        else
        if (CR_FAW(is_upper)(ff))
            ff += CR_XC('a') - CR_XC('A');

        if (CR_FAW(is_slash)(ll))
            ll = CR_XN(CR_SLASH);
        else
        if (CR_FAW(is_upper)(ll))
            ll += CR_XC('a') - CR_XC('A');
    }
    while (ff != CR_XN(NIL) && ff == ll);

    return (ff - ll);
}

/*
=======================================
    去除文件扩展名T
=======================================
*/
CR_API XCHAR*
CR_FAW(filext_remove) (
  __CR_IO__ XCHAR*  str
    )
{
    leng_t  len = CR_FAW(str_len)(str);

    while (len-- != 0) {
        if (str[len] == CR_XC('.')) {
            str[len] = CR_XN(NIL);
            break;
        }
    }
    return (str);
}

/*
=======================================
    改变文件扩展名T (带点)
=======================================
*/
CR_API XCHAR*
CR_FAW(filext_change) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src,
  __CR_IN__ const XCHAR*    ext
    )
{
    XCHAR*  ret = dst;
    leng_t  len = CR_FAW(str_len)(src);

    for (; len != 0; len--) {
        if (src[len - 1] == CR_XC('.'))
            break;
    }

    if (len == 0) {
        CR_FAW(str_cpy)(dst, src);
    }
    else {
        while (--len != 0)
            *dst++ = *src++;
        *dst = CR_XN(NIL);
    }
    return (CR_FAW(str_cat)(ret, ext));
}

/*
=======================================
    抽出文件扩展名T (包括点)
=======================================
*/
CR_API XCHAR*
CR_FAW(filext_extract) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src
    )
{
    leng_t  len = CR_FAW(str_len)(src);

    for (; len != 0; len--) {
        if (src[len - 1] == CR_XC('.'))
            break;
    }

    if (len != 0)
        return (CR_FAW(str_cpy)(dst, src + len - 1));

    *dst = CR_XN(NIL);
    return (dst);
}

/*
=======================================
    检测文件扩展名T (带点)
=======================================
*/
CR_API bool_t
CR_FAW(filext_check) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR*    ext
    )
{
    leng_t  len = CR_FAW(str_len)(str);

    while (len-- != 0)
    {
        if (str[len] == CR_XC('.')) {
            if (ext == NULL)
                return (FALSE);
            return (CR_FAW(str_cmpI)(str + len, ext) == 0);
        }
    }
    return ((ext == NULL) ? TRUE : FALSE);
}

/*
=======================================
    字符串通配符匹配T
=======================================
*/
CR_API bool_t
CR_FAW(wildcard_match) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR*    pat
    )
{
    while (*str != CR_XN(NIL) && *pat != CR_XN(NIL))
    {
        if (*pat == CR_XC('?')) {
            if (CR_FAW(wildcard_match)(str, pat + 1))
                return (TRUE);
        }
        else
        if (*pat == CR_XC('*')) {
            while (*pat == CR_XC('*') || *pat == CR_XC('?'))
                pat++;

            if (*pat == CR_XN(NIL))
                return (TRUE);

            while (*str != CR_XN(NIL)) {
                if (CR_FAW(wildcard_match)(str, pat))
                    return (TRUE);
                str++;
            }
            return (FALSE);
        }
        else if (*pat != *str) {
            return (FALSE);
        }
        str++; pat++;
    }

    if (*str != CR_XN(NIL))
        return (FALSE);

    while (*pat == CR_XC('*') || *pat == CR_XC('?'))
        pat++;
    return ((*pat == CR_XN(NIL)) ? TRUE : FALSE);
}

/*
=======================================
    字符串通配符匹配T (忽略大小写)
=======================================
*/
CR_API bool_t
CR_FAW(wildcard_matchI) (
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR*    pat
    )
{
    XCHAR   ff, ll;

    while (*str != CR_XN(NIL) && *pat != CR_XN(NIL))
    {
        if (*pat == CR_XC('?')) {
            if (CR_FAW(wildcard_matchI)(str, pat + 1))
                return (TRUE);
        }
        else
        if (*pat == CR_XC('*')) {
            while (*pat == CR_XC('*') || *pat == CR_XC('?'))
                pat++;

            if (*pat == CR_XN(NIL))
                return (TRUE);

            while (*str != CR_XN(NIL)) {
                if (CR_FAW(wildcard_matchI)(str, pat))
                    return (TRUE);
                str++;
            }
            return (FALSE);
        }
        else {
            ff = *str;
            ll = *pat;
            if (CR_FAW(is_upper)(ff))
                ff += CR_XC('a') - CR_XC('A');
            if (CR_FAW(is_upper)(ll))
                ll += CR_XC('a') - CR_XC('A');

            if (ff != ll)
                return (FALSE);
        }
        str++; pat++;
    }

    if (*str != CR_XN(NIL))
        return (FALSE);

    while (*pat == CR_XC('*') || *pat == CR_XC('?'))
        pat++;
    return ((*pat == CR_XN(NIL)) ? TRUE : FALSE);
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
