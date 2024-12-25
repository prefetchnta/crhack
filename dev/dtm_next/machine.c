/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-30  */
/*     #######          ###    ###      [SOFT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 手持机操作函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "encdec.h"
#include "memlib.h"
#include "morder.h"
#include "mtplib.h"
#include "machine.h"
#include "util/commlib.h"

/* 版本号最大长度 */
#define MAXVL   64

/*****************************************************************************/
/*                                 公用部分                                  */
/*****************************************************************************/

/*
=======================================
    初始化 RS232 串口
=======================================
*/
CR_API bool_t
rs232_init (
  __CR_IN__ uint_t  port,
  __CR_IN__ int32u  baud,
  __CR_IN__ uint_t  timeout
    )
{
    /* 打开串口 */
    if (!sio_open(port))
        return (FALSE);

    /* 设置串口参数 */
    if (!sio_setup(port, baud, 8, CR_SIO_NOP, CR_SIO_STOP10))
        goto _failure;

    /* 设置读写超时 */
    if (!sio_set_wr_timeout(port, 0, 0))
        goto _failure;
    if (!sio_set_rd_timeout(port, 0, 0, timeout))
        goto _failure;

    /* 设置收发缓冲 */
    if (!sio_set_buffer(port, 1200, 1200))
        goto _failure;

    /* 刷新读写缓冲 */
    if (!sio_flush(port, CR_SIO_FLU_RT))
        goto _failure;
    return (TRUE);

_failure:
    sio_close(port);
    return (FALSE);
}

/*
=======================================
    电源盒语音播放
=======================================
*/
CR_API bool_t
power_play_voice (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  index
    )
{
    byte_t  frame[6];

    frame[0] = 0xB0;
    frame[1] = 0x00;
    frame[2] = 0x01;
    frame[3] = 0x37;
    frame[4] = index;
    frame[5] = frame[3] + frame[4];
    return (dtmo_sio_send(port, frame, sizeof(frame)));
}

/*
---------------------------------------
    获取南昌 GPS 设备单项数据
---------------------------------------
*/
static bool_t
gps_nc_get_item (
  __CR_IN__ uint_t      port,
  __CR_OT__ void_t*     data,
  __CR_IN__ byte_t      index,
  __CR_IN__ uint_t      osize,
  __CR_OT__ sGPS_NC*    gps_data
    )
{
    byte_t  temp[5 + 2];
    uint_t  size = sizeof(temp);

    if (!sdt22_sio_iorw(port, 1, 0, temp, &size, &index, 1, NULL))
        return (FALSE);
    if (temp[0] == index && size == 1) {
        gps_data->gps_locate = FALSE;
        return (TRUE);
    }
    if (temp[0] != index || size != 1 + osize)
        return (FALSE);
    gps_data->gps_locate = TRUE;
    mem_cpy(data, &temp[1], osize);
    return (TRUE);
}

/*
=======================================
    获取南昌 GPS 设备数据
=======================================
*/
CR_API bool_t
gps_nc_get_data (
  __CR_IN__ uint_t      port,
  __CR_OT__ sGPS_NC*    gps_data
    )
{
    /* 经度 (分) */
    if (!gps_nc_get_item(port, &gps_data->longitude, 1, 4, gps_data))
        return (FALSE);
    gps_data->longitude = DWORD_BE(gps_data->longitude);

    /* 纬度 (分) */
    if (!gps_nc_get_item(port, &gps_data->latitude, 2, 4, gps_data))
        return (FALSE);
    gps_data->latitude = DWORD_BE(gps_data->latitude);

    /* 速度 (km/h) */
    if (!gps_nc_get_item(port, &gps_data->velocity, 3, 1, gps_data))
        return (FALSE);

    /* 方向 (度) */
    if (!gps_nc_get_item(port, &gps_data->direction, 4, 2, gps_data))
        return (FALSE);
    gps_data->direction = WORD_BE(gps_data->direction);

    /* 时间 (时分秒) */
    return (gps_nc_get_item(port, &gps_data->gps_time, 5, 3, gps_data));
}

