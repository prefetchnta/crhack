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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack LZW 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* LZW 常数 */
#define LZW_BASE        0x102       /* the code base   */
#define LZW_CODE_LEN    12          /* max code length */
#define LZW_TABLE_LEN   4099        /* must be prime number and bigger than
                                       2 ^ LZW_CODE_LEN = 4096 such as 5051 is
                                       also ok */
#define LZW_BUFFERSIZE  1024
#define LZW_HASHSTEP    13          /* it should bigger than 0 */
#define LZW_DIV         LZW_TABLE_LEN
#define LZW_INIT(buf)   buf.top     = 0; \
                        buf.left    = 0; \
                        buf.index   = 0; \
                        buf.buffer  = 0; \
                        buf.endFlag = FALSE;
/* LZW 内部结构 */
typedef struct
{
        uint_t  prefix;
        uint_t  suffix;
        uint_t  code, curCodeLen;
        int16u  codeTable[LZW_TABLE_LEN];
        int16u  prefixTable[LZW_TABLE_LEN];
        byte_t  suffixTable[LZW_TABLE_LEN];
} sLZW;

typedef struct
{
        uint_t  index;
        uint_t  buffer;
        bool_t  endFlag;
        uint_t  top, left;
        byte_t  buf[LZW_BUFFERSIZE];

} sLZWBUFFER;

typedef struct
{
        uint_t  index;
        byte_t  stack[LZW_TABLE_LEN];

} sLZWSTACK;

/* 代码简化 */
typedef struct
{
        sLZW        handle;
        sLZWBUFFER  in, out;

} encode;

typedef struct
{
        sLZW        handle;
        sLZWSTACK   stack;
        sLZWBUFFER  in, out;

} decode;

/*
---------------------------------------
    初始化代码表
---------------------------------------
*/
static void_t
lzw_reinit (
  __CR_OT__ sLZW*   handle
    )
{
    uint_t  idx;

    handle->code = LZW_BASE;
    handle->curCodeLen = 9;

    for (idx = 0; idx < LZW_TABLE_LEN; idx++)
        handle->codeTable[idx] = 0xFFFF;
}

/*
---------------------------------------
    读取一块数据
---------------------------------------
*/
static uint_t
lzw_load_buffer (
  __CR_IN__ const byte_t*   src,
  __CR_IO__ leng_t*         ssize,
  __CR_IO__ sLZWBUFFER*     buf
    )
{
    uint_t  idx;

    for (idx = 0; idx < LZW_BUFFERSIZE && ssize[0] != 0; ssize[0]--, idx++)
        buf->buf[idx] = src[idx];

    buf->top = idx;
    buf->index = 0;
    return (idx);
}

/*
---------------------------------------
    清空输入缓冲
---------------------------------------
*/
static uint_t
lzw_empty_buffer (
  __CR_OT__ byte_t*     dst,
  __CR_IO__ leng_t*     dsize,
  __CR_IO__ sLZWBUFFER* buf
    )
{
    uint_t  idx;

    if (buf->endFlag && buf->left != 0)
        buf->buf[buf->index++] = (uchar)((buf->buffer >> (32 - buf->left))
                                                      << ( 8 - buf->left));
    for (idx = 0; idx < buf->index; dsize[0]--, idx++)
    {
        if (dsize[0] == 0) {
            idx = buf->index;
            break;
        }
        dst[idx] = buf->buf[idx];
    }

    buf->top = idx;
    buf->index = 0;
    return (idx);
}

/*
---------------------------------------
    计算哈希
---------------------------------------
*/
static uint_t
lzw_hash (
  __CR_IN__ const sLZW* handle
    )
{
    uint_t  prefix = handle->prefix;
    uint_t  suffix = handle->suffix;

    return (((prefix << 8) | suffix) % LZW_DIV);
}

/*
---------------------------------------
    重新计算哈希
---------------------------------------
*/
static uint_t
lzw_rehash (
  __CR_IN__ uint_t  hash
    )
{
    return ((hash + LZW_HASHSTEP) % LZW_DIV);
}

