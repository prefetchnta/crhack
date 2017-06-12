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
/*  >>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库解析组模板 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    生成转义字符串T
=======================================
*/
CR_API XCHAR*
CR_FUW(str_esc_make) (
  __CR_IN__ const XCHAR*    str
    )
{
    XCHAR*  nstr;
    leng_t  after = 0;
    XCHAR*  ptr = (XCHAR*)str;

    /* 统计转义后的大小 */
    while (*ptr != CR_XN(NIL)) {
        if (*ptr == CR_XC('<') || *ptr == CR_XC('>') ||
            *ptr == CR_XC('\\') || *ptr == CR_XC('\"'))
            after += 1;
        ptr   += 1;
        after += 1;
    }

    /* 分配目标大小 */
    nstr = CR_FAW(str_alloc)(after + 1);
    if (nstr == NULL)
        return (NULL);

    /* 开始复制字符串 */
    ptr = nstr;
    while (*str != CR_XN(NIL)) {
        if (*str == CR_XC('<')) {
            *ptr++ = CR_XC('\\');
            *ptr++ = CR_XC('<');
        }
        else
        if (*str == CR_XC('>')) {
            *ptr++ = CR_XC('\\');
            *ptr++ = CR_XC('>');
        }
        else
        if (*str == CR_XC('\\')) {
            *ptr++ = CR_XC('\\');
            *ptr++ = CR_XC('\\');
        }
        else
        if (*str == CR_XC('\"')) {
            *ptr++ = CR_XC('\\');
            *ptr++ = CR_XC('\"');
        }
        else {
            *ptr++ = *str;
        }
        str++;
    }
    *ptr = CR_XN(NIL);
    return (nstr);
}

/*
=======================================
    估算转义字符串的实际大小T
=======================================
*/
CR_API leng_t
CR_FUW(str_esc_size) (
  __CR_IN__ const XCHAR*    str
    )
{
    leng_t          add = 0;
    const XCHAR*    eos = str;

    if (*eos++ != CR_XC('\"'))
        return (0);

    /* UTF-32 需要特殊处理 */
    while (*eos != CR_XC('\"')) {
        if (*eos == CR_XN(NIL))
            break;
        if (*eos == CR_XC('\\')) {
            eos += 1;
            if (*eos == CR_XN(NIL))
                break;
            if (*eos == CR_XC('U'))
                add += 2;
        }
        eos += 1;
    }

    if (*eos != CR_XC('\"'))
        return (0);
    return ((leng_t)(eos - str) + add);
}

/*
=======================================
    获取原始字符串的实际大小T
=======================================
*/
CR_API leng_t
CR_FAW(str_raw_size) (
  __CR_IN__ const XCHAR*    str
    )
{
    const XCHAR*    eos = str;

#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(eos, CR_XS("\"\"\""), 3) != 0)
#else
    if (CR_FAW(chr_cmp)(eos, s_wcs_raws, 3) != 0)
#endif
        return (0);

#ifndef _CR_NO_WIDE_FUNC_
    eos = CR_FAW(str_str)(eos + 3, CR_XS("\"\"\""));
#else
    eos = CR_FAW(str_str)(eos + 3, s_wcs_raws);
#endif
    if (eos == NULL)
        return (0);
    return ((leng_t)(eos - str - 2));
}