/*****************************************************************************/
/*                              DTM_NEW 手持机                               */
/*****************************************************************************/

/*
=======================================
    手持机是否在线
=======================================
*/
CR_API bool_t
dtm_new_online (
  __CR_IN__ uint_t  port
    )
{
    byte_t  temp[2];
    int16u  retcode;
    uint_t  size = sizeof(temp);

    if (!dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_ONLINE,
            temp, &size, &retcode, 0, 0, NULL, 0, NULL))
        return (FALSE);
    return ((size != 0 || retcode != 0) ? FALSE : TRUE);
}

/*
=======================================
    手持机参数初始化
=======================================
*/
CR_API bool_t
dtm_new_areset (
  __CR_IN__ uint_t  port
    )
{
    byte_t  temp[2];
    int16u  retcode;
    uint_t  size = sizeof(temp);

    if (!dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_ARESET,
            temp, &size, &retcode, 0, 0, NULL, 0, NULL))
        return (FALSE);
    return ((size != 0 || retcode != 0) ? FALSE : TRUE);
}

/*
=======================================
    手持机清训练数据
=======================================
*/
CR_API bool_t
dtm_new_tclear (
  __CR_IN__ uint_t  port
    )
{
    byte_t  temp[2];
    int16u  retcode;
    uint_t  size = sizeof(temp);

    if (!dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_TCLEAR,
            temp, &size, &retcode, 0, 0, NULL, 0, NULL))
        return (FALSE);
    return ((size != 0 || retcode != 0) ? FALSE : TRUE);
}

/*
=======================================
    手持机返回版本串
=======================================
*/
CR_API ansi_t*
dtm_new_version (
  __CR_IN__ uint_t  port
    )
{
    int16u  retcode;
    ansi_t  temp[MAXVL + 2];
    uint_t  size = sizeof(temp);

    if (!dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_VERSION,
            temp, &size, &retcode, 0, 0, NULL, 0, NULL))
        return (NULL);
    if (size <= 1 || retcode != 0 || temp[size - 1] != CR_AC(NIL))
        return (NULL);
    return (str_dupA(temp));
}

/*
=======================================
    读取手持机机内参数 (索引)
=======================================
*/
CR_API bool_t
dtm_new_bin_read (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  type,
  __CR_IN__ int16u  index,
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    int16u  retce;
    uint_t  ssize;
    byte_t* sstru;

    if (size > 65535 - sizeof(int16u))
        return (FALSE);
    ssize = size + sizeof(int16u);
    sstru = (byte_t*)mem_malloc(ssize);
    if (sstru == NULL)
        return (FALSE);

    /* 类型和索引为最大表示对日期时间操作 */
    if (type == 0xFF && index == 0xFFFF) {
        if (!dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_GETTIME,
                sstru, &ssize, &retce, 0, 0, NULL, 0, NULL))
            goto _failure;
    }
    else {
        if (!dtmn_sio_iorw(port, NCMD_BINEE_RD, type,
                sstru, &ssize, &retce, 0, index, NULL, 0, NULL))
            goto _failure;
    }
    if (ssize != size || retce != 0)
        goto _failure;
    mem_cpy(data, sstru, size);
    mem_free(sstru);
    return (TRUE);

_failure:
    mem_free(sstru);
    return (FALSE);
}

/*
=======================================
    写入手持机机内参数 (索引)
=======================================
*/
CR_API bool_t
dtm_new_bin_write (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          type,
  __CR_IN__ int16u          index,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    byte_t  temp[2];
    int16u  retcode;
    uint_t  tsize = sizeof(temp);

    /* 类型和索引为最大表示对日期时间操作 */
    if (type == 0xFF && index == 0xFFFF) {
        if (!dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_SETTIME,
                temp, &tsize, &retcode, 0, 0, data, size, NULL))
            return (FALSE);
    }
    else {
        if (!dtmn_sio_iorw(port, NCMD_BINEE_WR, type,
                temp, &tsize, &retcode, 0, index, data, size, NULL))
            return (FALSE);
    }
    return ((tsize != 0 || retcode != 0) ? FALSE : TRUE);
}

