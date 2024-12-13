/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-07  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>> TOTAL CFG 序列化内置原生类型函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*****************************************************************************/
/*                              二进制转字符串                               */
/*****************************************************************************/

/*
---------------------------------------
    类型 ansi_t 转字符串
---------------------------------------
*/
static ansi_t*
ansi_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    ansi_t* outp;
    ansi_t* temp;

    temp = (ansi_t*)mem_malloc(size + 1);
    if (temp == NULL)
        return (NULL);
    mem_cpy(temp, data, size);
    temp[size] = CR_AC(NIL);
    outp = str_fmtA("<%s value=\"%s\" />", memb->show, temp);
    mem_free(temp);
    return (outp);
}

/*
---------------------------------------
    类型 wide_t 转字符串
---------------------------------------
*/
static ansi_t*
wide_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    ansi_t* outp;
    ansi_t* temp;

    if (size % 2 != 0)
        return (NULL);
    temp = (ansi_t*)mem_malloc(size * 2 + 1);
    if (temp == NULL)
        return (NULL);
    hex2strA(temp, data, size);
    outp = str_fmtA("<%s value=\"%s\" />", memb->show, temp);
    mem_free(temp);
    return (outp);
}

/*
---------------------------------------
    类型 byte_t 转字符串
---------------------------------------
*/
static ansi_t*
byte_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    ansi_t* outp;
    ansi_t* temp;

    temp = (ansi_t*)mem_malloc(size * 2 + 1);
    if (temp == NULL)
        return (NULL);
    hex2strA(temp, data, size);
    outp = str_fmtA("<%s value=\"%s\" />", memb->show, temp);
    mem_free(temp);
    return (outp);
}

