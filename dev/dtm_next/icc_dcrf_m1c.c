/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-11  */
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
/*  >>>>>>>>>>>>> DTM_NEXT DCRF S50/S70 ICCARD 设备驱动函数库 <<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "iccard.h"
#include "memlib.h"
#include "plugin.h"

/* DCRF S50/S70 ICCARD 接口函数 */
typedef void_t* (STDCALL *dc_init_t) (int16s, int32u);
typedef int16s  (STDCALL *dc_exit_t) (void_t*);
typedef int16s  (STDCALL *dc_card_t) (void_t*, byte_t, int32u*);
typedef int16s  (STDCALL *dc_read_t) (void_t*, byte_t, void_t*);
typedef int16s  (STDCALL *dc_write_t) (void_t*, byte_t, const void_t*);
typedef int16s  (STDCALL *dc_access_t) (void_t*, byte_t, byte_t,
                                        const void_t*);
typedef int16s  (STDCALL *dc_change_t) (void_t*, byte_t, const void_t*,
                                        byte_t, byte_t, byte_t, byte_t,
                                        byte_t, const void_t*);
/* DCRF S50/S70 ICCARD 设备内部结构 */
typedef struct
{
        /* 通用部分 */
        sDEV_BLOCK  devb;

        /* 个性部分 */
        sbin_t  m_sbin;         /* 模块句柄 */
        leng_t  m_size;         /* 文件大小 */
        int32u  m_port;         /* 端口编号 */
        int32u  m_baud;         /* 通讯速度 */
        byte_t  m_key[40][6];   /* 扇区密钥 */

        /* 模块接口函数 */
        dc_init_t   dc_init;    /* 打开设备 */
        dc_exit_t   dc_exit;    /* 关闭设备 */
        dc_card_t   dc_card;    /* 寻卡操作 */
        dc_read_t   dc_read;    /* 读取数据 */
        dc_write_t  dc_write;   /* 写入数据 */
        dc_access_t dc_access;  /* 密码认证 */
        dc_change_t dc_change;  /* 密码修改 */

} sICC_DCRF;

/*****************************************************************************/
/*                                 通用部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    DCRF M1 计算块和偏移
---------------------------------------
*/
static byte_t
icc_m1_block (
  __CR_IN__ leng_t  addr,
  __CR_OT__ uint_t* offset
    )
{
    *offset = (uint_t)(addr % 16);
    if (addr < (32 * 3 - 1) * 16)
        return ((byte_t)(addr / 16 + (addr / 16 + 1) / 3 + 1));
    addr -= (32 * 3 - 1) * 16;
    return ((byte_t)(addr / 16 + addr / 16 / 15 + 128));
}