/*
---------------------------------------
    符号是否在表中
---------------------------------------
*/
static bool_t
lzw_in_table (
  __CR_IN__ const sLZW* handle
    )
{
    uint_t  result;
    uint_t  hash = lzw_hash(handle);

    if (handle->codeTable[hash] == 0xFFFF)
        return (FALSE);

    if (handle->prefixTable[hash] == (int16u)handle->prefix &&
        handle->suffixTable[hash] == (byte_t)handle->suffix)
        return (TRUE);
    result = FALSE;

    while (handle->codeTable[hash] != 0xFFFF)
    {
        if (handle->prefixTable[hash] == (int16u)handle->prefix &&
            handle->suffixTable[hash] == (byte_t)handle->suffix)
        {
            result = TRUE;
            break;
        }
        hash = lzw_rehash(hash);
    }
    return (result);
}

/*
---------------------------------------
    获取代码
---------------------------------------
*/
static uint_t
lzw_get_code (
  __CR_IN__ const sLZW* handle
    )
{
    uint_t  hash = lzw_hash(handle);

    if (handle->prefixTable[hash] == (int16u)handle->prefix &&
        handle->suffixTable[hash] == (byte_t)handle->suffix)
        return (handle->codeTable[hash]);

    while (handle->prefixTable[hash] != (int16u)handle->prefix ||
           handle->suffixTable[hash] != (byte_t)handle->suffix)
        hash = lzw_rehash(hash);
    return (handle->codeTable[hash]);
}

/*
---------------------------------------
    插入表格
---------------------------------------
*/
static void_t
lzw_insert_table (
  __CR_IO__ sLZW*   handle
    )
{
    uint_t  hash = lzw_hash(handle);

    if (handle->codeTable[hash] == 0xFFFF)
    {
        handle->codeTable[hash]   = (int16u)handle->code;
        handle->prefixTable[hash] = (int16u)handle->prefix;
        handle->suffixTable[hash] = (byte_t)handle->suffix;
    }
    else
    {
        while (handle->codeTable[hash] != 0xFFFF)
            hash = lzw_rehash(hash);
        handle->codeTable[hash]   = (int16u)handle->code;
        handle->prefixTable[hash] = (int16u)handle->prefix;
        handle->suffixTable[hash] = (byte_t)handle->suffix;
    }
}

/*
---------------------------------------
    输出代码
---------------------------------------
*/
static uint_t
lzw_output_code (
  __CR_OT__ byte_t*     dst,
  __CR_IO__ leng_t*     dsize,
  __CR_IN__ uint_t      code,
  __CR_IO__ sLZWBUFFER* out,
  __CR_IN__ const sLZW* handle
    )
{
    uint_t  idx = 0;

    out->buffer |= code << (32 - out->left - handle->curCodeLen);
    out->left   += handle->curCodeLen;

    while (out->left >= 8)
    {
        if (out->index == LZW_BUFFERSIZE)
            idx += lzw_empty_buffer(&dst[idx], dsize, out);
        out->buf[out->index++] = (uchar)(out->buffer >> 24);

        out->left    -= 8;
        out->buffer <<= 8;
    }
    return (idx);
}

/*
---------------------------------------
    进行编码
---------------------------------------
*/
static uint_t
lzw_do_encode (
  __CR_OT__ byte_t*     dst,
  __CR_IO__ leng_t*     dsize,
  __CR_IO__ sLZWBUFFER* in,
  __CR_IO__ sLZWBUFFER* out,
  __CR_IO__ sLZW*       handle
    )
{
    uint_t  prefix;
    uint_t  idx = 0;

    while (in->index != in->top)
    {
        if (!lzw_in_table(handle))
        {
            lzw_insert_table(handle);

            prefix = handle->suffix;
            idx += lzw_output_code(&dst[idx], dsize, handle->prefix,
                                  out, handle);
            handle->code++;
            if (handle->code == (1UL << handle->curCodeLen))
            {
                handle->curCodeLen++;
                if (handle->curCodeLen == LZW_CODE_LEN + 1)
                    lzw_reinit(handle);
            }
        }
        else
        {
            prefix = lzw_get_code(handle);
        }
        handle->prefix = prefix;
        handle->suffix = in->buf[in->index++];
    }
    return (idx);
}

