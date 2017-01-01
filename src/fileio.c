/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-26  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 文件操作函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "fileio.h"
#include "memlib.h"

/*****************************************************************************/
/*                      基本 I/O 操作 (不支持二级缓冲)                       */
/*****************************************************************************/

/*
=======================================
    文件指针定位64
=======================================
*/
CR_API bool_t
file_raw_seek64 (
  __CR_IN__ fraw_t  file,
  __CR_IN__ int64s  offset,
  __CR_IN__ uint_t  whence
    )
{
#if defined(_CR_FILE64_)
    return (file_raw_seek(file, offset, whence));
#else
    fdist_t move;

    /* 64位有符号整数截断检查 */
    if (cut_int64_32s(&move, offset))
        return (FALSE);
    return (file_raw_seek(file, move, whence));
#endif
}

/*
=======================================
    文件指针移动到文件开始处
=======================================
*/
CR_API bool_t
file_raw_rewind (
  __CR_IN__ fraw_t  file
    )
{
    return (file_raw_seek(file, 0, SEEK_SET));
}

/*
=======================================
    文件指针是否位于文件的末尾
=======================================
*/
CR_API bool_t
file_raw_eof (
  __CR_IN__ fraw_t  file
    )
{
    fsize_t tell, size;

    tell = file_raw_tell(file);
    if (tell == CR_F_ERROR)
        return (TRUE);

    size = file_raw_size(file);
    if (size == CR_F_ERROR)
        return (TRUE);
    return ((tell >= size) ? TRUE : FALSE);
}