/*
=======================================
    转义字符串转换T
=======================================
*/
CR_API leng_t
CR_FUW(str_esc_cpy) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src,
  __CR_OT__ leng_t*         skip
    )
{
    XCHAR   jj;
    int32u  vv;
    leng_t  kk, nn;
    uint_t  ii, tt;

    if (*src != CR_XC('\"'))
        return (0);

    for (kk = 0, nn = 1; src[nn] != CR_XC('\"'); nn++)
    {
        if (src[nn] == CR_XC('\\'))
        {
            tt = 8;
            switch (src[++nn])
            {
                case CR_XN(NIL):
                    return (0);

                /* 控制符 */
                case CR_XC('0'): dst[kk++] = CR_XC('\0'); break;
                case CR_XC('a'): dst[kk++] = CR_XC('\a'); break;
                case CR_XC('b'): dst[kk++] = CR_XC('\b'); break;
                case CR_XC('f'): dst[kk++] = CR_XC('\f'); break;
                case CR_XC('n'): dst[kk++] = CR_XC('\n'); break;
                case CR_XC('r'): dst[kk++] = CR_XC('\r'); break;
                case CR_XC('t'): dst[kk++] = CR_XC('\t'); break;
                case CR_XC('v'): dst[kk++] = CR_XC('\v'); break;

                /* 16进制数 */
    #if defined(_CR_BUILD_ANSI_)
                case CR_XC('x'): tt /= 2;
    #endif
                case CR_XC('u'): tt /= 2;
                case CR_XC('U'): jj = src[nn++];
                    for (vv = 0, ii = 0; ii < tt; ii++)
                    {
                        if (CR_FAW(is_digit)(src[nn]))
                        {
                            vv <<= 4;
                            vv |= (src[nn++] & 0x0F);
                        }
                        else
                        if (CR_FAW(is_xnumb)(src[nn]))
                        {
                            vv <<= 4;
                            vv |= (src[nn++] & 0x0F) + 9;
                        }
                        else
                        {
                            break;
                        }
                    }

                    /* 置数 */
    #if defined(_CR_BUILD_ANSI_)
                    if (jj == CR_XC('x'))
                        *(byte_t*)(&dst[kk]) = (byte_t)vv;
                    else
    #endif
                    if (jj == CR_XC('u'))
                        save_misali16(&dst[kk], (int16u)vv);
                    else
                        save_misali32(&dst[kk], (int32u)vv);
                    kk += tt / (2 * sizeof(XCHAR));

                    nn--;
                    break;

                default:
                    dst[kk++] = src[nn];
                    break;
            }
        }
        else if (src[nn] != CR_XC('\r') &&
                 src[nn] != CR_XC('\n'))
        {
            /* 跳过换行字符, 复制普通字符 */
            dst[kk++] = src[nn];
        }
    }

    if (skip != NULL)
        *skip = nn + 1;

    /* 结尾 */
    dst[kk] = CR_XN(NIL);
    return (kk + 1);
}

/*
=======================================
    复制原始字符串T
=======================================
*/
CR_API leng_t
CR_FAW(str_raw_cpy) (
  __CR_OT__ XCHAR*          dst,
  __CR_IN__ const XCHAR*    src,
  __CR_OT__ leng_t*         skip
    )
{
    XCHAR*          ret = dst;
    const XCHAR*    eos = src;

#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(eos, CR_XS("\"\"\""), 3) != 0)
#else
    if (CR_FAW(chr_cmp)(eos, s_wcs_raws, 3) != 0)
#endif
        return (0);

    for (eos += 3; *eos != CR_XN(NIL); *dst++ = *eos++) {
#ifndef _CR_NO_WIDE_FUNC_
        if (CR_FAW(chr_cmp)(eos, CR_XS("\"\"\""), 3) == 0)
#else
        if (CR_FAW(chr_cmp)(eos, s_wcs_raws, 3) == 0)
#endif
            break;
    }

    if (*eos == CR_XN(NIL))
        return (0);

    if (skip != NULL)
        *skip = (leng_t)(eos - src + 3);

    *dst++ = CR_XN(NIL);
    return ((leng_t)(dst - ret));
}

/*
=======================================
    新建转义字符串T
=======================================
*/
CR_API XCHAR*
CR_FUW(str_esc_dup) (
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         size,
  __CR_OT__ leng_t*         skip
    )
{
    XCHAR*  nstr;
    leng_t  nlen = CR_FUW(str_esc_size)(str);

    if (nlen == 0)
        return (NULL);

    nstr = CR_FAW(str_alloc)(nlen);
    if (nstr == NULL)
        return (NULL);

    nlen = CR_FUW(str_esc_cpy)(nstr, str, skip);
    if (size != NULL)
        *size = nlen * sizeof(XCHAR);
    return (nstr);
}