/*
---------------------------------------
    类型 int08s 转字符串
---------------------------------------
*/
static ansi_t*
int08s_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    byte_t  vals;

    if (size != sizeof(byte_t))
        return (NULL);
    vals = *(byte_t*)data;
    return (str_fmtA("<%s value=\"%" CR_I08 "d\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int08u 转字符串
---------------------------------------
*/
static ansi_t*
int08u_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    byte_t  vals;

    if (size != sizeof(byte_t))
        return (NULL);
    vals = *(byte_t*)data;
    return (str_fmtA("<%s value=\"%" CR_I08 "u\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int16s 转字符串
---------------------------------------
*/
static ansi_t*
int16s_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int16s  vals;

    if (size != sizeof(int16s))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int16s));
    if (memb->attr == 1)
        vals = WORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = WORD_BE(vals);
    return (str_fmtA("<%s value=\"%" CR_I16 "d\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int16u 转字符串
---------------------------------------
*/
static ansi_t*
int16u_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int16u  vals;

    if (size != sizeof(int16u))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int16u));
    if (memb->attr == 1)
        vals = WORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = WORD_BE(vals);
    return (str_fmtA("<%s value=\"%" CR_I16 "u\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int32s 转字符串
---------------------------------------
*/
static ansi_t*
int32s_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int32s  vals;

    if (size != sizeof(int32s))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int32s));
    if (memb->attr == 1)
        vals = DWORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = DWORD_BE(vals);
    return (str_fmtA("<%s value=\"%" CR_I32 "d\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int32u 转字符串
---------------------------------------
*/
static ansi_t*
int32u_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int32u  vals;

    if (size != sizeof(int32u))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int32u));
    if (memb->attr == 1)
        vals = DWORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = DWORD_BE(vals);
    return (str_fmtA("<%s value=\"%" CR_I32 "u\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int64s 转字符串
---------------------------------------
*/
static ansi_t*
int64s_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int64s  vals;

    if (size != sizeof(int64s))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int64s));
    if (memb->attr == 1)
        vals = QWORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = QWORD_BE(vals);
    return (str_fmtA("<%s value=\"%" CR_I64 "d\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 int64u 转字符串
---------------------------------------
*/
static ansi_t*
int64u_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int64u  vals;

    if (size != sizeof(int64u))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int64u));
    if (memb->attr == 1)
        vals = QWORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = QWORD_BE(vals);
    return (str_fmtA("<%s value=\"%" CR_I64 "u\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 fp32_t 转字符串
---------------------------------------
*/
static ansi_t*
fp32_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int32s  temp;
    fp32_t  vals;

    if (size != sizeof(fp32_t))
        return (NULL);
    mem_cpy(&temp, data, sizeof(int32s));
    if (memb->attr == 1)
        temp = DWORD_LE(temp);
    else
    if (memb->attr == 2)
        temp = DWORD_BE(temp);
    vals = cvt_i2f(temp);
    return (str_fmtA("<%s value=\"%G\" />", memb->show, vals));
}

/*
---------------------------------------
    类型 fp64_t 转字符串
---------------------------------------
*/
static ansi_t*
fp64_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    int64s  temp;
    fp64_t  vals;

    if (size != sizeof(fp64_t))
        return (NULL);
    mem_cpy(&temp, data, sizeof(int64s));
    if (memb->attr == 1)
        temp = QWORD_LE(temp);
    else
    if (memb->attr == 2)
        temp = QWORD_BE(temp);
    vals = cvt_i2d(temp);
    return (str_fmtA("<%s value=\"%G\" />", memb->show, vals));
}

/*****************************************************************************/
/*                              字符串转二进制                               */
/*****************************************************************************/

/*
---------------------------------------
    字符串转 ansi_t 类型
---------------------------------------
*/
static bool_t
ansi_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    ansi_t* temps;
    wide_t* value;

    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = utf16_to_local(cpage, value);
    mem_free(value);
    if (temps == NULL)
        return (FALSE);
    index = str_lenA(temps);
    if (attr) index++;      /* 保留\0结尾 */
    if (index > size) {
        mem_free(temps);
        return (FALSE);
    }
    mem_zero(data, size);
    if (index != 0)
        mem_cpy(data, temps, index);
    mem_free(temps);
    return (TRUE);
}

/*
---------------------------------------
    字符串转 wide_t 类型
---------------------------------------
*/
static bool_t
wide_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t* value;

    CR_NOUSE(attr); CR_NOUSE(cpage);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    index = str_lenW(value);
    if (index % 4 != 0 || index / 2 != size) {
        mem_free(value);
        return (FALSE);
    }
    index = size;
    str2datW(data, &index, value);
    mem_free(value);
    return ((index != size) ? FALSE : TRUE);
}

/*
---------------------------------------
    字符串转 byte_t 类型
---------------------------------------
*/
static bool_t
byte_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t* value;

    CR_NOUSE(attr); CR_NOUSE(cpage);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    index = str_lenW(value);
    if (index % 2 != 0 || index / 2 != size) {
        mem_free(value);
        return (FALSE);
    }
    index = size;
    str2datW(data, &index, value);
    mem_free(value);
    return ((index != size) ? FALSE : TRUE);
}

/*
---------------------------------------
    字符串转 int08s 类型
---------------------------------------
*/
static bool_t
int08s_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    sint_t  temps;
    wide_t* value;

    CR_NOUSE(attr); CR_NOUSE(cpage);
    if (size != sizeof(byte_t))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2intW(value, &index);
    mem_free(value);
    if (index > 4 || temps > 127 || temps < -128)
        return (FALSE);
    *(byte_t*)data = (byte_t)temps;
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int08u 类型
---------------------------------------
*/
static bool_t
int08u_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    uint_t  temps;
    wide_t* value;

    CR_NOUSE(attr); CR_NOUSE(cpage);
    if (size != sizeof(byte_t))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2intW(value, &index);
    mem_free(value);
    if (index > 3 || temps > 255)
        return (FALSE);
    *(byte_t*)data = (byte_t)temps;
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int16s 类型
---------------------------------------
*/
static bool_t
int16s_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int16s  copys;
    int32s  temps;
    wide_t* value;

    if (size != sizeof(int16s))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2int32W(value, &index);
    mem_free(value);
    if (index > 6 || temps > 32767L || temps < -32768L)
        return (FALSE);
    CR_NOUSE(cpage);
    copys = (int16s)temps;
    if (attr == 1)
        copys = WORD_LE(copys);
    else
    if (attr == 2)
        copys = WORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int16s));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int16u 类型