/*
=======================================
    读取一个字节
=======================================
*/
CR_API bool_t
file_raw_getb (
  __CR_OT__ byte_t* val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_read(val, 1, file) != 1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取一个单字
=======================================
*/
CR_API bool_t
file_raw_getw (
  __CR_OT__ int16u* val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_read(val, 2, file) != 2)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取一个双字
=======================================
*/
CR_API bool_t
file_raw_getd (
  __CR_OT__ int32u* val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_read(val, 4, file) != 4)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取一个四字
=======================================
*/
CR_API bool_t
file_raw_getq (
  __CR_OT__ int64u* val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_read(val, 8, file) != 8)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个字节
=======================================
*/
CR_API bool_t
file_raw_putb (
  __CR_IN__ byte_t  val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_write(&val, 1, file) != 1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个单字
=======================================
*/
CR_API bool_t
file_raw_putw (
  __CR_IN__ int16u  val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_write(&val, 2, file) != 2)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个双字
=======================================
*/
CR_API bool_t
file_raw_putd (
  __CR_IN__ int32u  val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_write(&val, 4, file) != 4)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个四字
=======================================
*/
CR_API bool_t
file_raw_putq (
  __CR_IN__ int64u  val,
  __CR_IN__ fraw_t  file
    )
{
    if (file_raw_write(&val, 8, file) != 8)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入字符串A
=======================================
*/
CR_API bool_t
file_raw_putsA (
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ fraw_t          file
    )
{
    leng_t  len = str_lenA(str) * sizeof(ansi_t);

    if (len != 0) {
        if (file_raw_write(str, len, file) != len)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    写入字符串W
=======================================
*/
CR_API bool_t
file_raw_putsW (
  __CR_IN__ const wide_t*   str,
  __CR_IN__ fraw_t          file
    )
{
    leng_t  len = str_lenW(str) * sizeof(wide_t);

    if (len != 0) {
        if (file_raw_write(str, len, file) != len)
            return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                       基本 I/O 操作 (支持二级缓冲)                        */
/*****************************************************************************/

/*
=======================================
    文件指针定位64
=======================================
*/
CR_API bool_t
file_buf_seek64 (
  __CR_IN__ fbuf_t  file,
  __CR_IN__ int64s  offset,
  __CR_IN__ uint_t  whence
    )
{
#if defined(_CR_FILE64_)
    return (file_buf_seek(file, offset, whence));
#else
    fdist_t move;

    /* 64位有符号整数截断检查 */
    if (cut_int64_32s(&move, offset))
        return (FALSE);
    return (file_buf_seek(file, move, whence));
#endif
}

/*
=======================================
    文件指针移动到文件开始处
=======================================
*/
CR_API bool_t
file_buf_rewind (
  __CR_IN__ fbuf_t  file
    )
{
    return (file_buf_seek(file, 0, SEEK_SET));
}

/*
=======================================
    文件指针是否位于文件的末尾
=======================================
*/
CR_API bool_t
file_buf_eof (
  __CR_IN__ fbuf_t  file
    )
{
    fsize_t tell, size;

    tell = file_buf_tell(file);
    if (tell == CR_F_ERROR)
        return (TRUE);

    size = file_buf_size(file);
    if (size == CR_F_ERROR)
        return (TRUE);
    return ((tell >= size) ? TRUE : FALSE);
}

/*
=======================================
    读取一个字节
=======================================
*/
CR_API bool_t
file_buf_getb (
  __CR_OT__ byte_t* val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_read(val, 1, file) != 1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取一个单字
=======================================
*/
CR_API bool_t
file_buf_getw (
  __CR_OT__ int16u* val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_read(val, 2, file) != 2)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取一个双字
=======================================
*/
CR_API bool_t
file_buf_getd (
  __CR_OT__ int32u* val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_read(val, 4, file) != 4)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取一个四字
=======================================
*/
CR_API bool_t
file_buf_getq (
  __CR_OT__ int64u* val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_read(val, 8, file) != 8)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个字节
=======================================
*/
CR_API bool_t
file_buf_putb (
  __CR_IN__ byte_t  val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_write(&val, 1, file) != 1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个单字
=======================================
*/
CR_API bool_t
file_buf_putw (
  __CR_IN__ int16u  val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_write(&val, 2, file) != 2)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个双字
=======================================
*/
CR_API bool_t
file_buf_putd (
  __CR_IN__ int32u  val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_write(&val, 4, file) != 4)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入一个四字
=======================================
*/
CR_API bool_t
file_buf_putq (
  __CR_IN__ int64u  val,
  __CR_IN__ fbuf_t  file
    )
{
    if (file_buf_write(&val, 8, file) != 8)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入字符串A
=======================================
*/
CR_API bool_t
file_buf_putsA (
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ fbuf_t          file
    )
{
    leng_t  len = str_lenA(str) * sizeof(ansi_t);

    if (len != 0) {
        if (file_buf_write(str, len, file) != len)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    写入字符串W
=======================================
*/
CR_API bool_t
file_buf_putsW (
  __CR_IN__ const wide_t*   str,
  __CR_IN__ fbuf_t          file
    )
{
    leng_t  len = str_lenW(str) * sizeof(wide_t);

    if (len != 0) {
        if (file_buf_write(str, len, file) != len)
            return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                             文件操作助手函数                              */
/*****************************************************************************/

/*
=======================================
    检查文件是否存在A
=======================================
*/
CR_API bool_t
file_existA (
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t  file;

    file = file_raw_openA(name, CR_FO_RO);
    if (file == NULL)
        return (FALSE);
    file_raw_close(file);
    return (TRUE);
}

/*
=======================================
    检查文件是否存在W
=======================================
*/
CR_API bool_t
file_existW (
  __CR_IN__ const wide_t*   name
    )
{
    fraw_t  file;

    file = file_raw_openW(name, CR_FO_RO);
    if (file == NULL)
        return (FALSE);
    file_raw_close(file);
    return (TRUE);
}

/*
=======================================
    获取文件大小A
=======================================
*/
CR_API fsize_t
file_sizeA (
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t  file;
    fsize_t size;

    file = file_raw_openA(name, CR_FO_RO);
    if (file == NULL)
        return (CR_F_ERROR);

    size = file_raw_size(file);
    file_raw_close(file);
    return (size);
}

/*
=======================================
    获取文件大小W
=======================================
*/
CR_API fsize_t
file_sizeW (
  __CR_IN__ const wide_t*   name
    )
{
    fraw_t  file;
    fsize_t size;

    file = file_raw_openW(name, CR_FO_RO);
    if (file == NULL)
        return (CR_F_ERROR);

    size = file_raw_size(file);
    file_raw_close(file);
    return (size);
}

/*
=======================================
    把整个文件当字符串读入A
=======================================
*/
CR_API ansi_t*
file_load_as_strA (
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t  file;
    leng_t  back;
    ansi_t* text;
    fsize_t size;

    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (NULL);

    size = file_raw_size(file);
    if (size == CR_F_ERROR)
        goto _failure;

    if (size > ((leng_t)-1) / 2 - sizeof(int64u))
        goto _failure;

    /* 兼容 UTF-32 编码 */
    text = str_allocA((leng_t)(size + sizeof(int64u)));
    if (text == NULL)
        goto _failure;

    back = file_raw_read(text, (leng_t)size, file);
    if (back != size) {
        mem_free(text);
        goto _failure;
    }
    file_raw_close(file);
    mem_zero(&text[back], sizeof(int64u));
    return (text);

_failure:
    file_raw_close(file);
    return (NULL);
}

/*
=======================================
    把整个文件当字符串读入W
=======================================
*/
CR_API ansi_t*
file_load_as_strW (
  __CR_IN__ const wide_t*   name
    )
{
    fraw_t  file;
    leng_t  back;
    ansi_t* text;
    fsize_t size;

    file = file_raw_openW(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (NULL);

    size = file_raw_size(file);
    if (size == CR_F_ERROR)
        goto _failure;

    if (size > ((leng_t)-1) / 2 - sizeof(int64u))
        goto _failure;

    /* 兼容 UTF-32 编码 */
    text = str_allocA((leng_t)(size + sizeof(int64u)));
    if (text == NULL)
        goto _failure;

    back = file_raw_read(text, (leng_t)size, file);
    if (back != size) {
        mem_free(text);
        goto _failure;
    }
    file_raw_close(file);
    mem_zero(&text[back], sizeof(int64u));
    return (text);

_failure:
    file_raw_close(file);
    return (NULL);
}

/*
=======================================
    读入整个文件A
=======================================
*/
CR_API void_t*
file_load_as_binA (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ leng_t*         size
    )
{
    fraw_t  file;
    leng_t  back;
    void_t* data;
    fsize_t temp;

    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (NULL);

    temp = file_raw_size(file);
    if (temp == CR_F_ERROR)
        goto _failure;

    if (temp > ((leng_t)-1) / 2)
        goto _failure;

    data = mem_malloc((leng_t)temp);
    if (data == NULL)
        goto _failure;

    back = file_raw_read(data, (leng_t)temp, file);
    if (back != temp) {
        mem_free(data);
        goto _failure;
    }

    if (size != NULL)
        *size = back;
    file_raw_close(file);
    return (data);

_failure:
    file_raw_close(file);
    return (NULL);
}

/*
=======================================
    读入整个文件W
=======================================
*/
CR_API void_t*
file_load_as_binW (
  __CR_IN__ const wide_t*   name,
  __CR_OT__ leng_t*         size
    )
{
    fraw_t  file;
    leng_t  back;
    void_t* data;
    fsize_t temp;

    file = file_raw_openW(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (NULL);

    temp = file_raw_size(file);
    if (temp == CR_F_ERROR)
        goto _failure;

    if (temp > ((leng_t)-1) / 2)
        goto _failure;

    data = mem_malloc((leng_t)temp);
    if (data == NULL)
        goto _failure;

    back = file_raw_read(data, (leng_t)temp, file);
    if (back != temp) {
        mem_free(data);
        goto _failure;
    }

    if (size != NULL)
        *size = back;
    file_raw_close(file);
    return (data);

_failure:
    file_raw_close(file);
    return (NULL);
}

/*
=======================================
    生成一个文件A
=======================================
*/
CR_API bool_t
file_saveA (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fraw_t  file;
    leng_t  back;

    file = file_raw_openA(name, CR_FO_WO | CR_FO_SEQ);
    if (file == NULL)
        return (FALSE);

    back = file_raw_write(data, size, file);
    file_raw_close(file);
    if (back != size) {
        file_deleteA(name);
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    生成一个文件W
=======================================
*/
CR_API bool_t
file_saveW (
  __CR_IN__ const wide_t*   name,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fraw_t  file;
    leng_t  back;

    file = file_raw_openW(name, CR_FO_WO | CR_FO_SEQ);
    if (file == NULL)
        return (FALSE);

    back = file_raw_write(data, size, file);
    file_raw_close(file);
    if (back != size) {
        file_deleteW(name);
        return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    文件逐块复制
---------------------------------------
*/
static bool_t
copy_file_data (
  __CR_IN__ fraw_t  out,
  __CR_IN__ fraw_t  inp,
  __CR_IN__ fsize_t size
    )
{
    byte_t  tmp[4096];
    fsize_t blk = (fsize_t)(size / sizeof(tmp));
    leng_t  rst = ( leng_t)(size % sizeof(tmp));

    /* 逐块复制 */
    for (; blk != 0; blk--) {
        if (file_raw_read(tmp, sizeof(tmp), inp) != sizeof(tmp))
            return (FALSE);
        if (file_raw_write(tmp, sizeof(tmp), out) != sizeof(tmp))
            return (FALSE);
    }

    /* 复制尾部 */
    if (rst != 0) {
        if (file_raw_read(tmp, rst, inp) != rst)
            return (FALSE);
        if (file_raw_write(tmp, rst, out) != rst)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    替换文件内容A
=======================================
*/
CR_API fsize_t
file_replaceA (
  __CR_IN__ const ansi_t*   new_name,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ leng_t          count,
  __CR_IN__ const fdist_t   offsets[],
  __CR_IN__ const fdist_t   blocks[],
  __CR_IN__ const ansi_t*   files[]
    )
{
    leng_t  idx;
    bool_t  ret;
    fraw_t  i_temp;
    fraw_t  i_file;
    fraw_t  o_file;
    fdist_t rplc_size;
    fsize_t last, size, total;

    /* 打开源文件 */
    i_file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (i_file == NULL)
        return (CR_F_ERROR);

    size = file_raw_size(i_file);
    if (size == CR_F_ERROR)
        goto _failure1;

    /* 生成新文件 */
    o_file = file_raw_openA(new_name, CR_FO_WO | CR_FO_SEQ);
    if (o_file == NULL)
        goto _failure1;

    /* 逐块输出替换 */
    for (idx = 0, last = 0, total = size; idx < count; idx++)
    {
        /* 安全检查, 偏移必须是升序不重叠的, 替换块不能超过原大小 */
        if (offsets[idx] < 0 || blocks[idx] < 0 ||
           (fsize_t)offsets[idx] < last || (fsize_t)offsets[idx] > size ||
           (fsize_t) blocks[idx] > size  - (fsize_t)offsets[idx] ||
            !copy_file_data(o_file, i_file, offsets[idx] - last))
            goto _failure2;
        rplc_size = 0;

        /* NULL 表示删除处理 */
        if (files[idx] != NULL)
        {
            /* 打开替换文件 */
            i_temp = file_raw_openA(files[idx], CR_FO_RO | CR_FO_SEQ);
            if (i_temp == NULL)
                goto _failure2;

            rplc_size = file_raw_size(i_temp);
            if (rplc_size < 0) {
                file_raw_close(i_temp);
                goto _failure2;
            }

            /* 替换指定数据 */
            ret = copy_file_data(o_file, i_temp, rplc_size);
            file_raw_close(i_temp);
            if (!ret)
                goto _failure2;
        }

        /* 跳过被替换掉的数据 */
        if (!file_raw_seek(i_file, blocks[idx], SEEK_CUR))
            goto _failure2;

        last = offsets[idx] + blocks[idx];
        total += (fsize_t)(rplc_size - blocks[idx]);
    }

    /* 复制剩余的数据 */
    if (!copy_file_data(o_file, i_file, size - last))
        goto _failure2;
    file_raw_close(o_file);
    file_raw_close(i_file);
    return (total);

_failure2:
    file_raw_close(o_file);
    file_deleteA(new_name);
_failure1:
    file_raw_close(i_file);
    return (CR_F_ERROR);
}

/*
=======================================
    替换文件内容W
=======================================
*/
CR_API fsize_t
file_replaceW (
  __CR_IN__ const wide_t*   new_name,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ leng_t          count,
  __CR_IN__ const fdist_t   offsets[],
  __CR_IN__ const fdist_t   blocks[],
  __CR_IN__ const wide_t*   files[]
    )
{
    leng_t  idx;
    bool_t  ret;
    fraw_t  i_temp;
    fraw_t  i_file;
    fraw_t  o_file;
    fdist_t rplc_size;
    fsize_t last, size, total;

    /* 打开源文件 */
    i_file = file_raw_openW(name, CR_FO_RO | CR_FO_SEQ);
    if (i_file == NULL)
        return (CR_F_ERROR);

    size = file_raw_size(i_file);
    if (size == CR_F_ERROR)
        goto _failure1;

    /* 生成新文件 */
    o_file = file_raw_openW(new_name, CR_FO_WO | CR_FO_SEQ);
    if (o_file == NULL)
        goto _failure1;

    /* 逐块输出替换 */
    for (idx = 0, last = 0, total = size; idx < count; idx++)
    {
        /* 安全检查, 偏移必须是升序不重叠的, 替换块不能超过原大小 */
        if (offsets[idx] < 0 || blocks[idx] < 0 ||
           (fsize_t)offsets[idx] < last || (fsize_t)offsets[idx] > size ||
           (fsize_t) blocks[idx] > size  - (fsize_t)offsets[idx] ||
            !copy_file_data(o_file, i_file, offsets[idx] - last))
            goto _failure2;
        rplc_size = 0;

        /* NULL 表示删除处理 */
        if (files[idx] != NULL)
        {
            /* 打开替换文件 */
            i_temp = file_raw_openW(files[idx], CR_FO_RO | CR_FO_SEQ);
            if (i_temp == NULL)
                goto _failure2;

            rplc_size = file_raw_size(i_temp);
            if (rplc_size < 0) {
                file_raw_close(i_temp);
                goto _failure2;
            }

            /* 替换指定数据 */
            ret = copy_file_data(o_file, i_temp, rplc_size);
            file_raw_close(i_temp);
            if (!ret)
                goto _failure2;
        }

        /* 跳过被替换掉的数据 */
        if (!file_raw_seek(i_file, blocks[idx], SEEK_CUR))
            goto _failure2;

        last = offsets[idx] + blocks[idx];
        total += (fsize_t)(rplc_size - blocks[idx]);
    }

    /* 复制剩余的数据 */
    if (!copy_file_data(o_file, i_file, size - last))
        goto _failure2;
    file_raw_close(o_file);
    file_raw_close(i_file);
    return (total);

_failure2:
    file_raw_close(o_file);
    file_deleteW(new_name);
_failure1:
    file_raw_close(i_file);
    return (CR_F_ERROR);
}

/*
=======================================
    替换文件内容A (支持数据处理)
=======================================
*/
CR_API fsize_t
file_replace2A (
  __CR_IN__ const ansi_t*   new_name,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ leng_t          count,
  __CR_IN__ const fdist_t   offsets[],
  __CR_IN__ const fdist_t   blocks[],
  __CR_IN__ const ansi_t*   files[],
  __CR_IN__ replace_fileA_t get_data,
  __CR_IN__ void_t*         param
    )
{
    fraw_t  i_file;
    fraw_t  o_file;
    leng_t  idx, wrt;
    leng_t  rplc_size;
    void_t* rplc_data;
    fsize_t last, size, total;

    /* 打开源文件 */
    i_file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (i_file == NULL)
        return (CR_F_ERROR);

    size = file_raw_size(i_file);
    if (size == CR_F_ERROR)
        goto _failure1;

    /* 生成新文件 */
    o_file = file_raw_openA(new_name, CR_FO_WO | CR_FO_SEQ);
    if (o_file == NULL)
        goto _failure1;

    /* 逐块输出替换 */
    for (idx = 0, last = 0, total = size; idx < count; idx++)
    {
        /* 安全检查, 偏移必须是升序不重叠的, 替换块不能超过原大小 */
        if (offsets[idx] < 0 || blocks[idx] < 0 ||
           (fsize_t)offsets[idx] < last || (fsize_t)offsets[idx] > size ||
           (fsize_t) blocks[idx] > size  - (fsize_t)offsets[idx] ||
            !copy_file_data(o_file, i_file, offsets[idx] - last))
            goto _failure2;
        rplc_size = 0;

        /* NULL 表示删除处理 */
        if (files[idx] != NULL)
        {
            /* 打开替换文件 */
            rplc_data = get_data(files[idx], &rplc_size, param);
            if (rplc_data == NULL)
                goto _failure2;

            /* 替换指定数据 */
            wrt = file_raw_write(rplc_data, rplc_size, o_file);
            mem_free(rplc_data);
            if (wrt != rplc_size)
                goto _failure2;
        }

        /* 跳过被替换掉的数据 */
        if (!file_raw_seek(i_file, blocks[idx], SEEK_CUR))
            goto _failure2;

        last = offsets[idx] + blocks[idx];
        total += (fsize_t)(rplc_size - blocks[idx]);
    }

    /* 复制剩余的数据 */
    if (!copy_file_data(o_file, i_file, size - last))
        goto _failure2;
    file_raw_close(o_file);
    file_raw_close(i_file);
    return (total);

_failure2:
    file_raw_close(o_file);
    file_deleteA(new_name);
_failure1:
    file_raw_close(i_file);
    return (CR_F_ERROR);
}

/*
=======================================
    替换文件内容W (支持数据处理)
=======================================
*/
CR_API fsize_t
file_replace2W (
  __CR_IN__ const wide_t*   new_name,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ leng_t          count,
  __CR_IN__ const fdist_t   offsets[],
  __CR_IN__ const fdist_t   blocks[],
  __CR_IN__ const wide_t*   files[],
  __CR_IN__ replace_fileW_t get_data,
  __CR_IN__ void_t*         param
    )
{
    fraw_t  i_file;
    fraw_t  o_file;
    leng_t  idx, wrt;
    leng_t  rplc_size;
    void_t* rplc_data;
    fsize_t last, size, total;

    /* 打开源文件 */
    i_file = file_raw_openW(name, CR_FO_RO | CR_FO_SEQ);
    if (i_file == NULL)
        return (CR_F_ERROR);

    size = file_raw_size(i_file);
    if (size == CR_F_ERROR)
        goto _failure1;

    /* 生成新文件 */
    o_file = file_raw_openW(new_name, CR_FO_WO | CR_FO_SEQ);
    if (o_file == NULL)
        goto _failure1;

    /* 逐块输出替换 */
    for (idx = 0, last = 0, total = size; idx < count; idx++)
    {
        /* 安全检查, 偏移必须是升序不重叠的, 替换块不能超过原大小 */
        if (offsets[idx] < 0 || blocks[idx] < 0 ||
           (fsize_t)offsets[idx] < last || (fsize_t)offsets[idx] > size ||
           (fsize_t) blocks[idx] > size  - (fsize_t)offsets[idx] ||
            !copy_file_data(o_file, i_file, offsets[idx] - last))
            goto _failure2;
        rplc_size = 0;

        /* NULL 表示删除处理 */
        if (files[idx] != NULL)
        {
            /* 打开替换文件 */
            rplc_data = get_data(files[idx], &rplc_size, param);
            if (rplc_data == NULL)
                goto _failure2;

            /* 替换指定数据 */
            wrt = file_raw_write(rplc_data, rplc_size, o_file);
            mem_free(rplc_data);
            if (wrt != rplc_size)
                goto _failure2;
        }

        /* 跳过被替换掉的数据 */
        if (!file_raw_seek(i_file, blocks[idx], SEEK_CUR))
            goto _failure2;

        last = offsets[idx] + blocks[idx];
        total += (fsize_t)(rplc_size - blocks[idx]);
    }

    /* 复制剩余的数据 */
    if (!copy_file_data(o_file, i_file, size - last))
        goto _failure2;
    file_raw_close(o_file);
    file_raw_close(i_file);
    return (total);

_failure2:
    file_raw_close(o_file);
    file_deleteW(new_name);
_failure1:
    file_raw_close(i_file);
    return (CR_F_ERROR);
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