/*
=======================================
    新建原始字符串T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_raw_dup) (
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         size,
  __CR_OT__ leng_t*         skip
    )
{
    XCHAR*  nstr;
    leng_t  nlen = CR_FAW(str_raw_size)(str);

    if (nlen == 0)
        return (NULL);

    nstr = CR_FAW(str_alloc)(nlen);
    if (nstr == NULL)
        return (NULL);

    nlen = CR_FAW(str_raw_cpy)(nstr, str, skip);
    if (size != NULL)
        *size = nlen * sizeof(XCHAR);
    return (nstr);
}

/*
=======================================
    分割字符串T
=======================================
*/
CR_API XCHAR**
CR_FAW(str_split) (
  __CR_IO__ XCHAR*  str,
  __CR_IN__ XCHAR   sep,
  __CR_OT__ leng_t* count
    )
{
    XCHAR** lst;
    leng_t  idx = 1;
    leng_t  cnt = 1;
    XCHAR*  tmp = str;

    while (*tmp != CR_XN(NIL)) {
        if (*tmp++ == sep)
            cnt++;
    }

    lst = mem_talloc(cnt, XCHAR*);
    if (lst == NULL)
        return (NULL);
    lst[0] = str;

    while (*str != CR_XN(NIL)) {
        if (*str == sep) {
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
---------------------------------------
    查找空白T
---------------------------------------
*/
static XCHAR*
CR_FAW(find_space) (
  __CR_IO__ XCHAR*  str,
  __CR_IN__ bool_t  copy
    )
{
    sint_t  step = 1;
    leng_t  size = CR_FAW(str_len)(str) + 1;

    for (; *str != CR_XN(NIL); str++, size--)
    {
        /* 跳过引号里的空白 */
        if (*str == CR_XC('\"')) {
            if (copy) {
                CR_FAW(chr_cpy)(str, str + 1, size - 1);
                str--;
            }
            step *= -1;
        }
        else if (step > 0) {
            if (CR_FAW(is_space)(*str))
                break;
        }
    }
    return (str);
}

/*
=======================================
    解析命令行字符串T
=======================================
*/
CR_API XCHAR**
CR_FAW(str_cmd_split) (
  __CR_IO__ XCHAR*  str,
  __CR_OT__ uint_t* count
    )
{
    XCHAR** lst;
    XCHAR*  ptr = str;
    uint_t  idx, num = 0;

    /* 删除尾部空白 */
    CR_FAW(str_trimR)(str);

    /* 统计参数个数 */
    while (*ptr != CR_XN(NIL)) {
        ptr = CR_FAW(skip_space)(ptr);
        if (*ptr == CR_XN(NIL))
            break;
        num += 1;
        ptr = CR_FAW(find_space)(ptr, FALSE);
    }
    if (count != NULL)
        *count = num;
    if (num == 0)
        return (NULL);

    /* 提取各个参数 */
    lst = mem_talloc(num, XCHAR*);
    if (lst == NULL)
        return (NULL);
    idx = 0;
    while (idx < num) {
        str = CR_FAW(skip_space)(str);
        lst[idx++] = str;
        str = CR_FAW(find_space)(str, TRUE);
        *str++ = CR_XN(NIL);
    }
    return (lst);
}

/*
=======================================
    清空所有字符串内容T (支持转义字符)
=======================================
*/
CR_API XCHAR*
CR_FUW(str_esc_empty) (
  __CR_IO__ XCHAR*  str
    )
{
    sint_t  step = 1;
    XCHAR*  temp = str;

    while (*temp != CR_XN(NIL))
    {
        if (*temp == CR_XC('\"'))
        {
            step *= -1;
        }
        else if (step < 0)
        {
            /* 跳过转义 */
            if (*temp == CR_XC('\\'))
                *temp++ = CR_XC(' ');

            /* 存在非法串 */
            if (*temp == CR_XN(NIL))
                return (NULL);
            *temp = CR_XC(' ');
        }
        temp++;
    }

    if (step < 0)
        return (NULL);
    return (str);
}

/*
=======================================
    跳过空白T
=======================================
*/
CR_API XCHAR*
CR_FAW(skip_space) (
  __CR_IN__ const XCHAR*    str
    )
{
    while (CR_FAW(is_space)(*str))
        str++;
    return ((XCHAR*)str);
}

/*
=======================================
    跳过 C/C++ 注释T
=======================================
*/
CR_API XCHAR*
CR_FAW(skip_cpp_cmt) (
  __CR_IN__ const XCHAR*    str
    )
{
    if (str[0] != CR_XC('/'))
        return ((XCHAR*)str);

    if (str[1] == CR_XC('/'))
    {
        /* 跳到行尾 */
        str = CR_FAW(str_chr2)(str + 2, CR_XC('\r'), CR_XC('\n'));
        if (str == NULL)
            return (NULL);
        return ((XCHAR*)str + 1);
    }
    else
    if (str[1] == CR_XC('*'))
    {
        /* 跳到块尾 */
#ifndef _CR_NO_WIDE_FUNC_
        str = CR_FAW(str_str)(str + 2, CR_XS("*/"));
#else
        str = CR_FAW(str_str)(str + 2, s_wcs_ccmte);
#endif
        if (str == NULL)
            return (NULL);
        return ((XCHAR*)str + 2);
    }
    return ((XCHAR*)str);
}

/*
=======================================
    跳过 Html/Xml 注释T
=======================================
*/
CR_API XCHAR*
CR_FAW(skip_xml_cmt) (
  __CR_IN__ const XCHAR*    str
    )
{
#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(str, CR_XS("<!--"), 4) == 0)
        str = CR_FAW(str_str)(str + 4, CR_XS("-->"));
    else
    if (CR_FAW(chr_cmp)(str, CR_XS("<![CDATA["), 9) == 0)
        str = CR_FAW(str_str)(str + 9, CR_XS("]]>"));
#else
    if (CR_FAW(chr_cmp)(str, s_wcs_xcmts1, 4) == 0)
        str = CR_FAW(str_str)(str + 4, s_wcs_xcmte1);
    else
    if (CR_FAW(chr_cmp)(str, s_wcs_xcmts2, 9) == 0)
        str = CR_FAW(str_str)(str + 9, s_wcs_xcmte2);
#endif
    else
        return ((XCHAR*)str);

    if (str == NULL)
        return (NULL);
    return ((XCHAR*)str + 3);
}

/*
=======================================
    移除 Html/Xml 注释T
=======================================
*/
CR_API XCHAR*
CR_FAW(remove_xml_cmt) (
  __CR_IO__ XCHAR*  str
    )
{
    uint_t  idx;
    XCHAR*  ret = str;

    str = CR_FAW(skip_space)(str);

#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(str, CR_XS("<!--"), 4) == 0)
#else
    if (CR_FAW(chr_cmp)(str, s_wcs_xcmts1, 4) == 0)
#endif
    {
        for (idx = 4; idx != 0; idx--)
            *str++ = CR_XC(' ');

#ifndef _CR_NO_WIDE_FUNC_
        str = CR_FAW(str_str)(str, CR_XS("-->"));
#else
        str = CR_FAW(str_str)(str, s_wcs_xcmte1);
#endif
    }
    else
#ifndef _CR_NO_WIDE_FUNC_
    if (CR_FAW(chr_cmp)(str, CR_XS("<![CDATA["), 9) == 0)
#else
    if (CR_FAW(chr_cmp)(str, s_wcs_xcmts2, 9) == 0)
#endif
    {
        for (idx = 9; idx != 0; idx--)
            *str++ = CR_XC(' ');

#ifndef _CR_NO_WIDE_FUNC_
        str = CR_FAW(str_str)(str, CR_XS("]]>"));
#else
        str = CR_FAW(str_str)(str, s_wcs_xcmte2);
#endif
    }
    else
    {
        return (ret);
    }

    for (idx = 3; idx != 0; idx--)
        *str++ = CR_XC(' ');
    return (ret);
}

/*
=======================================
    获取行号T
=======================================
*/
CR_API leng_t
CR_FAW(str_get_line) (
  __CR_IN__ const XCHAR*    pos,
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ leng_t*         cols
    )
{
    leng_t  col = 1, row = 1;

    while (*str != CR_XN(NIL) && str != pos)
    {
        if (*str == CR_XC('\n')) {
            row++;
            col = 0;
        }
        else
        if (*str == CR_XC('\r')) {
            if (str[1] == CR_XC('\n')) {
                str++; col++;
                if (str == pos)
                    break;
            }
            row++;
            col = 0;
        }
        str++;
        col++;
    }

    /* 返回0表示未找到 */
    if (*str == CR_XN(NIL))
        return (0);

    /* 返回行号列号 */
    if (cols != NULL)
        *cols = col;
    return (row);
}

/*
=======================================
    解析 URL 字符串T
=======================================
*/
CR_API XCHAR*
CR_FAW(str_url_split) (
  __CR_IN__ const XCHAR*    url,
  __CR_OT__ XCHAR**         scheme,
  __CR_OT__ XCHAR**         username,
  __CR_OT__ XCHAR**         password,
  __CR_OT__ XCHAR**         host,
  __CR_OT__ uint_t*         port,
  __CR_OT__ XCHAR**         path,
  __CR_OT__ XCHAR**         params,
  __CR_OT__ XCHAR**         flags
    )
{
    XCHAR*  str;
    XCHAR*  ptr;
    XCHAR*  tmp1;
    XCHAR*  tmp2;

    /* 复制一份副本 */
    str = CR_FAW(str_dup)(url);
    if (str == NULL)
        return (NULL);

    /* 查找协议头 */
#ifndef _CR_NO_WIDE_FUNC_
    ptr = CR_FAW(str_str)(str, CR_XS("://"));
#else
    ptr = CR_FAW(str_str)(str, s_wcs_scheme);
#endif
    if (ptr == NULL) {
        if (scheme != NULL)
            *scheme = NULL;
        ptr = str;
    }
    else {
        if (scheme != NULL)
            *scheme = str;
        *ptr = CR_XN(NIL);
        ptr += 3;
    }

    /* 查找用户名 & 密码 */
    tmp1 = CR_FAW(str_chr)(ptr, CR_XC('@'));
    if (tmp1 == NULL) {
        if (username != NULL)
            *username = NULL;
        if (password != NULL)
            *password = NULL;
    }
    else {
        if (username != NULL)
            *username = ptr;
        *tmp1 = CR_XN(NIL);
        tmp2 = CR_FAW(str_chr)(ptr, CR_XC(':'));
        ptr = tmp1 + 1;
        if (tmp2 == NULL) {
            if (password != NULL)
                *password = NULL;
        }
        else {
            if (password != NULL)
                *password = tmp2 + 1;
            *tmp2 = CR_XN(NIL);
        }
    }

    /* 查找主机 & 端口号 */
    if (host != NULL)
        *host = ptr;
    tmp1 = CR_FAW(str_chr)(ptr, CR_XC('/'));
    if (tmp1 == NULL) {
        if (path != NULL)
            *path = NULL;
        if (params != NULL)
            *params = NULL;
        if (flags != NULL)
            *flags = NULL;
    }
    else {
        *tmp1 = CR_XN(NIL);
    }
    tmp2 = CR_FAW(str_chr)(ptr, CR_XC(':'));
    if (tmp2 == NULL) {
        if (port != NULL)
            *port = 0;
    }
    else {
        if (port != NULL)
            *port = CR_FAW(str2intx)(tmp2 + 1, NULL);
        *tmp2 = CR_XN(NIL);
    }

    /* 没有后续参数直接结束 */
    if (tmp1 == NULL)
        return (str);
    ptr = tmp1 + 1;

    /* 查找参数 */
    if (path != NULL)
        *path = ptr;
    tmp1 = CR_FAW(str_chr)(ptr, CR_XC('?'));
    if (tmp1 == NULL) {
        if (params != NULL)
            *params = NULL;
    }
    else {
        if (params != NULL)
            *params = tmp1 + 1;
        ptr = tmp1 + 1;
        *tmp1 = CR_XN(NIL);
    }

    /* 查找标志 */
    tmp1 = CR_FAW(str_chr)(ptr, CR_XC('#'));
    if (tmp1 == NULL) {
        if (flags != NULL)
            *flags = NULL;
    }
    else {
        if (flags != NULL)
            *flags = tmp1 + 1;
        *tmp1 = CR_XN(NIL);
    }
    return (str);
}

/*
=======================================
    解析 IPv4 字符串T
=======================================
*/
CR_API bool_t
CR_FAW(str2ip4addr) (
  __CR_IN__ const XCHAR*    str,
  __CR_OT__ byte_t*         val
    )
{
    leng_t  skip;
    sint_t  idx, ip[4];

    /* 解析三个数字 */
    for (idx = 0; idx < 3; idx++) {
        ip[idx] = CR_FAW(str2int)(str, &skip);
        if (skip == 0 || skip > 3)
            return (FALSE);
        if (ip[idx] < 0 || ip[idx] > 255)
            return (FALSE);
        str += skip;
        if (*str != CR_XC('.'))
            return (FALSE);
        str += 1;
    }

    /* 解析第四个数字 */
    ip[idx] = CR_FAW(str2int)(str, &skip);
    if (skip == 0 || skip > 3)
        return (FALSE);
    if (ip[idx] < 0 || ip[idx] > 255)
        return (FALSE);
    str += skip;
    if (*str != CR_XN(NIL))
        return (FALSE);

    /* 返回 IPv4 地址 */
    if (val != NULL) {
        for (idx = 0; idx < 4; idx++)
            val[idx] = (byte_t)(ip[idx] & 0xFF);
    }
    return (TRUE);
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
