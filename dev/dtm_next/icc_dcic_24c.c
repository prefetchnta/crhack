/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-20  */
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
/*  >>>>>>>>>>>>>> DTM_NEXT DCIC 24CXX ICCARD 设备驱动函数库 <<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "iccard.h"
#include "memlib.h"
#include "plugin.h"

/* DCIC 24CXX ICCARD 接口函数 */
typedef void_t* (STDCALL *IC_InitComm_t) (int16s);
typedef int16s  (STDCALL *IC_ExitComm_t) (void_t*);
typedef int16s  (STDCALL *IC_InitType_t) (void_t*, int16s);
typedef int16s  (STDCALL *IC_Status_t) (void_t*);
typedef int16s  (STDCALL *IC_Read24_t) (void_t*, int16s, int16s, void_t*);
typedef int16s  (STDCALL *IC_Write24_t) (void_t*, int16s, int16s,
                                         const void_t*);
/* DCIC 24CXX ICCARD 设备内部结构 */
typedef struct
{
        /* 通用部分 */
        sDEV_BLOCK  devb;

        /* 个性部分 */
        sbin_t  m_sbin;     /* 模块句柄 */
        leng_t  m_size;     /* 文件大小 */
        int32u  m_port;     /* 端口编号 */

        /* 模块接口函数 */
        IC_Status_t     IC_Status;      /* 检查状态 */
        IC_Read24_t     IC_Read24;      /* 读取数据 */
        IC_Write24_t    IC_Write24;     /* 写入数据 */
        IC_InitComm_t   IC_InitComm;    /* 打开设备 */
        IC_ExitComm_t   IC_ExitComm;    /* 关闭设备 */
        IC_InitType_t   IC_InitType;    /* 设置卡型 */

} sICC_DCIC;

/*****************************************************************************/
/*                                 接口部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    关闭 DCIC 24CXX IC 卡设备
---------------------------------------
*/
static void_t
icc_24cxx_close (
  __CR_IN__ devb_t  devb
    )
{
    sICC_DCIC*  real;

    real = (sICC_DCIC*)devb;
    sbin_unload(real->m_sbin);
    mem_free((void_t*)devb);
}

