/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-05-23  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>> SFRW 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "fileio.h"
#include "memlib.h"
#include "rtclib.h"
#include "strlib.h"
#include "util/commlib.h"

/* SFRW 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ int32u  type;   /* 命令类型 */
/*004*/ int32u  size;   /* 数据长度 */
/*008*/
} CR_PACKED sSFRW_HDR;

/* 日期时间结构 */
CR_TYPEDEF struct
{
/*000*/ int16u  year;       /* 年 */
/*002*/ byte_t  month;      /* 月 */
/*003*/ byte_t  day;        /* 日 */
/*004*/ byte_t  hour;       /* 时 */
/*005*/ byte_t  minute;     /* 分 */
/*006*/ byte_t  second;     /* 秒 */
/*007*/ byte_t  week;       /* 周 */
/*008*/
} CR_PACKED sSFRW_TIME;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* 内置的命令类型 */
#define SFRW_OKAY   "OKAY"  /* 操作成功 */
#define SFRW_FAIL   "FAIL"  /* 操作失败 */
/* ------------------------------------ */
#define SFRW_FIND   "FIND"  /* 查询文件 */
#define SFRW_PULL   "PULL"  /* 下载文件 */
#define SFRW_PUSH   "PUSH"  /* 上传文件 */
/* ------------------------------------ */
#define SFRW_INFO   "INFO"  /* 文件信息 */
#define SFRW_DATA   "DATA"  /* 文件数据 */
/* ------------------------------------ */

/* 数据分块大小 */
#if !defined(SFRW_BLK_SIZE)
    #define SFRW_BLK_SIZE   512
#endif

/* 最大传输文件 */
#if !defined(SFRW_MAX_SIZE)
    #define SFRW_MAX_SIZE   CR_M2B(512)
#endif

/*
---------------------------------------
    SFRW 读取包头
---------------------------------------
*/
static int32u
sfrw_get_head (
  __CR_IN__ const sSFRW_HDR*    head
    )
{
    return (DWORD_BE(head->size));
}

/*
---------------------------------------
    SFRW 设置包头
---------------------------------------
*/
static void_t
sfrw_set_head (
  __CR_OT__ sSFRW_HDR*      head,
  __CR_IN__ const ansi_t*   type,
  __CR_IN__ int32u          size
    )
{
    head->type = mk_tag4(type);
    head->size = DWORD_BE(size);
}

/*
---------------------------------------
    SFRW 获取文件信息
---------------------------------------
*/
static bool_t
sfrw_get_finfo (
  __CR_OT__ sSFRW_INFO*     info,
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t  file;
    uint_t  rsts, back;
    int32u  size, blks, crc32;
    byte_t  data[SFRW_BLK_SIZE];
    fsize_t fsze = file_sizeA(name);

    /* 过滤文件大小 */
    if (fsze == 0UL || fsze > SFRW_MAX_SIZE)
        return (FALSE);
    size = (int32u)fsze;
    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (FALSE);

    /* 计算文件校验 */
    crc32 = hash_crc32i_init();
    blks = (int32u)(size / SFRW_BLK_SIZE);
    rsts = (uint_t)(size % SFRW_BLK_SIZE);
    for (; blks != 0; blks--) {
        back = (uint_t)file_raw_read(data, SFRW_BLK_SIZE, file);
        if (back != SFRW_BLK_SIZE)
            goto _failure;
        crc32 = hash_crc32i_update(crc32, data, SFRW_BLK_SIZE);
    }
    if (rsts != 0) {
        back = (uint_t)file_raw_read(data, rsts, file);
        if (back != rsts)
            goto _failure;
        crc32 = hash_crc32i_update(crc32, data, rsts);
    }
    file_raw_close(file);
    crc32 = hash_crc32i_finish(crc32);
    info->size  = size;
    info->crc32 = crc32;
    return (TRUE);

_failure:
    file_raw_close(file);
    return (FALSE);
}

