/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-12-15  */
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
/*  >>>>>>>>>>>>>>>>> DTM_NEXT UNIV ICCARD 设备驱动函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "iccard.h"
#include "memlib.h"
#include "morder.h"
#include "machine.h"
#include "util/commlib.h"

/* UNIV ICCARD 设备内部结构 */
typedef struct
{
        /* 通用部分 */
        sDEV_BLOCK  devb;

        /* 个性部分 */
        bool_t  m_rfic;     /* 是否非接 */
        int32u  m_port;     /* 端口编号 */
        leng_t  m_size;     /* 文件大小 */

} sICC_UNIV;

/*****************************************************************************/
/*                                 接口部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    关闭 UNIV IC 卡设备
---------------------------------------
*/
static void_t
icc_univ_close (
  __CR_IN__ devb_t  devb
    )
{
    sICC_UNIV*  real;

    real = (sICC_UNIV*)devb;
    sio_close((uint_t)real->m_port);
    mem_free((void_t*)devb);
}

/*
---------------------------------------
    读取 UNIV IC 卡设备
---------------------------------------
*/
static leng_t
icc_univ_read (
  __CR_IN__ devb_t  devb,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  addr,
  __CR_IN__ leng_t  size
    )
{
    uint_t      port;
    uint_t      outp;
    int16u      inpt;
    int16u      rett;
    leng_t      blks;
    byte_t*     pntr;
    void_t*     temp;
    sICC_UNIV*  real;

    real = (sICC_UNIV*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    temp = mem_malloc(size + sizeof(int16u));
    if (temp == NULL) {
        real->devb.error = CR_DEVB_ERR_ICC_UNIV_FLMEM;
        return ((leng_t)-1);
    }
    port = (uint_t)real->m_port;
    if (!rs232_init(port, 115200UL, 2000)) {
        mem_free(temp);
        real->devb.error = CR_DEVB_ERR_ICC_UNIV_OPEN;
        return ((leng_t)-1);
    }
#if 0
    inpt = CWORD_LE(0x8000);
    outp = sizeof(int16u) + sizeof(inpt);
    if (!dtmn_sio_iorw(port, NCMD_MISCX, NCMD_MISCX_SETMODES,
            temp, &outp, &rett, 0, 0, &inpt, sizeof(inpt), NULL) ||
                   outp != sizeof(inpt) || rett != 0x0000) {
        mem_free(temp); sio_close(port);
        real->devb.error = CR_DEVB_ERR_ICC_UNIV_STYPE;
        return ((leng_t)-1);
    }
    outp = sizeof(int16u);
    if (real->m_rfic) {     /* 非接式卡类型 */
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_CARD_RFC,
                temp, &outp, &rett, 0, 0, NULL, 0, NULL) ||
                       outp != 0 || rett != 0x0000) {
            mem_free(temp); sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_STYPE;
            return ((leng_t)-1);
        }
    }
    else {                  /* 接触式卡类型 */
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_CARD_24C,
                temp, &outp, &rett, 0, 0, NULL, 0, NULL) ||
                       outp != 0 || rett != 0x0000) {
            mem_free(temp); sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_STYPE;
            return ((leng_t)-1);
        }
    }
#endif
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    for (pntr = (byte_t*)temp, blks = size / 1024; blks != 0; blks--) {
        inpt = CWORD_LE((int16u)1024);
        outp = 1024 + sizeof(int16u);
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_RD, pntr, &outp,
                &rett, 0, (int16u)addr, &inpt, sizeof(inpt), NULL) ||
                        outp != 1024 || rett != 0x0000) {
            mem_free(temp); sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_READ;
            return ((leng_t)-1);
        }
        addr += 1024;
        pntr += 1024;
    }
    blks = size % 1024;
    if (blks != 0) {
        inpt = WORD_LE((int16u)blks);
        outp = (uint_t)blks + sizeof(int16u);
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_RD, pntr, &outp,
                &rett, 0, (int16u)addr, &inpt, sizeof(inpt), NULL) ||
                        outp != blks || rett != 0x0000) {
            mem_free(temp); sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_READ;
            return ((leng_t)-1);
        }
    }
    mem_cpy(data, temp, size);
    mem_free(temp); sio_close(port);
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    写入 UNIV IC 卡设备
---------------------------------------
*/
static leng_t
icc_univ_write (
  __CR_IN__ devb_t          devb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          addr,
  __CR_IN__ leng_t          size
    )
{
    uint_t      port;
    uint_t      outp;
    int16u      inpt;
    int16u      rett;
    int32u      temp;
    leng_t      blks;
    byte_t*     pntr;
    sICC_UNIV*  real;

    real = (sICC_UNIV*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    port = (uint_t)real->m_port;
    if (!rs232_init(port, 115200UL, 2000)) {
        real->devb.error = CR_DEVB_ERR_ICC_UNIV_OPEN;
        return ((leng_t)-1);
    }
#if 0
    outp = sizeof(int32u);
    inpt = CWORD_LE(0x8000);
    if (!dtmn_sio_iorw(port, NCMD_MISCX, NCMD_MISCX_SETMODES,
            &temp, &outp, &rett, 0, 0, &inpt, sizeof(inpt), NULL) ||
                    outp != sizeof(int16u) || rett != 0x0000) {
        sio_close(port);
        real->devb.error = CR_DEVB_ERR_ICC_UNIV_STYPE;
        return ((leng_t)-1);
    }
    outp = sizeof(int16u);
    if (real->m_rfic) {     /* 非接式卡类型 */
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_CARD_RFC,
                &temp, &outp, &rett, 0, 0, NULL, 0, NULL) ||
                        outp != 0 || rett != 0x0000) {
            sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_STYPE;
            return ((leng_t)-1);
        }
    }
    else {                  /* 接触式卡类型 */
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_CARD_24C,
                &temp, &outp, &rett, 0, 0, NULL, 0, NULL) ||
                        outp != 0 || rett != 0x0000) {
            sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_STYPE;
            return ((leng_t)-1);
        }
    }