/*
---------------------------------------
    DCRF 读取 M1 IC 卡数据
---------------------------------------
*/
static bool_t
icc_m1_blk_read (
  __CR_IO__ sICC_DCRF*  real,
  __CR_IN__ void_t*     dcrf,
  __CR_OT__ void_t*     data,
  __CR_IN__ leng_t      addr,
  __CR_IN__ leng_t      size
    )
{
    int32u  snr;
    byte_t  temp[16];
    byte_t  idx, *ptr;
    byte_t  blk1, blk2;
    byte_t  sec1, sec2;
    uint_t  off1, off2;

    /* 必须先寻卡 */
    if (real->dc_card(dcrf, 1, &snr) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_FIND;
        return (FALSE);
    }
    blk1 = icc_m1_block(addr + 0x00, &off1);
    blk2 = icc_m1_block(addr + size, &off2);

    /* 处理大扇区 */
    ptr = (byte_t*)data;
    if (blk1 < 128)
        sec1 = (blk1 / 4);
    else
        sec1 = (blk1 - 128) / 16 + 32;

    /* 读开头的块 */
    if (real->dc_access(dcrf, 0, sec1, real->m_key[sec1]) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
        return (FALSE);
    }
    if (real->dc_read(dcrf, blk1, temp) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_READ;
        return (FALSE);
    }

    /* 在同一块中 */
    if (blk1 == blk2) {
        off2 -= off1;
        mem_cpy(ptr, &temp[off1], off2);
        real->devb.error = CR_DEVB_ERR_OK;
        return (TRUE);
    }
    mem_cpy(ptr, &temp[off1], 16 - off1);
    ptr += 16 - off1;

    /* 读大块数据 */
    for (idx = blk1 + 1; idx <= blk2 - 1; idx++)
    {
        /* 跳过密码块 */
        if (idx < 128) {
            if ((idx + 1) % 4 == 0)
                continue;
            sec2 = (idx / 4);
        }
        else {
            if ((idx + 1) % 16 == 0)
                continue;
            sec2 = (idx - 128) / 16 + 32;
        }

        /* 认证块密码 */
        if (sec1 != sec2) {
            if (real->dc_access(dcrf, 0, sec2, real->m_key[sec2]) != 0) {
                real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
                return (FALSE);
            }
            sec1 = sec2;
        }
        if (real->dc_read(dcrf, idx, ptr) != 0) {
            real->devb.error = CR_DEVB_ERR_ICC_DCRF_READ;
            return (FALSE);
        }
        ptr += 16;
    }

    /* 读结束的块 */
    if (off2 != 0)
    {
        /* 处理大扇区 */
        if (blk2 < 128)
            sec2 = (blk2 / 4);
        else
            sec2 = (blk2 - 128) / 16 + 32;

        /* 认证块密码 */
        if (sec1 != sec2) {
            if (real->dc_access(dcrf, 0, sec2, real->m_key[sec2]) != 0) {
                real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
                return (FALSE);
            }
        }
        if (real->dc_read(dcrf, blk2, temp) != 0) {
            real->devb.error = CR_DEVB_ERR_ICC_DCRF_READ;
            return (FALSE);
        }
        mem_cpy(ptr, temp, off2);
    }
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*
---------------------------------------
    DCRF 写入 M1 IC 卡数据
---------------------------------------
*/
static bool_t
icc_m1_blk_write (
  __CR_IO__ sICC_DCRF*      real,
  __CR_IN__ void_t*         dcrf,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          addr,
  __CR_IN__ leng_t          size
    )
{
    int32u  snr;
    byte_t  temp[16];
    byte_t  idx, *ptr;
    byte_t  blk1, blk2;
    byte_t  sec1, sec2;
    uint_t  off1, off2;

    /* 必须先寻卡 */
    if (real->dc_card(dcrf, 1, &snr) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_FIND;
        return (FALSE);
    }
    blk1 = icc_m1_block(addr + 0x00, &off1);
    blk2 = icc_m1_block(addr + size, &off2);

    /* 处理大扇区 */
    ptr = (byte_t*)data;
    if (blk1 < 128)
        sec1 = (blk1 / 4);
    else
        sec1 = (blk1 - 128) / 16 + 32;

    /* 读开头的块 */
    if (real->dc_access(dcrf, 0, sec1, real->m_key[sec1]) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
        return (FALSE);
    }
    if (real->dc_read(dcrf, blk1, temp) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_READ;
        return (FALSE);
    }

    /* 在同一块中 */
    if (blk1 == blk2) {
        off2 -= off1;
        mem_cpy(&temp[off1], ptr, off2);
        if (real->dc_write(dcrf, blk1, temp) != 0) {
            real->devb.error = CR_DEVB_ERR_ICC_DCRF_WRITE;
            return (FALSE);
        }
        real->devb.error = CR_DEVB_ERR_OK;
        return (TRUE);
    }
    mem_cpy(&temp[off1], ptr, 16 - off1);
    if (real->dc_write(dcrf, blk1, temp) != 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_WRITE;
        return (FALSE);
    }
    ptr += 16 - off1;

    /* 读大块数据 */
    for (idx = blk1 + 1; idx <= blk2 - 1; idx++)
    {
        /* 跳过密码块 */
        if (idx < 128) {
            if ((idx + 1) % 4 == 0)
                continue;
            sec2 = (idx / 4);
        }
        else {
            if ((idx + 1) % 16 == 0)
                continue;
            sec2 = (idx - 128) / 16 + 32;
        }

        /* 认证块密码 */
        if (sec1 != sec2) {
            if (real->dc_access(dcrf, 0, sec2, real->m_key[sec2]) != 0) {
                real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
                return (FALSE);
            }
            sec1 = sec2;
        }
        if (real->dc_write(dcrf, idx, ptr) != 0) {
            real->devb.error = CR_DEVB_ERR_ICC_DCRF_WRITE;
            return (FALSE);
        }
        ptr += 16;
    }

    /* 读结束的块 */
    if (off2 != 0)
    {
        /* 处理大扇区 */
        if (blk2 < 128)
            sec2 = (blk2 / 4);
        else
            sec2 = (blk2 - 128) / 16 + 32;

        /* 认证块密码 */
        if (sec1 != sec2) {
            if (real->dc_access(dcrf, 0, sec2, real->m_key[sec2]) != 0) {
                real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
                return (FALSE);
            }
        }
        if (real->dc_read(dcrf, blk2, temp) != 0) {
            real->devb.error = CR_DEVB_ERR_ICC_DCRF_READ;
            return (FALSE);
        }
        mem_cpy(temp, ptr, off2);
        if (real->dc_write(dcrf, blk2, temp) != 0) {
            real->devb.error = CR_DEVB_ERR_ICC_DCRF_WRITE;
            return (FALSE);
        }
    }
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*
---------------------------------------
    DCRF 设置 IC 卡扇区密钥
---------------------------------------
*/
static bool_t
icc_m1_blk_skey (
  __CR_IO__ sICC_DCRF*              real,
  __CR_IN__ const sICC_DCRF_M1_KEY* skey
    )
{
    int32u  snr;
    void_t* dcrf;

    /* 打开设备 */
    dcrf = real->dc_init((int16s)real->m_port, real->m_baud);
    if ((ptrdiff_t)dcrf < 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_OPEN;
        return (FALSE);
    }

    /* 必须先寻卡 */
    if (real->dc_card(dcrf, 1, &snr) != 0) {
        real->dc_exit(dcrf);
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_FIND;
        return (FALSE);
    }

    /* 认证块密码 */
    if (real->dc_access(dcrf, 0, (byte_t)skey->sec_id,
                    real->m_key[skey->sec_id]) != 0) {
        real->dc_exit(dcrf);
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_PASS;
        return (FALSE);
    }

    /* 修改块密码 */
    if (real->dc_change(dcrf, (byte_t)skey->sec_id,
                skey->akey, skey->b0, skey->b1, skey->b2,
                skey->b3, skey->bk, skey->bkey) != 0) {
        real->dc_exit(dcrf);
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_SKEY;
        return (FALSE);
    }
    real->dc_exit(dcrf);
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*****************************************************************************/
/*                                 接口部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    关闭 DCRF S50/S70 IC 卡设备
---------------------------------------
*/
static void_t
icc_m1_close (
  __CR_IN__ devb_t  devb
    )
{
    sICC_DCRF*  real;

    real = (sICC_DCRF*)devb;
    sbin_unload(real->m_sbin);
    mem_free((void_t*)devb);
}

/*
---------------------------------------
    读取 DCRF S50/S70 IC 卡设备
---------------------------------------
*/
static leng_t
icc_m1_read (
  __CR_IN__ devb_t  devb,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  addr,
  __CR_IN__ leng_t  size
    )
{
    bool_t      rett;
    void_t*     dcrf;
    sICC_DCRF*  real;

    real = (sICC_DCRF*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    dcrf = real->dc_init((int16s)real->m_port, real->m_baud);
    if ((ptrdiff_t)dcrf < 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_OPEN;
        return ((leng_t)-1);
    }
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    rett = icc_m1_blk_read(real, dcrf, data, addr, size);
    real->dc_exit(dcrf);
    return (rett ? size : ((leng_t)-1));
}

/*
---------------------------------------
    写入 DCRF S50/S70 IC 卡设备
---------------------------------------
*/
static leng_t
icc_m1_write (
  __CR_IN__ devb_t          devb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          addr,
  __CR_IN__ leng_t          size
    )
{
    bool_t      rett;
    void_t*     dcrf;
    sICC_DCRF*  real;

    real = (sICC_DCRF*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    dcrf = real->dc_init((int16s)real->m_port, real->m_baud);
    if ((ptrdiff_t)dcrf < 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_OPEN;
        return ((leng_t)-1);
    }
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    rett = icc_m1_blk_write(real, dcrf, data, addr, size);
    real->dc_exit(dcrf);
    return (rett ? size : ((leng_t)-1));
}

/*
---------------------------------------
    控制 DCRF S50/S70 IC 卡设备
---------------------------------------
*/
static bool_t
icc_m1_ioctl (
  __CR_IN__ devb_t          devb,
  __CR_IN__ uint_t          cmd_code,
  __CR_OT__ void_t*         obuff,
  __CR_IN__ leng_t          osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ leng_t          isize,
  __CR_OT__ leng_t*         ret_size
    )
{
    uint_t              idx;
    void_t*             dcrf;
    sICC_DCRF*          real;
    sDEV_BINFO          info;
    sICC_DCRF_M1_KEY    skey;

    if (ret_size != NULL)
        *ret_size = 0x00;
    real = (sICC_DCRF*)devb;
    switch (cmd_code)
    {
        /* 设置端口号 */
        case ICC_DCRF_SET_PORT:
            if (isize != sizeof(int32u)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            mem_cpy(&real->m_port, ibuff, sizeof(int32u));
            if (real->m_port > 0 && real->m_port < 100)
                real->m_port -= 1;
            break;

        /* 设置波特率 */
        case ICC_DCRF_SET_BAUD:
            if (isize != sizeof(int32u)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            mem_cpy(&real->m_baud, ibuff, sizeof(int32u));
            break;

        /* 自动设置端口和速度 */
        case ICC_AUTO_SET_PORT:
            idx = 100;
            dcrf = real->dc_init((int16s)idx, 0UL);
            if ((ptrdiff_t)dcrf < 0) {
                for (idx = 0; idx < _CR_SIO_COUNT_; idx++) {
                    dcrf = real->dc_init((int16s)idx, 115200UL);
                    if ((ptrdiff_t)dcrf > 0)
                        break;
                }
                if (idx >= _CR_SIO_COUNT_) {
                    real->devb.error = CR_DEVB_ERR_ICC_DCRF_OPEN;
                    return (FALSE);
                }
            }
            real->dc_exit(dcrf);
            real->m_port = idx;
            real->m_baud = 115200UL;
            break;

        /* 设置 M1 卡访问密钥 */
        /* 设置 M1 卡扇区密钥 */
        case ICC_DCRF_SET_RKEY:
        case ICC_DCRF_SET_SKEY:
            if (isize != sizeof(sICC_DCRF_M1_KEY)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            struct_cpy(&skey, ibuff, sICC_DCRF_M1_KEY);
            if (real->m_size == 752) {
                if (skey.sec_id >= 16) {
                    real->devb.error = CR_DEVB_ERR_ICC_DCRF_NSEC;
                    return (FALSE);
                }
            }
            else {
                if (skey.sec_id >= 40) {
                    real->devb.error = CR_DEVB_ERR_ICC_DCRF_NSEC;
                    return (FALSE);
                }
            }
            if (cmd_code == ICC_DCRF_SET_SKEY) {
                if (!icc_m1_blk_skey(real, &skey))
                    return (FALSE);
            }
            for (idx = 0; idx < 6; idx++)
                real->m_key[skey.sec_id][idx] = skey.akey[idx];
            break;

        /* 获取块设备信息 */
        case DEVB_GET_INFOR:
            if (ret_size == NULL) {
                real->devb.error = CR_DEVB_ERR_CONTROL_RETTS;
                return (FALSE);
            }
            if (osize < sizeof(sDEV_BINFO)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_OSIZE;
                return (FALSE);
            }
            info.page_size = 16;
            info.devb_size = real->m_size;
            info.page_cnts = real->m_size / 16;
            if (real->m_size == 752)
                info.devb_name = "DCRF S50 ICCARD";
            else
                info.devb_name = "DCRF S70 ICCARD";
            struct_cpy(obuff, &info, sDEV_BINFO);
            *ret_size = sizeof(sDEV_BINFO);
            break;

        /* 不支持的控制码 */
        default:
            real->devb.error = CR_DEVB_ERR_CONTROL_CODE;
            return (FALSE);
    }
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*
---------------------------------------
    打开 DCRF S50/S70 IC 卡设备
---------------------------------------
*/
static devb_t
icc_m1_open (
  __CR_IN__ leng_t  size
    )
{
    uint_t      xx;
    uint_t      yy;
    sICC_DCRF*  rett;

    rett = struct_new(sICC_DCRF);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sICC_DCRF);
    rett->m_sbin = sbin_loadA("dcrf32.dll");
    if (rett->m_sbin == NULL)
        goto _failure1;
    rett->dc_init   = sbin_exportT(rett->m_sbin, "dc_init", dc_init_t);
    rett->dc_exit   = sbin_exportT(rett->m_sbin, "dc_exit", dc_exit_t);
    rett->dc_access = sbin_exportT(rett->m_sbin, "dc_authentication_pass",
                                   dc_access_t);
    if (rett->dc_init == NULL)
        rett->dc_init = sbin_exportT(rett->m_sbin, "dc_init_w", dc_init_t);
    /* ----------------------------------------------------------------- */
    rett->dc_card   = sbin_exportT(rett->m_sbin, "dc_card", dc_card_t);
    rett->dc_read   = sbin_exportT(rett->m_sbin, "dc_read", dc_read_t);
    rett->dc_write  = sbin_exportT(rett->m_sbin, "dc_write", dc_write_t);
    rett->dc_change = sbin_exportT(rett->m_sbin, "dc_changeb3", dc_change_t);
    if (rett->dc_init   == NULL || rett->dc_read   == NULL ||
        rett->dc_exit   == NULL || rett->dc_card   == NULL ||
        rett->dc_write  == NULL || rett->dc_change == NULL ||
        rett->dc_access == NULL)
        goto _failure2;
    rett->m_size = size;
    rett->m_port =  100;
    rett->m_baud = 9600;
    rett->devb.read  = icc_m1_read;
    rett->devb.write = icc_m1_write;
    rett->devb.ioctl = icc_m1_ioctl;
    rett->devb.close = icc_m1_close;
    for (xx = 0; xx < 40; xx++)
    for (yy = 0; yy <  6; yy++)
        rett->m_key[xx][yy] = 0xFF;     /* 默认密钥为全一 */
    return ((devb_t)rett);

_failure2:
    sbin_unload(rett->m_sbin);
_failure1:
    mem_free(rett);
    return (NULL);
}

/*****************************************************************************/
/*                               S50/S70 ICC                                 */
/*****************************************************************************/

/*
---------------------------------------
    打开 DCRF S50 IC 卡设备
---------------------------------------
*/
static devb_t
m1_s50_open (
  __CR_IN__ const ansi_t*   name
    )
{
    /* (16 * 3 - 1) * 16 = 752 */
    CR_NOUSE(name);
    return (icc_m1_open(752));
}

/*
---------------------------------------
    打开 DCRF S70 IC 卡设备
---------------------------------------
*/
static devb_t
m1_s70_open (
  __CR_IN__ const ansi_t*   name
    )
{
    /* (32 * 3 - 1) * 16 +
        8 * 15 * 16 = 3440 */
    CR_NOUSE(name);
    return (icc_m1_open(3440));
}

/*****************************************************************************/
/*                                驱动注册表                                 */
/*****************************************************************************/

/* S50 */
const sDEV_BNODE _rom_ g_icc_dcrf_s50 =
{
    "/iccard/dcrf/s50", m1_s50_open,
};

/* S70 */
const sDEV_BNODE _rom_ g_icc_dcrf_s70 =
{
    "/iccard/dcrf/s70", m1_s70_open,
};

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