/*
=======================================
    读取手持机机内参数 (文件)
=======================================
*/
CR_API bool_t
dtm_new_txt_read (
  __CR_IN__ uint_t          port,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ void_t*         data,
  __CR_IN__ uint_t          size
    )
{
    int16u  retce;
    leng_t  nsize;
    uint_t  ssize;
    byte_t* sstru;

    nsize = str_lenA(name);
    if (nsize >= 65535)
        return (FALSE);
    nsize += 1;
    if (size > 65535 - sizeof(int16u))
        return (FALSE);
    ssize = size + sizeof(int16u);
    sstru = (byte_t*)mem_malloc(ssize);
    if (sstru == NULL)
        return (FALSE);
    if (!dtmn_sio_iorw(port, NCMD_TXTEE_RD, NCMD_TXTEE_RD,
            sstru, &ssize, &retce, 0, 0, name, (int16u)nsize, NULL))
        goto _failure;
    if (ssize != size || retce != 0)
        goto _failure;
    mem_cpy(data, sstru, size);
    mem_free(sstru);
    return (TRUE);

_failure:
    mem_free(sstru);
    return (FALSE);
}

/*
=======================================
    写入手持机机内参数 (文件)
=======================================
*/
CR_API bool_t
dtm_new_txt_write (
  __CR_IN__ uint_t          port,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    leng_t  ssend;
    byte_t* dsend;
    leng_t  nsize;
    uint_t  tsize;
    byte_t  temp[2];
    int16u  retcode;

    nsize = str_lenA(name);
    if (nsize >= 65535)
        return (FALSE);
    nsize += 1;
    if ((leng_t)size > 65535 - nsize)
        return (FALSE);
    ssend = nsize + size;
    dsend = (byte_t*)mem_malloc(ssend);
    if (dsend == NULL)
        return (FALSE);
    tsize = sizeof(temp);
    mem_cpy(&dsend[0], name, nsize);
    mem_cpy(&dsend[nsize], data, size);
    if (!dtmn_sio_iorw(port, NCMD_TXTEE_WR, NCMD_TXTEE_WR,
            temp, &tsize, &retcode, 0, 0, dsend, (int16u)ssend, NULL))
        goto _failure;
    if (tsize != 0 || retcode != 0)
        goto _failure;
    mem_free(dsend);
    return (TRUE);

_failure:
    mem_free(dsend);
    return (FALSE);
}

/*
=======================================
    搜索并打开手持机
=======================================
*/
CR_API uint_t
dtm_new_search (
  __CR_IN__ uint_t  port_max,
  __CR_IN__ uint_t  timeout,
  __CR_OT__ int32u* baud
    )
{
    byte_t  temp[2];
    int16u  retcode;
    uint_t  port, size;

    /* 搜索指定个串口号 */
    for (port = 1; port <= port_max; port++)
    {
        /* 初始化串口 */
        if (!rs232_init(port, 115200UL, timeout))
            continue;

        /* 进行联机测试 */
        size = sizeof(temp);
        if (dtmn_sio_iorw(port, NCMD_STATE, NCMD_STATE_ONLINE,
                temp, &size, &retcode, 0, 0, NULL, 0, NULL)) {
            if (size == 0 && retcode == 0) {
                if (baud != NULL)
                    *baud = 115200UL;
                return (port);  /* 退出时串口处于打开状态 */
            }
        }
        sio_close(port);
    }
    return ((uint_t)0);     /* 返回0表示未找到设备 */
}

/* 程序下载内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 下载程序参数结构 (LE) */
CR_TYPEDEF struct
{
/*000*/ int32u  addr;   /* 下载起始地址 */
/*004*/ int32u  size;   /* 下载文件大小 (对齐到8字节) */
/*008*/ int32u  crc32;  /* 下载程序的校验 */
/*012*/
} CR_PACKED sNFLASH_DOWN;

#define NFLASH_DOWN_RETRY   3       /* 重试次数 */
#define NFLASH_DOWN_SPLITE  1024    /* 下载分块 */