/*
---------------------------------------
    输出代码
---------------------------------------
*/
static uint_t
lzw_out_code (
  __CR_OT__ byte_t*     dst,
  __CR_IO__ leng_t*     dsize,
  __CR_IN__ uint_t      code,
  __CR_IO__ sLZWBUFFER* buf,
  __CR_IN__ const sLZW* handle,
  __CR_IO__ sLZWSTACK*  stack
    )
{
    uint_t  tmp;
    uint_t  idx = 0;

    if (code < 0x100)
    {
        stack->stack[stack->index++] = (uchar)code;
    }
    else
    {
        stack->stack[stack->index++] = handle->suffixTable[code];

        tmp = handle->prefixTable[code];

        while (tmp > 0x100)
        {
            stack->stack[stack->index++] = handle->suffixTable[tmp];
            tmp = handle->prefixTable[tmp];
        }
        stack->stack[stack->index++] = (uchar)tmp;
    }

    while (stack->index != 0)
    {
        if (buf->index == LZW_BUFFERSIZE)
            idx += lzw_empty_buffer(&dst[idx], dsize, buf);
        buf->buf[buf->index++] = stack->stack[--stack->index];
    }
    return (idx);
}

/*
---------------------------------------
    插入表格
---------------------------------------
*/
static void_t
lzw_insert_to_table (
  __CR_IO__ sLZW*   handle
    )
{
    handle->codeTable[handle->code]   = (int16u)handle->code;
    handle->prefixTable[handle->code] = (int16u)handle->prefix;
    handle->suffixTable[handle->code] = (byte_t)handle->suffix;
    handle->code++;

    if (handle->code == (1UL << handle->curCodeLen) - 1)
    {
        handle->curCodeLen++;

        if (handle->curCodeLen == LZW_CODE_LEN + 1)
            handle->curCodeLen = 9;
    }

    if (handle->code >= (1UL << LZW_CODE_LEN))
        lzw_reinit(handle);
}

/*
---------------------------------------
    获取下一个代码
---------------------------------------
*/
static uint_t
lzw_get_next_code (
  __CR_IO__ byte_t**    src,
  __CR_IO__ leng_t*     ssize,
  __CR_IO__ sLZWBUFFER* buf,
  __CR_IN__ const sLZW* handle
    )
{
    uint_t  next;
    uint_t  code;
    uint_t  idx = 0;

    while (buf->left < handle->curCodeLen)
    {
        if (buf->index == LZW_BUFFERSIZE)
        {
            idx += lzw_load_buffer(*src, ssize, buf);
            *src += idx;
        }
        next = buf->buf[buf->index++];

        buf->buffer |= next << (24 - buf->left);
        buf->left   += 8;
    }
    code = buf->buffer >> (32 - handle->curCodeLen);

    buf->left    -= handle->curCodeLen;
    buf->buffer <<= handle->curCodeLen;
    return (code);
}

/*
---------------------------------------
    进行解码
---------------------------------------
*/
static uint_t
lzw_do_decode (
  __CR_OT__ byte_t*     dst,
  __CR_IO__ leng_t*     dsize,
  __CR_IO__ byte_t**    src,
  __CR_IO__ leng_t*     ssize,
  __CR_IO__ sLZWBUFFER* in,
  __CR_IO__ sLZWBUFFER* out,
  __CR_IO__ sLZW*       handle,
  __CR_IO__ sLZWSTACK*  stack
    )
{
    uint_t  tmp;
    uint_t  code, idx = 0;

    while (in->index != in->top)
    {
        code = lzw_get_next_code(src, ssize, in, handle);

        if (code < 0x100)
        {
            handle->suffix = code;
        }
        else
        {
            if (code < handle->code)
            {
                tmp = handle->prefixTable[code];

                while (tmp > 0x100)
                    tmp = handle->prefixTable[tmp];
                handle->suffix = tmp;
            }
            else
            {
                tmp = handle->prefix;

                while (tmp > 0x100)
                    tmp = handle->prefixTable[tmp];
                handle->suffix = tmp;
            }
        }
        lzw_insert_to_table(handle);

        idx += lzw_out_code(&dst[idx], dsize, code, out, handle, stack);
        handle->prefix = code;
    }
    return (idx);
}