#endif
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    for (pntr = (byte_t*)data, blks = size / 1024; blks != 0; blks--) {
        inpt = (int16u)1024;
        outp = sizeof(int16u);
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_WR, &temp, &outp,
                &rett, 0, (int16u)addr, pntr, inpt, NULL) ||
                        outp != 0 || rett != 0x0000) {
            sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_WRITE;
            return ((leng_t)-1);
        }
        addr += 1024;
        pntr += 1024;
    }
    blks = size % 1024;
    if (blks != 0) {
        inpt = (int16u)blks;
        outp = sizeof(int16u);
        if (!dtmn_sio_iorw(port, NCMD_RAWIC, NCMD_RAWIC_WR, &temp, &outp,
                &rett, 0, (int16u)addr, pntr, inpt, NULL) ||
                        outp != 0 || rett != 0x0000) {
            sio_close(port);
            real->devb.error = CR_DEVB_ERR_ICC_UNIV_WRITE;
            return ((leng_t)-1);
        }
    }
    sio_close(port);
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    控制 UNIV IC 卡设备
---------------------------------------
*/
static bool_t
icc_univ_ioctl (
  __CR_IN__ devb_t          devb,
  __CR_IN__ uint_t          cmd_code,
  __CR_OT__ void_t*         obuff,
  __CR_IN__ leng_t          osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ leng_t          isize,
  __CR_OT__ leng_t*         ret_size
    )
{
    uint_t      port;
    int32u      baud;
    sICC_UNIV*  real;
    sDEV_BINFO  info;

    if (ret_size != NULL)
        *ret_size = 0x00;
    real = (sICC_UNIV*)devb;
    switch (cmd_code)
    {
        /* 设置端口号 */
        case ICC_UNIV_SET_PORT:
            if (isize != sizeof(int32u)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            mem_cpy(&real->m_port, ibuff, sizeof(int32u));
            break;

        /* 自动设置端口和速度 */
        case ICC_AUTO_SET_PORT:
            port = dtm_new_search(_CR_SIO_COUNT_, 100, &baud);
            if (port == 0) {
                real->devb.error = CR_DEVB_ERR_ICC_UNIV_OPEN;
                return (FALSE);
            }
            real->m_port = port;
            sio_close(port);
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
            info.page_cnts = 1;
            info.page_size = real->m_size;
            info.devb_size = real->m_size;
            if (real->m_rfic)
                info.devb_name = "UNIV RFC ICCARD";
            else
                info.devb_name = "UNIV 24C ICCARD";
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
    打开 UNIV IC 卡设备
---------------------------------------
*/
static devb_t
icc_univ_open (
  __CR_IN__ leng_t  size,
  __CR_IN__ bool_t  rfic
    )
{
    sICC_UNIV*  rett;

    rett = struct_new(sICC_UNIV);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sICC_UNIV);
    rett->m_port = 0x01;
    rett->m_size = size;
    rett->m_rfic = rfic;
    rett->devb.read  = icc_univ_read;
    rett->devb.write = icc_univ_write;
    rett->devb.ioctl = icc_univ_ioctl;
    rett->devb.close = icc_univ_close;
    return ((devb_t)rett);
}

/*****************************************************************************/
/*                               S50/S70 ICC                                 */
/*****************************************************************************/

/*
---------------------------------------
    打开 UNIV S50 IC 卡设备
---------------------------------------
*/
static devb_t
m1_s50_open (
  __CR_IN__ const ansi_t*   name
    )
{
    /* (16 * 3 - 1) * 16 = 752 */
    CR_NOUSE(name);
    return (icc_univ_open(752, TRUE));
}

/*
---------------------------------------
    打开 UNIV S70 IC 卡设备
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
    return (icc_univ_open(3440, TRUE));
}

/*****************************************************************************/
/*                                24CXX ICC                                  */
/*****************************************************************************/

/*
---------------------------------------
    打开 UNIV 24C64 IC 卡设备
---------------------------------------
*/
static devb_t
e24c64_open (
  __CR_IN__ const ansi_t*   name
    )
{
    CR_NOUSE(name);
    return (icc_univ_open(CR_K2B(8), FALSE));
}

/*
---------------------------------------
    打开 UNIV 24C128 IC 卡设备
---------------------------------------
*/
static devb_t
e24c128_open (
  __CR_IN__ const ansi_t*   name
    )
{
    CR_NOUSE(name);
    return (icc_univ_open(CR_K2B(16), FALSE));
}

/*****************************************************************************/
/*                                驱动注册表                                 */
/*****************************************************************************/

/* S50 */
const sDEV_BNODE _rom_ g_icc_univ_s50 =
{
    "/iccard/univ/s50", m1_s50_open,
};

/* S70 */
const sDEV_BNODE _rom_ g_icc_univ_s70 =
{
    "/iccard/univ/s70", m1_s70_open,
};

/* 24C64 */
const sDEV_BNODE _rom_ g_icc_univ_24c64 =
{
    "/iccard/univ/24c64", e24c64_open,
};

/* 24C128 */
const sDEV_BNODE _rom_ g_icc_univ_24c128 =
{
    "/iccard/univ/24c128", e24c128_open,
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