/* 下载程序数据结构 (LE) */
CR_TYPEDEF struct
{
/*000*/ int32u  offset;                     /* 当前数据的偏移 */
/*004*/ byte_t  data[NFLASH_DOWN_SPLITE];   /* 分块下载的数据 */
/*NNN*/
} CR_PACKED sNFLASH_DATA;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    下载手持机程序
=======================================
*/
CR_API bool_t
dtm_new_download (
  __CR_IN__ uint_t          port,
  __CR_IN__ int32u          addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const sPROBAR*  probar
    )
{
    leng_t          blks;
    leng_t          rsts;
    byte_t*         pntr;
    byte_t          op[2];
    byte_t          frnum;
    int32u          offst;
    int16u          retce;
    int16u          isize;
    uint_t          osize;
    uint_t          index;
    sNFLASH_DOWN    param;
    sNFLASH_DATA    block;

    /* 过滤参数 (大小为0也走流程) */
    if (size > CR_K2B(512))
        return (FALSE);

    /* 下发下载程序的参数 */
    sio_set_rd_timeout(port, 0, 0, 2000);
    param.addr  = DWORD_LE((int32u)addr);
    param.size  = DWORD_LE((int32u)size);
    param.crc32 = hash_crc32i_total(data, size);
    param.crc32 = DWORD_LE(param.crc32);
    for (index = 0; index < NFLASH_DOWN_RETRY; index++) {
        osize = (uint_t)(sizeof(op));
        isize = (int16u)(sizeof(param));
        if (dtmn_sio_iorw(port, NCMD_FLASH, NCMD_FLASH_PARAM,
                op, &osize, &retce, 0, 0, &param, isize, NULL)) {
            if (osize == 0 && retce == 0) {
                thread_sleep(50);
                break;
            }
        }
        thread_sleep(100);
    }
    if (index >= NFLASH_DOWN_RETRY)
        return (FALSE);

    /* 下发下载程序的数据 */
    frnum = 0;
    offst = 0UL;
    if (probar != NULL && probar->init != NULL)
        probar->init(probar->param, size);
    pntr = (byte_t*)data;
    blks = size / NFLASH_DOWN_SPLITE;
    rsts = size % NFLASH_DOWN_SPLITE;
    while (blks-- != 0) {
        block.offset = DWORD_LE(offst);
        mem_cpy(block.data, pntr, NFLASH_DOWN_SPLITE);
        for (index = 0; index < NFLASH_DOWN_RETRY; index++) {
            osize = (uint_t)(sizeof(op));
            isize = (int16u)(sizeof(block));
            if (dtmn_sio_iorw(port, NCMD_FLASH, NCMD_FLASH_BLOCK,
                    op, &osize, &retce, frnum, 0, &block, isize, NULL)) {
                if (osize == 0 && retce == 0) {
                    thread_sleep(50);
                    break;
                }
            }
            thread_sleep(100);
        }
        if (index >= NFLASH_DOWN_RETRY)
            return (FALSE);
        pntr  += NFLASH_DOWN_SPLITE;
        offst += NFLASH_DOWN_SPLITE;
        if (probar != NULL && probar->update != NULL)
            probar->update(probar->param, NFLASH_DOWN_SPLITE);
        frnum++;
    }

    /* 下发剩余分块的数据 */
    if (rsts != 0) {
        block.offset = DWORD_LE(offst);
        mem_cpy(block.data, pntr, rsts);
        for (index = 0; index < NFLASH_DOWN_RETRY; index++) {
            osize = (uint_t)(sizeof(op));
            isize = (int16u)(sizeof(block) - NFLASH_DOWN_SPLITE + rsts);
            if (dtmn_sio_iorw(port, NCMD_FLASH, NCMD_FLASH_BLOCK,
                    op, &osize, &retce, frnum, 0, &block, isize, NULL)) {
                if (osize == 0 && retce == 0) {
                    thread_sleep(50);
                    break;
                }
            }
            thread_sleep(100);
        }
        if (index >= NFLASH_DOWN_RETRY)
            return (FALSE);
        if (probar != NULL && probar->update != NULL)
            probar->update(probar->param, rsts);
    }

    /* 发送最后烧写程序的指令 */
    osize = (uint_t)(sizeof(op));
    sio_set_rd_timeout(port, 0, 0, 40000);
    if (!dtmn_sio_iorw(port, NCMD_FLASH, NCMD_FLASH_BURNIT,
            op, &osize, &retce, 0, 0, NULL, 0, NULL))
        return (FALSE);
    if (osize != 0 || retce != 0)
        return (FALSE);
    if (probar != NULL && probar->finish != NULL)
        probar->finish(probar->param, size);
    return (TRUE);
}