/*
---------------------------------------
    SFRW TCP 发送数据
---------------------------------------
*/
static bool_t
sfrw_tcp_send_data (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   type,
  __CR_IN__ int32u          size,
  __CR_IN__ const void_t*   data
    )
{
    int32u      blks;
    uint_t      rsts;
    uint_t      back;
    byte_t*     pntr;
    sSFRW_HDR   head;

    /* 状态包不带后续数据 */
    if (str_cmpA(type, SFRW_OKAY) == 0) {
        size = 0UL;
        data = NULL;
    }
    else
    if (str_cmpA(type, SFRW_FAIL) == 0) {
        data = NULL;
    }
    else {
        if (size == 0UL || size > SFRW_MAX_SIZE)
            return (FALSE);
    }

    /* 发送协议头 */
    sfrw_set_head(&head, type, size);
    back = socket_tcp_send(netw, &head, sizeof(head));
    if (back != sizeof(head))
        return (FALSE);

    /* 到此为止了 */
    if (data == NULL || size == 0UL)
        return (TRUE);

    /* 等待远端响应 */
    back = socket_tcp_recv(netw, &head, sizeof(head));
    if (back != sizeof(head))
        return (FALSE);

    /* 远端成功与否 */
    if (head.type != mk_tag4(SFRW_OKAY))
        return (FALSE);

    /* 分块发送数据 */
    pntr = (byte_t*)data;
    blks = (int32u)(size / SFRW_BLK_SIZE);
    rsts = (uint_t)(size % SFRW_BLK_SIZE);
    for (; blks != 0; blks--, pntr += SFRW_BLK_SIZE)
    {
        /* 发送一块数据 */
        back = socket_tcp_send(netw, pntr, SFRW_BLK_SIZE);
        if (back != SFRW_BLK_SIZE)
            return (FALSE);

        /* 等待远端响应 */
        back = socket_tcp_recv(netw, &head, sizeof(head));
        if (back != sizeof(head))
            return (FALSE);

        /* 远端成功与否 */
        if (head.type != mk_tag4(SFRW_OKAY))
            return (FALSE);
    }

     /* 发送尾部数据 */
     if (rsts != 0) {
        back = socket_tcp_send(netw, pntr, rsts);
        if (back != rsts)
            return (FALSE);

        /* 等待远端响应 */
        back = socket_tcp_recv(netw, &head, sizeof(head));
        if (back != sizeof(head))
            return (FALSE);

        /* 远端成功与否 */
        if (head.type != mk_tag4(SFRW_OKAY))
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    SFRW TCP 发送文件
---------------------------------------
*/
static bool_t
sfrw_tcp_send_file (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t      file;
    int32u      size;
    int32u      blks;
    uint_t      rsts;
    uint_t      back;
    sSFRW_HDR   head;
    /* ------------ */
    byte_t  data[SFRW_BLK_SIZE];
    fsize_t fsze = file_sizeA(name);

    /* 过滤文件大小 */
    if (fsze == 0UL || fsze > SFRW_MAX_SIZE)
        return (FALSE);
    size = (int32u)fsze;
    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (FALSE);

    /* 发送协议头 */
    sfrw_set_head(&head, SFRW_DATA, size);
    back = socket_tcp_send(netw, &head, sizeof(head));
    if (back != sizeof(head))
        goto _failure;

    /* 等待远端响应 */
    back = socket_tcp_recv(netw, &head, sizeof(head));
    if (back != sizeof(head))
        goto _failure;

    /* 远端成功与否 */
    if (head.type != mk_tag4(SFRW_OKAY))
        goto _failure;

    /* 分块发送数据 */
    blks = (int32u)(size / SFRW_BLK_SIZE);
    rsts = (uint_t)(size % SFRW_BLK_SIZE);
    for (; blks != 0; blks--)
    {
        /* 读取文件数据 */
        back = (uint_t)file_raw_read(data, SFRW_BLK_SIZE, file);
        if (back != SFRW_BLK_SIZE)
            goto _failure;

        /* 发送一块数据 */
        back = socket_tcp_send(netw, data, SFRW_BLK_SIZE);
        if (back != SFRW_BLK_SIZE)
            goto _failure;

        /* 等待远端响应 */
        back = socket_tcp_recv(netw, &head, sizeof(head));
        if (back != sizeof(head))
            goto _failure;

        /* 远端成功与否 */
        if (head.type != mk_tag4(SFRW_OKAY))
            goto _failure;
    }

    /* 发送尾部数据 */
    if (rsts != 0)
    {
        /* 读取文件数据 */
        back = (uint_t)file_raw_read(data, rsts, file);
        if (back != rsts)
            goto _failure;
        back = socket_tcp_send(netw, data, rsts);
        if (back != rsts)
            goto _failure;

        /* 等待远端响应 */
        back = socket_tcp_recv(netw, &head, sizeof(head));
        if (back != sizeof(head))
            goto _failure;

        /* 远端成功与否 */
        if (head.type != mk_tag4(SFRW_OKAY))
            goto _failure;
    }
    file_raw_close(file);
    return (TRUE);

_failure:
    file_raw_close(file);
    return (FALSE);
}

/*
---------------------------------------
    SFRW TCP 接收数据
---------------------------------------
*/
static bool_t
sfrw_tcp_recv_data (
  __CR_IN__ socket_t    netw,
  __CR_OT__ sSFRW_HDR*  head,
  __CR_OT__ void_t**    data
    )
{
    int32u  blks;
    uint_t  rsts;
    uint_t  back;
    int32u  errs;
    byte_t* pntr;
    void_t* buff;

    /* 接收协议头 */
    back = socket_tcp_recv(netw, head, sizeof(sSFRW_HDR));
    if (back != sizeof(sSFRW_HDR)) {
        errs = __LINE__;
        goto _failure1;
    }

    /* 状态包返回 */
    head->size = sfrw_get_head(head);
    if (head->type == mk_tag4(SFRW_OKAY) ||
        head->type == mk_tag4(SFRW_FAIL)) {
        if (data != NULL)
            *data = NULL;
        return (TRUE);
    }

    /* 非法的参数 */
    if (data == NULL) {
        errs = __LINE__;
        goto _failure1;
    }
    *data = NULL;

    /* 分配包内存 */
    if (head->size == 0UL || head->size > SFRW_MAX_SIZE) {
        errs = __LINE__;
        goto _failure1;
    }
    buff = mem_malloc32(head->size);
    if (buff == NULL) {
        errs = __LINE__;
        goto _failure1;
    }

    /* 响应后接收后续数据 */
    if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
        goto _failure0;
    pntr = (byte_t*)buff;
    blks = (int32u)(head->size / SFRW_BLK_SIZE);
    rsts = (uint_t)(head->size % SFRW_BLK_SIZE);
    for (; blks != 0; blks--, pntr += SFRW_BLK_SIZE)
    {
        /* 接收一块数据 */
        back = socket_tcp_recv(netw, pntr, SFRW_BLK_SIZE);
        if (back != SFRW_BLK_SIZE) {
            errs = __LINE__;
            goto _failure2;
        }

        /* 发送接收响应 */
        if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
            goto _failure0;
    }

    /* 接收尾部数据 */
    if (rsts != 0) {
        back = socket_tcp_recv(netw, pntr, rsts);
        if (back != rsts) {
            errs = __LINE__;
            goto _failure2;
        }

        /* 发送接收响应 */
        if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
            goto _failure0;
    }
    *data = buff;
    return (TRUE);

_failure2:
    mem_free(buff);
_failure1:
    sfrw_tcp_send_data(netw, SFRW_FAIL, errs, NULL);
    return (FALSE);

_failure0:
    mem_free(buff);
    return (FALSE);
}

/*
---------------------------------------
    SFRW TCP 接收文件
---------------------------------------
*/
static bool_t
sfrw_tcp_recv_file (
  __CR_IN__ socket_t        netw,
  __CR_OT__ sSFRW_HDR*      head,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ int32u*         crc32
    )
{
    fraw_t  file;
    leng_t  wrte;
    uint_t  rsts, back;
    int32u  blks, errs = 0;
    byte_t  data[SFRW_BLK_SIZE];

    /* 接收协议头 */
    back = socket_tcp_recv(netw, head, sizeof(sSFRW_HDR));
    if (back != sizeof(sSFRW_HDR)) {
        errs = __LINE__;
        goto _failure1;
    }

    /* 必须是文件 */
    if (head->type != mk_tag4(SFRW_DATA)) {
        errs = __LINE__;
        goto _failure1;
    }

    /* 创建目标文件 */
    head->size = sfrw_get_head(head);
    if (head->size == 0UL || head->size > SFRW_MAX_SIZE) {
        errs = __LINE__;
        goto _failure1;
    }
    file = file_raw_openA(name, CR_FO_WO | CR_FO_SEQ);
    if (file == NULL) {
        errs = __LINE__;
        goto _failure1;
    }

    /* 响应后接收后续数据 */
    if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
        goto _failure0;
    if (crc32 != NULL)
        errs = hash_crc32i_init();
    blks = (int32u)(head->size / SFRW_BLK_SIZE);
    rsts = (uint_t)(head->size % SFRW_BLK_SIZE);
    for (; blks != 0; blks--)
    {
        /* 接收一块数据 */
        back = socket_tcp_recv(netw, data, SFRW_BLK_SIZE);
        if (back != SFRW_BLK_SIZE) {
            errs = __LINE__;
            goto _failure2;
        }
        wrte = file_raw_write(data, SFRW_BLK_SIZE, file);
        if (wrte != SFRW_BLK_SIZE) {
            errs = __LINE__;
            goto _failure2;
        }

        /* 发送接收响应 */
        if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
            goto _failure0;
        if (crc32 != NULL)
            errs = hash_crc32i_update(errs, data, SFRW_BLK_SIZE);
    }

    /* 接收尾部数据 */
    if (rsts != 0)
    {
        /* 接收剩余数据 */
        back = socket_tcp_recv(netw, data, rsts);
        if (back != rsts) {
            errs = __LINE__;
            goto _failure2;
        }
        wrte = file_raw_write(data, rsts, file);
        if (wrte != rsts) {
            errs = __LINE__;
            goto _failure2;
        }

        /* 发送接收响应 */
        if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
            goto _failure0;
        if (crc32 != NULL)
            errs = hash_crc32i_update(errs, data, rsts);
    }
    file_raw_close(file);
    if (crc32 != NULL)
        *crc32 = hash_crc32i_finish(errs);
    return (TRUE);

_failure2:
    file_raw_close(file);
    file_deleteA(name);
_failure1:
    sfrw_tcp_send_data(netw, SFRW_FAIL, errs, NULL);
    return (FALSE);

_failure0:
    file_raw_close(file);
    file_deleteA(name);
    return (FALSE);
}

/*
=======================================
    SFRW TCP 服务端
=======================================
*/
CR_API bool_t
sfrw_tcp_srv_loop (
  __CR_IN__ socket_t        netw,
  __CR_IN__ uint_t          flags,
  __CR_IN__ const ansi_t*   down,
  __CR_IN__ const ansi_t*   uppe
    )
{
    int32u      errs;
    void_t*     data;
    ansi_t*     file;
    sSFRW_HDR   head;
    sDATETIME   rtcx;
    sSFRW_TIME  date;
    sSFRW_INFO  info;

    /* 接收一个协议包头 */
    if (!sfrw_tcp_recv_data(netw, &head, &data))
        return (FALSE);

    /* 错误的命令包 */
    if (data == NULL)
        return (FALSE);

    /* 附加数据必须是字符串 */
    file = (ansi_t*)data;
    if (file[head.size - 1] != CR_AC(NIL)) {
        errs = __LINE__;
        goto _failure2;
    }

    /* 过滤非法文件名字符串 */
    if (str_strA(file, CR_AS("..")) != NULL) {
        errs = __LINE__;
        goto _failure2;
    }

    /* 处理各种服务请求 */
    if (head.type == mk_tag4(SFRW_FIND))
    {
        if (*file == CR_AC(':'))
        {
            /* 设备文件请求 */
            if (str_cmpA(file, CR_AS(":datetime")) != 0) {
                errs = __LINE__;
                goto _failure2;
            }

            /* 时间日期请求 */
            struct_zero(&rtcx, sDATETIME);
            if (!datetime_get(&rtcx)) {
                errs = __LINE__;
                goto _failure2;
            }
            date.year   = (int16u)rtcx.year;
            date.year   = WORD_BE(date.year);
            date.month  = (byte_t)rtcx.month;
            date.day    = (byte_t)rtcx.day;
            date.hour   = (byte_t)rtcx.hour;
            date.minute = (byte_t)rtcx.minute;
            date.second = (byte_t)rtcx.second;
            date.week   = (byte_t)rtcx.week;
            if (!sfrw_tcp_send_data(netw, SFRW_INFO, sizeof(date), &date))
                goto _failure1;
        }
        else
        {
            /* 普通文件请求 */
            file = path_appendA(down, file);
            if (file == NULL) {
                errs = __LINE__;
                goto _failure2;
            }
            mem_free(data);
            data = (void_t*)path_uniqueA(file);
            if (!sfrw_get_finfo(&info, file)) {
                errs = __LINE__;
                goto _failure2;
            }
            info.size  = DWORD_BE(info.size);
            info.crc32 = DWORD_BE(info.crc32);
            if (!sfrw_tcp_send_data(netw, SFRW_INFO, sizeof(info), &info))
                goto _failure1;
        }
    }
    else
    if ((flags & SFRW_DOWN) && (head.type == mk_tag4(SFRW_PULL)))
    {
        /* 下载文件请求 */
        file = path_appendA(down, file);
        if (file == NULL) {
            errs = __LINE__;
            goto _failure2;
        }
        mem_free(data);
        data = (void_t*)path_uniqueA(file);
        if (!sfrw_tcp_send_file(netw, file))
            goto _failure1;
    }
    else
    if ((flags & SFRW_UPPE) && (head.type == mk_tag4(SFRW_PUSH)))
    {
        /* 上传文件请求 */
        file = path_appendA(uppe, file);
        if (file == NULL) {
            errs = __LINE__;
            goto _failure2;
        }
        mem_free(data);
        data = (void_t*)path_uniqueA(file);
        if (!sfrw_tcp_send_data(netw, SFRW_OKAY, 0, NULL))
            goto _failure1;
        if (!sfrw_tcp_recv_file(netw, &head, file, &errs))
            goto _failure1;

        /* 校验文件 */
        if (!sfrw_get_finfo(&info, file) ||
            info.crc32 != errs || info.size != head.size) {
            file_deleteA(file);
            goto _failure1;
        }
    }
    else
    {
        /* 无法支持的服务请求 */
        errs = __LINE__;
        goto _failure2;
    }
    mem_free(data);
    return (TRUE);

_failure2:
    sfrw_tcp_send_data(netw, SFRW_FAIL, errs, NULL);
_failure1:
    mem_free(data);
    return (FALSE);
}

/*
=======================================
    SFRW TCP 查询文件
=======================================
*/
CR_API bool_t
sfrw_tcp_cli_find (
  __CR_IN__ socket_t        netw,
  __CR_OT__ sSFRW_INFO*     info,
  __CR_IN__ const ansi_t*   path
    )
{
    leng_t      size;
    void_t*     data;
    sSFRW_HDR   head;
    sDATETIME   rtcx;
    sSFRW_INFO* find;
    sSFRW_TIME* date;

    /* 发送命令 */
    size = str_sizeA(path);
    if (!sfrw_tcp_send_data(netw, SFRW_FIND, (int32u)size, path))
        return (FALSE);

    /* 接收响应 */
    if (!sfrw_tcp_recv_data(netw, &head, &data))
        return (FALSE);

    /* 过滤数据 */
    if ((data == NULL) ||
        (head.type != mk_tag4(SFRW_INFO)) ||
        (head.size != sizeof(sSFRW_TIME) &&
         head.size != sizeof(sSFRW_INFO))) {
        TRY_FREE(data);
        return (FALSE);
    }

    /* 根据大小执行动作 */
    if (str_cmpA(path, CR_AS(":datetime")) == 0)
    {
        /* 设置时间 */
        if (info != NULL) {
            info->size  = sizeof(sSFRW_TIME);
            info->crc32 = hash_crc32i_total(data, sizeof(sSFRW_TIME));
        }
        date = (sSFRW_TIME*)data;
        rtcx.year   =  WORD_BE(date->year);
        rtcx.month  = (ufast_t)date->month;
        rtcx.day    = (ufast_t)date->day;
        rtcx.hour   = (ufast_t)date->hour;
        rtcx.minute = (ufast_t)date->minute;
        rtcx.second = (ufast_t)date->second;
        rtcx.week   = (ufast_t)date->week;
        if (!datetime_set(&rtcx))
            goto _failure;
    }
    else
    {
        /* 返回信息 */
        if (info == NULL)
            goto _failure;
        find = (sSFRW_INFO*)data;
        info->size  = DWORD_BE(find->size);
        info->crc32 = DWORD_BE(find->crc32);
    }
    mem_free(data);
    return (TRUE);

_failure:
    mem_free(data);
    return (FALSE);
}

/*
=======================================
    SFRW TCP 下载文件
=======================================
*/
CR_API bool_t
sfrw_tcp_cli_pull (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   path,
  __CR_IN__ const ansi_t*   name
    )
{
    leng_t      size;
    int32u      crc4;
    sSFRW_HDR   head;
    sSFRW_INFO  info;

    /* 发送命令 */
    size = str_sizeA(path);
    if (!sfrw_tcp_send_data(netw, SFRW_PULL, (int32u)size, path))
        return (FALSE);

    /* 接收响应 */
    if (!sfrw_tcp_recv_file(netw, &head, name, &crc4))
        return (FALSE);

    /* 校验文件 */
    if (!sfrw_get_finfo(&info, name) ||
        info.crc32 != crc4 || info.size != head.size) {
        file_deleteA(name);
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    SFRW TCP 上传文件
=======================================
*/
CR_API bool_t
sfrw_tcp_cli_push (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   path,
  __CR_IN__ const ansi_t*   name
    )
{
    leng_t      size;
    void_t*     data;
    sSFRW_HDR   head;

    /* 发送命令 */
    size = str_sizeA(path);
    if (!sfrw_tcp_send_data(netw, SFRW_PUSH, (int32u)size, path))
        return (FALSE);

    /* 等待响应 */
    if (!sfrw_tcp_recv_data(netw, &head, &data))
        return (FALSE);

    /* 过滤数据 */
    if (data != NULL ||
        head.size != 0UL ||
        head.type != mk_tag4(SFRW_OKAY)) {
        TRY_FREE(data);
        return (FALSE);
    }

    /* 发送文件 */
    return (sfrw_tcp_send_file(netw, name));
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