---------------------------------------
*/
static bool_t
int16u_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int16u  copys;
    int32u  temps;
    wide_t* value;

    if (size != sizeof(int16u))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2int32W(value, &index);
    mem_free(value);
    if (index > 5 || temps > 65535UL)
        return (FALSE);
    CR_NOUSE(cpage);
    copys = (int16u)temps;
    if (attr == 1)
        copys = WORD_LE(copys);
    else
    if (attr == 2)
        copys = WORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int16u));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int32s 类型
---------------------------------------
*/
static bool_t
int32s_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int32s  copys;
    int64s  temps;
    wide_t* value;

    if (size != sizeof(int32s))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2int64W(value, &index);
    mem_free(value);
    if (index > 11 ||
        temps > CR_SLL( 2147483647) ||
        temps < CR_SLL(-2147483648))
        return (FALSE);
    CR_NOUSE(cpage);
    copys = (int32s)temps;
    if (attr == 1)
        copys = DWORD_LE(copys);
    else
    if (attr == 2)
        copys = DWORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int32s));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int32u 类型
---------------------------------------
*/
static bool_t
int32u_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int32u  copys;
    int64u  temps;
    wide_t* value;

    if (size != sizeof(int32u))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2int64W(value, &index);
    mem_free(value);
    if (index > 10 || temps > CR_ULL(4294967295))
        return (FALSE);
    CR_NOUSE(cpage);
    copys = (int32u)temps;
    if (attr == 1)
        copys = DWORD_LE(copys);
    else
    if (attr == 2)
        copys = DWORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int32u));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int64s 类型
---------------------------------------
*/
static bool_t
int64s_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int64s  temps;
    wide_t* value;

    if (size != sizeof(int64s))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2int64W(value, &index);
    mem_free(value);
    if (index > 20)
        return (FALSE);
    CR_NOUSE(cpage);
    if (attr == 1)
        temps = QWORD_LE(temps);
    else
    if (attr == 2)
        temps = QWORD_BE(temps);
    mem_cpy(data, &temps, sizeof(int64s));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 int64u 类型
---------------------------------------
*/
static bool_t
int64u_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int64u  temps;
    wide_t* value;

    if (size != sizeof(int64u))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = str2int64W(value, &index);
    mem_free(value);
    if (index > 20)
        return (FALSE);
    CR_NOUSE(cpage);
    if (attr == 1)
        temps = QWORD_LE(temps);
    else
    if (attr == 2)
        temps = QWORD_BE(temps);
    mem_cpy(data, &temps, sizeof(int64u));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 fp32_t 类型
---------------------------------------
*/
static bool_t
fp32_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int32s  copys;
    fp32_t  temps;
    wide_t* value;

    if (size != sizeof(fp32_t))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    CR_NOUSE(cpage);
    CR_NOUSE(index);
    temps = str2fp32W(value, NULL);
    copys = cvt_f2i(temps);
    mem_free(value);
    if (attr == 1)
        copys = DWORD_LE(copys);
    else
    if (attr == 2)
        copys = DWORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int32s));
    return (TRUE);
}

/*
---------------------------------------
    字符串转 fp64_t 类型
---------------------------------------
*/
static bool_t
fp64_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int64s  copys;
    fp64_t  temps;
    wide_t* value;

    if (size != sizeof(fp64_t))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    CR_NOUSE(cpage);
    CR_NOUSE(index);
    temps = str2fp64W(value, NULL);
    copys = cvt_d2i(temps);
    mem_free(value);
    if (attr == 1)
        copys = QWORD_LE(copys);
    else
    if (attr == 2)
        copys = QWORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int64s));
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