/*****************************************************************************/
/*                              DTM_OLD 手持机                               */
/*****************************************************************************/

/* 老手持机通讯相关 */
static const int32u _rom_ s_baud_old[] =
{
    115200UL, 57600UL, 9600UL,
};
static const byte_t _rom_ s_online[] =
{
    OCMD_BYTE1_ONLINE, OCMD_BYTE2_ONLINE, 0x00, 0x00,
};
static const byte_t _rom_ s_areset[] =
{
    OCMD_BYTE1_ARESET, OCMD_BYTE2_ARESET, 0x00, 0x00,
};
static const byte_t _rom_ s_version[] =
{
    OCMD_BYTE1_VERSION, OCMD_BYTE2_VERSION, 0x00, 0x00,
};

/* 程序下载内部结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 手持机返回结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  cmd1;   /* 通讯命令01 */
/*001*/ byte_t  cmd2;   /* 通讯命令02 */
/*002*/ byte_t  zero;   /* 保留的字段 */
/*003*/ byte_t  retc;   /* 返回错误码 */
/*004*/ byte_t  sum8;   /* 帧数据校验 */
/*005*/
} CR_PACKED sODTM_RET;

/* 下载程序参数结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  cmd1;   /* 通讯命令01 */
/*001*/ byte_t  cmd2;   /* 通讯命令02 */
/*002*/ int32u  size1;  /* 第一块大小 */
/*006*/ int32u  size2;  /* 第二块大小 */
/*010*/
} CR_PACKED sOFLASH_DOWN;

#define OFLASH_DOWN_RETRY   3       /* 重试次数 */
#define OFLASH_DOWN_SPLITE  1024    /* 下载分块 */

/* 下载程序数据结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  cmd1;                       /* 通讯命令01 */
/*001*/ byte_t  cmd2;                       /* 通讯命令02 */
/*002*/ int16u  numb;                       /* 数据包编号 */
/*004*/ int16u  size;                       /* 数据包大小 */
/*006*/ byte_t  data[OFLASH_DOWN_SPLITE];   /* 下载的数据 */
/*NNN*/
} CR_PACKED sOFLASH_DATA;