/*
=======================================
    LZW 压缩
=======================================
*/
CR_API leng_t
compr_lzw (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  idx;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  o_ptr;
    leng_t  dsize;
    bool_t  first;
    encode* handle;

    if (dst == NULL)
        return (srclen + srclen / 2 + 4096);

    handle = struct_new(encode);
    if (handle == NULL)
        return (0);
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;
    first = TRUE;
    dsize = dstlen;

    LZW_INIT(handle->in)
    LZW_INIT(handle->out)
    lzw_reinit(&handle->handle);

    while ((idx = lzw_load_buffer(i_buf, &srclen, &handle->in)) != 0)
    {
        i_buf += idx;

        if (first)
        {
            first = FALSE;
            handle->handle.prefix = handle->in.buf[handle->in.index++];
            handle->handle.suffix = handle->in.buf[handle->in.index++];
        }

        if (o_ptr >= dstlen)
            goto _failure;
        o_ptr += lzw_do_encode(&o_buf[o_ptr], &dsize, &handle->in,
                               &handle->out, &handle->handle);
    }

    if (o_ptr >= dstlen)
        goto _failure;
    o_ptr += lzw_output_code(&o_buf[o_ptr], &dsize, handle->handle.prefix,
                             &handle->out, &handle->handle);
    if (o_ptr >= dstlen)
        goto _failure;
    o_ptr += lzw_output_code(&o_buf[o_ptr], &dsize, handle->handle.suffix,
                             &handle->out, &handle->handle);
    handle->out.endFlag = TRUE;
    if (o_ptr >= dstlen)
        goto _failure;
    o_ptr += lzw_empty_buffer(&o_buf[o_ptr], &dsize, &handle->out);
    if (o_ptr > dstlen)
        goto _failure;
    mem_free(handle);
    return (o_ptr);

_failure:
    mem_free(handle);
    return (0);
}

/*
=======================================
    LZW 解压
=======================================
*/
CR_API leng_t
uncompr_lzw (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  idx;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  o_ptr;
    leng_t  dsize;
    bool_t  first;
    decode* handle;

    handle = struct_new(decode);
    if (handle == NULL)
        return (0);
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;
    first = TRUE;
    dsize = dstlen;

    LZW_INIT(handle->in)
    LZW_INIT(handle->out)
    handle->stack.index = 0;
    lzw_reinit(&handle->handle);

    while ((idx = lzw_load_buffer(i_buf, &srclen, &handle->in)) != 0)
    {
        i_buf += idx;

        if (first)
        {
            handle->handle.prefix = lzw_get_next_code((uchar**)(&i_buf),
                                                      &srclen, &handle->in,
                                                      &handle->handle);
            handle->handle.suffix = handle->handle.prefix;
            if (o_ptr >= dstlen)
                goto _failure;
            o_ptr += lzw_out_code(&o_buf[o_ptr], &dsize,
                        handle->handle.prefix, &handle->out, &handle->handle,
                                  &handle->stack);
            first = FALSE;
        }

        if (o_ptr >= dstlen)
            goto _failure;
        o_ptr += lzw_do_decode(&o_buf[o_ptr], &dsize, (uchar**)(&i_buf),
                               &srclen, &handle->in, &handle->out,
                               &handle->handle, &handle->stack);
    }

    if (o_ptr >= dstlen)
        goto _failure;
    o_ptr += lzw_empty_buffer(&o_buf[o_ptr], &dsize, &handle->out);
    if (o_ptr > dstlen)
        goto _failure;
    mem_free(handle);
    return (o_ptr);

_failure:
    mem_free(handle);
    return (0);
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
