/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack INI 解析器函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    生成 INI 解析器T
=======================================
*/
CR_API sINIx*
CR_FUW(ini_parse) (
  __CR_IN__ const XCHAR*    str
    )
{
    sINIx   ini;
    sINIx*  ret;
    XCHAR*  tmp = (XCHAR*)str;

    /* 计算行数 */
    for (ini.count = 1; *tmp != CR_XN(NIL); tmp++) {
        if (*tmp == CR_XC('\n')) {
            ini.count++;
        }
        else
        if (*tmp == CR_XC('\r')) {
            if (tmp[1] == CR_XC('\n'))
                tmp++;
            ini.count++;
        }
    }

    /* 分配内存 */
    ini.text = tmp = CR_FAW(str_dup)(str);
    if (ini.text == NULL)
        return (NULL);
    ini.lines = mem_talloc(ini.count, XCHAR*);
    if (ini.lines == NULL) {
        mem_free(ini.text);
        return (NULL);
    }

    /* 解析文本 */
    ini.lines[0] = ini.text;

    for (ini.count = 1; *tmp != CR_XN(NIL); tmp++) {
        if (*tmp == CR_XC('\n')) {
            *tmp = CR_XN(NIL);
            ini.lines[ini.count++] = tmp + 1;
        }
        else
        if (*tmp == CR_XC('\r')) {
            *tmp = CR_XN(NIL);
            if (tmp[1] == CR_XC('\n')) {
                tmp += 1;
                *tmp = CR_XN(NIL);
            }
            ini.lines[ini.count++] = tmp + 1;
        }
    }

    ret = struct_dup(&ini, sINIx);
    if (ret == NULL) {
        mem_free(ini.lines);
        mem_free(ini.text);
    }
    return (ret);
}

/*
=======================================
    释放 INI 解析器T
=======================================
*/
CR_API void_t
CR_FUW(ini_close) (
  __CR_IN__ sINIx*  ini
    )
{
    mem_free(ini->lines);
    mem_free(ini->text);
    mem_free(ini);
}

/*
=======================================
    从文本生成 INI 解析器T
=======================================
*/
CR_API sINIx*
CR_FUW(ini_from_text) (
  __CR_IN__ const ansi_t*   text
    )
{
    sINIx*  ini;
    XCHAR*  str;

#if defined(_CR_BUILD_ANSI_)
    str = text_to_utf8(text, NULL, FALSE);
#else   /* (_CR_BUILD_WIDE_) */
    str = text_to_utf16(text, NULL, FALSE);
#endif
    if (str == NULL)
        return (NULL);
    ini = CR_FUW(ini_parse)(str);
    mem_free(str);
    return (ini);
}

/*
=======================================
    从 INI 键读取整数T
=======================================
*/
CR_API uint_t
CR_FUW(ini_key_intx) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ uint_t          def,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx)(tmp, NULL));
}

/*
=======================================
    从 INI 键读取32位整数T
=======================================
*/
CR_API int32u
CR_FUW(ini_key_intx32) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ int32u          def,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx32)(tmp, NULL));
}

/*
=======================================
    从 INI 键读取64位整数T
=======================================
*/
CR_API int64u
CR_FUW(ini_key_intx64) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ int64u          def,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx64)(tmp, NULL));
}

/*
=======================================
    从 INI 键读取实数T
=======================================
*/
CR_API real_t
CR_FUW(ini_key_real) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ real_t          def,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2real)(tmp, NULL));
}

/*
=======================================
    从 INI 键读取32位实数T
=======================================
*/
CR_API fp32_t
CR_FUW(ini_key_fp32) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ fp32_t          def,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2fp32)(tmp, NULL));
}

/*
=======================================
    从 INI 键读取64位实数T
=======================================
*/
CR_API fp64_t
CR_FUW(ini_key_fp64) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ fp64_t          def,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2fp64)(tmp, NULL));
}

/*
=======================================
    从 INI 键读取转义字符串T
=======================================
*/
CR_API XCHAR*
CR_FUW(ini_key_string) (
  __CR_IN__ const XCHAR*    name,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(ini_key_buffer)(name, ini);
    if (tmp == NULL)
        return (NULL);
    tmp = CR_FAW(skip_space)(tmp);
    return (CR_FUW(str_esc_dup)(tmp, &ini->size, NULL));
}

/*
=======================================
    从 INI 键读取指针T
=======================================
*/
CR_API XCHAR*
CR_FUW(ini_key_buffer) (
  __CR_IN__ const XCHAR*    name,
  __CR_IO__ sINIx*          ini
    )
{
    XCHAR*  tmp;
    leng_t  idx, len = CR_FAW(str_len)(name);

    for (idx = 0; idx < ini->count; idx++)
    {
        /* 键名有效性检查 */
        tmp = CR_FAW(skip_space)(ini->lines[idx]);
        if (CR_FAW(chr_cmp)(tmp, name, len) == 0)
        {
            tmp = CR_FAW(skip_space)(tmp + len);
            if (*tmp == CR_XC('='))
            {
                /* 返回等号后面的指针 */
                ini->found = TRUE;
                return (tmp + 1);
            }
        }
    }
    ini->found = FALSE;
    return (NULL);
}

/*
=======================================
    检查字符串是否合法T
=======================================
*/
CR_API bool_t
CR_FUW(ini_check_string) (
  __CR_IN__ const XCHAR*    str
    )
{
    leng_t  idx;
    leng_t  len = CR_FAW(str_len)(str);

    for (idx = 0; idx < len; idx++)
    {
        if (str[idx] == CR_XC('\\'))
            idx++;
        else
        if (str[idx] == CR_XC('\"'))
            return (FALSE);
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