/* 下载程序类型结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  cmd1;   /* 通讯命令01 */
/*001*/ byte_t  cmd2;   /* 通讯命令02 */
/*002*/ byte_t  type;   /* 下载的类型 */
/*003*/ byte_t  sum8;   /* 文件总校验 */
/*004*/
} CR_PACKED sOFLASH_TYPE;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    手持机参数初始化
=======================================
*/
CR_API bool_t
dtm_old_areset (
  __CR_IN__ uint_t  port
    )
{
    byte_t  temp[4 + 1];
    uint_t  size = sizeof(temp);

    if (!dtmo_sio_iorw(port, temp, &size, s_areset,
                       sizeof(s_areset) - 2, NULL))
        return (FALSE);
    if (size != sizeof(s_areset) ||
        mem_cmp(temp, s_areset, sizeof(s_areset)) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    手持机返回版本串
=======================================
*/
CR_API ansi_t*
dtm_old_version (
  __CR_IN__ uint_t  port
    )
{
    ansi_t  *version;
    byte_t  key[16], temp[MAXVL + 1];
    uint_t  size = sizeof(temp);

    if (!dtmo_sio_iorw(port, temp, &size, s_version,
                       sizeof(s_version) - 2, NULL))
        return (NULL);
    if (size <= sizeof(s_version) ||
        mem_cmp(temp, s_version, sizeof(s_version)) != 0)
        return (NULL);
    size = size - sizeof(s_version) + 1;
    version = (ansi_t*)temp + sizeof(s_version);
    if (!dtmo_api_search(key, version, size))
        return (NULL);
    if (version[size - 2] != CR_AC(NIL))
        return (NULL);
    return (str_dupA(version));
}

/*
=======================================
    搜索并打开手持机
=======================================
*/
CR_API uint_t
dtm_old_search (
  __CR_IN__ uint_t  port_max,
  __CR_IN__ uint_t  timeout,
  __CR_OT__ int32u* baud
    )
{
    byte_t  temp[4 + 1];
    uint_t  bb, port, size;

    /* 自动搜索串口参数 */
    for (bb = 0; bb < cntsof(s_baud_old); bb++)
    {
        for (port = 1; port <= port_max; port++)
        {
            /* 初始化串口 */
            if (!rs232_init(port, s_baud_old[bb], timeout))
                continue;

            /* 进行联机测试 */
            size = sizeof(temp);
            if (dtmo_sio_iorw(port, temp, &size, s_online,
                              sizeof(s_online) - 2, NULL)) {
                if (size == sizeof(s_online) &&
                    mem_cmp(temp, s_online, sizeof(s_online)) == 0) {
                    if (baud != NULL)
                        *baud = s_baud_old[bb];
                    return (port);  /* 退出时串口处于打开状态 */
                }
            }
            sio_close(port);
        }
    }
    return ((uint_t)0);     /* 返回0表示未找到设备 */
}

/*
=======================================
    下载手持机程序
=======================================
*/
CR_API bool_t
dtm_old_download (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          type,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          shzk,
  __CR_IN__ const sPROBAR*  probar
    )
{
    leng_t          blks;
    leng_t          rsts;
    byte_t*         pntr;
    byte_t          tag1;
    byte_t          tag2;
    byte_t          sum8;
    int16u          frnum;
    int16u          isize;
    uint_t          osize;
    uint_t          index;
    sODTM_RET       srett;
    sODTM_RET       scomp;
    sOFLASH_DOWN    param;
    sOFLASH_DATA    block;
    sOFLASH_TYPE    dburn;

    /* 过滤参数 */
    if (size == 0 || size >= CR_M2B(1))
        return (FALSE);
    switch (type)
    {
        case DTM_OLD_DOWN_AT9_BOOT:
            shzk = 0;
            break;

        case DTM_OLD_DOWN_AT9_MAIN:
            if (shzk == 0)
                return (FALSE);
            break;

        case DTM_OLD_DOWN_DSP_MAIN:
        case DTM_OLD_DOWN_DSP_ZK12:
            shzk = 0;
            type -= 1;
            break;

        default:
            return (FALSE);
    }
    if (shzk > size || shzk > CR_K2B(512))
        return (FALSE);

    /* 计算校验和 */
    tag1 = 0x0A;
    tag2 = 0xA0;
    sum8 = ((byte_t*)data)[0];
    for (blks = 1; blks < size; blks++)
        sum8 = sum8 + ((byte_t*)data)[blks];
    size -= shzk;
    scomp.zero = scomp.retc = 0x00;
    sio_set_rd_timeout(port, 0, 0, 150);

    /* 下发设置程序的参数 */
    param.size1 = DWORD_BE((int32u)size);
    param.size2 = DWORD_BE((int32u)shzk);
_retry:
    param.cmd1 = scomp.cmd1 = OCMD_BYTE1_BOOTLDR | tag1;
    param.cmd2 = scomp.cmd2 = OCMD_BYTE2_BOOTLDR | tag2;
    for (index = 0; index < OFLASH_DOWN_RETRY; index++) {
        osize = (uint_t)(sizeof(srett));
        isize = (int16u)(sizeof(param));
        if (!dtmo_sio_iorw(port, &srett, &osize, &param, isize, NULL))
        {
            /* 上海手持机下载需要连接两次并切换波特率 */
            if (tag1 == 0x00 && sio_get_baud(port) == 9600L)
                sio_setup(port, 115200UL, 8, CR_SIO_NOP, CR_SIO_STOP10);
        }
        else if (osize == sizeof(srett) - 1 &&
                 mem_cmp(&srett, &scomp, sizeof(srett) - 1) == 0) {
            thread_sleep(50);
            break;
        }
        thread_sleep(100);
    }
    if (index >= OFLASH_DOWN_RETRY) {
        if (tag1 == 0x00)
            return (FALSE);
        tag1 = 0x00;
        tag2 = 0x30;
        goto _retry;
    }

    /* 下发下载程序的数据 */
    frnum = 0;
    size += shzk;
    sio_set_rd_timeout(port, 0, 0, 2000);
    if (probar != NULL && probar->init != NULL)
        probar->init(probar->param, size);
    pntr = (byte_t*)data;
    blks = size / OFLASH_DOWN_SPLITE;
    rsts = size % OFLASH_DOWN_SPLITE;
    block.size = CWORD_BE(OFLASH_DOWN_SPLITE);
    block.cmd1 = scomp.cmd1 = OCMD_BYTE1_DPACKET | tag1;
    block.cmd2 = scomp.cmd2 = OCMD_BYTE2_DPACKET | tag2;
    while (blks-- != 0) {
        block.numb = WORD_BE(frnum);
        mem_cpy(block.data, pntr, OFLASH_DOWN_SPLITE);
        for (index = 0; index < OFLASH_DOWN_RETRY; index++) {
            osize = (uint_t)(sizeof(srett));
            isize = (int16u)(sizeof(block));
            if (dtmo_sio_iorw(port, &srett, &osize, &block, isize, NULL)) {
                if (osize == sizeof(srett) - 1 &&
                    mem_cmp(&srett, &scomp, sizeof(srett) - 1) == 0) {
                    thread_sleep(50);
                    break;
                }
            }
            thread_sleep(100);
        }
        if (index >= OFLASH_DOWN_RETRY)
            return (FALSE);
        pntr += OFLASH_DOWN_SPLITE;
        if (probar != NULL && probar->update != NULL)
            probar->update(probar->param, OFLASH_DOWN_SPLITE);
        frnum++;
    }

    /* 下发剩余分块的数据 */
    if (rsts != 0) {
        block.numb = WORD_BE((int16u)frnum);
        block.size = WORD_BE((int16u)rsts);
        mem_cpy(block.data, pntr, rsts);
        for (index = 0; index < OFLASH_DOWN_RETRY; index++) {
            osize = (uint_t)(sizeof(srett));
            isize = (int16u)(sizeof(block) - OFLASH_DOWN_SPLITE + rsts);
            if (dtmo_sio_iorw(port, &srett, &osize, &block, isize, NULL)) {
                if (osize == sizeof(srett) - 1 &&
                    mem_cmp(&srett, &scomp, sizeof(srett) - 1) == 0) {
                    thread_sleep(50);
                    break;
                }
            }
            thread_sleep(100);
        }
        if (index >= OFLASH_DOWN_RETRY)
            return (FALSE);
        if (probar != NULL && probar->update != NULL)
            probar->update(probar->param, rsts);
    }

    /* 发送最后烧写程序的指令 */
    dburn.type = type;
    dburn.sum8 = sum8;
    osize = (uint_t)(sizeof(srett));
    isize = (int16u)(sizeof(dburn));
    sio_set_rd_timeout(port, 0, 0, 40000);
    dburn.cmd1 = scomp.cmd1 = OCMD_BYTE1_WRFLASH | tag1;
    dburn.cmd2 = scomp.cmd2 = OCMD_BYTE2_WRFLASH | tag2;
    if (!dtmo_sio_iorw(port, &srett, &osize, &dburn, isize, NULL))
        return (FALSE);
    if (osize != sizeof(srett) - 1 ||
        mem_cmp(&srett, &scomp, sizeof(srett) - 1) != 0)
        return (FALSE);
    if (probar != NULL && probar->finish != NULL)
        probar->finish(probar->param, size);
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