/*
---------------------------------------
    读取 DCIC 24CXX IC 卡设备
---------------------------------------
*/
static leng_t
icc_24cxx_read (
  __CR_IN__ devb_t  devb,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  addr,
  __CR_IN__ leng_t  size
    )
{
    void_t*     dcic;
    sICC_DCIC*  real;

    real = (sICC_DCIC*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    dcic = real->IC_InitComm((int16s)real->m_port);
    if ((ptrdiff_t)dcic < 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCIC_OPEN;
        return ((leng_t)-1);
    }
    if (real->IC_Status(dcic) != 0) {
        real->IC_ExitComm(dcic);
        real->devb.error = CR_DEVB_ERR_ICC_DCIC_FIND;
        return ((leng_t)-1);
    }
    if (real->m_size <= 2048) {     /* 24C16 */
        if (real->IC_InitType(dcic, 0x40) < 0) {
            real->IC_ExitComm(dcic);
            real->devb.error = CR_DEVB_ERR_ICC_DCIC_STYPE;
            return ((leng_t)-1);
        }
    }
    else {                          /* 24C64 */
        if (real->IC_InitType(dcic, 0xC0) < 0) {
            real->IC_ExitComm(dcic);
            real->devb.error = CR_DEVB_ERR_ICC_DCIC_STYPE;
            return ((leng_t)-1);
        }
    }
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    if (real->IC_Read24(dcic, (int16s)addr, (int16s)size, data) != 0) {
        real->IC_ExitComm(dcic);
        real->devb.error = CR_DEVB_ERR_ICC_DCIC_READ;
        return ((leng_t)-1);
    }
    real->IC_ExitComm(dcic);
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    写入 DCIC 24CXX IC 卡设备
---------------------------------------
*/
static leng_t
icc_24cxx_write (
  __CR_IN__ devb_t          devb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          addr,
  __CR_IN__ leng_t          size
    )
{
    void_t*     dcic;
    sICC_DCIC*  real;

    real = (sICC_DCIC*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    dcic = real->IC_InitComm((int16s)real->m_port);
    if ((ptrdiff_t)dcic < 0) {
        real->devb.error = CR_DEVB_ERR_ICC_DCIC_OPEN;
        return ((leng_t)-1);
    }
    if (real->IC_Status(dcic) != 0) {
        real->IC_ExitComm(dcic);
        real->devb.error = CR_DEVB_ERR_ICC_DCIC_FIND;
        return ((leng_t)-1);
    }
    if (real->m_size <= 2048) {     /* 24C16 */
        if (real->IC_InitType(dcic, 0x40) < 0) {
            real->IC_ExitComm(dcic);
            real->devb.error = CR_DEVB_ERR_ICC_DCIC_STYPE;
            return ((leng_t)-1);
        }
    }
    else {                          /* 24C64 */
        if (real->IC_InitType(dcic, 0xC0) < 0) {
            real->IC_ExitComm(dcic);
            real->devb.error = CR_DEVB_ERR_ICC_DCIC_STYPE;
            return ((leng_t)-1);
        }
    }
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    if (real->IC_Write24(dcic, (int16s)addr, (int16s)size, data) != 0) {
        real->IC_ExitComm(dcic);
        real->devb.error = CR_DEVB_ERR_ICC_DCIC_WRITE;
        return ((leng_t)-1);
    }
    real->IC_ExitComm(dcic);
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    控制 DCIC 24CXX IC 卡设备
---------------------------------------
*/
static bool_t
icc_24cxx_ioctl (
  __CR_IN__ devb_t          devb,
  __CR_IN__ uint_t          cmd_code,
  __CR_OT__ void_t*         obuff,
  __CR_IN__ leng_t          osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ leng_t          isize,
  __CR_OT__ leng_t*         ret_size
    )
{
    uint_t      idx;
    void_t*     dcic;
    sICC_DCIC*  real;
    sDEV_BINFO  info;

    if (ret_size != NULL)
        *ret_size = 0x00;
    real = (sICC_DCIC*)devb;
    switch (cmd_code)
    {
        /* 设置端口号 */
        case ICC_DCIC_SET_PORT:
            if (isize != sizeof(int32u)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            mem_cpy(&real->m_port, ibuff, sizeof(int32u));
            if (real->m_port > 0 && real->m_port < 100)
                real->m_port -= 1;
            break;

        /* 自动设置端口和速度 */
        case ICC_AUTO_SET_PORT:
            idx = 100;
            dcic = real->IC_InitComm((int16s)idx);
            if ((ptrdiff_t)dcic < 0) {
                for (idx = 0; idx < _CR_SIO_COUNT_; idx++) {
                    dcic = real->IC_InitComm((int16s)idx);
                    if ((ptrdiff_t)dcic > 0)
                        break;
                }
                if (idx >= _CR_SIO_COUNT_) {
                    real->devb.error = CR_DEVB_ERR_ICC_DCIC_OPEN;
                    return (FALSE);
                }
            }
            real->m_port = idx;
            real->IC_ExitComm(dcic);
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
            switch (real->m_size)
            {
                case 128:
                    info.page_size = 8;
                    info.devb_name = "DCIC 24C01 ICCARD";
                    break;

                case 256:
                    info.page_size = 8;
                    info.devb_name = "DCIC 24C02 ICCARD";
                    break;

                case 512:
                    info.page_size = 16;
                    info.devb_name = "DCIC 24C04 ICCARD";
                    break;

                case 1024:
                    info.page_size = 16;
                    info.devb_name = "DCIC 24C08 ICCARD";
                    break;

                case 2048:
                    info.page_size = 16;
                    info.devb_name = "DCIC 24C16 ICCARD";
                    break;

                case 4096:
                    info.page_size = 32;
                    info.devb_name = "DCIC 24C32 ICCARD";
                    break;

                case 8192:
                    info.page_size = 32;
                    info.devb_name = "DCIC 24C64 ICCARD";
                    break;

                case 16384:
                    info.page_size = 64;
                    info.devb_name = "DCIC 24C128 ICCARD";
                    break;

                default:
             /* case 32768: */
                    info.page_size = 64;
                    info.devb_name = "DCIC 24C256 ICCARD";
                    break;
            }
            info.devb_size = real->m_size;
            info.page_cnts = real->m_size / info.page_size;
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
    打开 DCIC 24CXX IC 卡设备
---------------------------------------
*/
static devb_t
icc_24cxx_open (
  __CR_IN__ leng_t  size
    )
{
    sICC_DCIC*  rett;

    rett = struct_new(sICC_DCIC);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sICC_DCIC);
    rett->m_sbin = sbin_loadA("dcic32.dll");
    if (rett->m_sbin == NULL)
        goto _failure1;
    rett->IC_Read24 = sbin_exportT(rett->m_sbin, "IC_Read", IC_Read24_t);
    rett->IC_Status = sbin_exportT(rett->m_sbin, "IC_Status", IC_Status_t);
    /* ----------------------------------------------------------------- */
    rett->IC_ExitComm = sbin_exportT(rett->m_sbin, "IC_ExitComm",
                                     IC_ExitComm_t);
    rett->IC_InitType = sbin_exportT(rett->m_sbin, "IC_InitType",
                                     IC_InitType_t);
    rett->IC_InitComm = sbin_exportT(rett->m_sbin, "IC_InitCommAdvanced",
                                     IC_InitComm_t);
    if (size <= 2048) {     /* 24C16 */
        rett->IC_Write24 = sbin_exportT(rett->m_sbin, "IC_Write24",
                                        IC_Write24_t);
    }
    else {                  /* 24C64 */
        rett->IC_Write24 = sbin_exportT(rett->m_sbin, "IC_Write64",
                                        IC_Write24_t);
    }
    if (rett->IC_InitComm == NULL || rett->IC_Status  == NULL ||
        rett->IC_ExitComm == NULL || rett->IC_Read24  == NULL ||
        rett->IC_InitType == NULL || rett->IC_Write24 == NULL)
        goto _failure2;
    rett->m_size = size;
    rett->m_port =  100;
    rett->devb.read  = icc_24cxx_read;
    rett->devb.write = icc_24cxx_write;
    rett->devb.ioctl = icc_24cxx_ioctl;
    rett->devb.close = icc_24cxx_close;
    return ((devb_t)rett);

_failure2:
    sbin_unload(rett->m_sbin);
_failure1:
    mem_free(rett);
    return (NULL);
}

/*****************************************************************************/
/*                                24CXX ICC                                  */
/*****************************************************************************/

/*
---------------------------------------
    打开 DCIC 24C64 IC 卡设备
---------------------------------------
*/
static devb_t
e24c64_open (
  __CR_IN__ const ansi_t*   name
    )
{
    CR_NOUSE(name);
    return (icc_24cxx_open(CR_K2B(8)));
}

/*
---------------------------------------
    打开 DCIC 24C128 IC 卡设备
---------------------------------------
*/
static devb_t
e24c128_open (
  __CR_IN__ const ansi_t*   name
    )
{
    CR_NOUSE(name);
    return (icc_24cxx_open(CR_K2B(16)));
}

/*****************************************************************************/
/*                                驱动注册表                                 */
/*****************************************************************************/

/* 24C64 */
const sDEV_BNODE _rom_ g_icc_dcic_24c64 =
{
    "/iccard/dcic/24c64", e24c64_open,
};

/* 24C128 */
const sDEV_BNODE _rom_ g_icc_dcic_24c128 =
{
    "/iccard/dcic/24c128", e24c128_open,
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
