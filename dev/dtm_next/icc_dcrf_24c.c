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
/*  >>>>>>>>>>>>>> DTM_NEXT DCRF 24CXX ICCARD 设备驱动函数库 <<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "iccard.h"
#include "memlib.h"
#include "plugin.h"

/* DCRF 24CXX ICCARD 接口函数 */
typedef void_t* (STDCALL *dc_init_t) (int16s, int32u);
typedef int16s  (STDCALL *dc_exit_t) (void_t*);
typedef int16s  (STDCALL *dc_read_24c_t) (void_t*, int16s, int16s, void_t*);
typedef int16s  (STDCALL *dc_write_24c_t) (void_t*, int16s, int16s,
                                           const void_t*);
/* DCRF 24CXX ICCARD 设备内部结构 */
typedef struct
{
        /* 通用部分 */
        sDEV_BLOCK  devb;

        /* 个性部分 */
        sbin_t  m_sbin;     /* 模块句柄 */
        leng_t  m_size;     /* 文件大小 */
        int32u  m_port;     /* 端口编号 */
        int32u  m_baud;     /* 通讯速度 */

        /* 模块接口函数 */
        dc_init_t       dc_init;        /* 打开设备 */
        dc_exit_t       dc_exit;        /* 关闭设备 */
        dc_read_24c_t   dc_read_24c;    /* 读取数据 */
        dc_write_24c_t  dc_write_24c;   /* 写入数据 */

} sICC_DCRF;

/*****************************************************************************/
/*                                 接口部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    关闭 DCRF 24CXX IC 卡设备
---------------------------------------
*/
static void_t
icc_24cxx_close (
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
    读取 DCRF 24CXX IC 卡设备
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
    if (real->dc_read_24c(dcrf, (int16s)addr, (int16s)size, data) != 0) {
        real->dc_exit(dcrf);
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_READ;
        return ((leng_t)-1);
    }
    real->dc_exit(dcrf);
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    写入 DCRF 24CXX IC 卡设备
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
    if (real->dc_write_24c(dcrf, (int16s)addr, (int16s)size, data) != 0) {
        real->dc_exit(dcrf);
        real->devb.error = CR_DEVB_ERR_ICC_DCRF_WRITE;
        return ((leng_t)-1);
    }
    real->dc_exit(dcrf);
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    控制 DCRF 24CXX IC 卡设备
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
    void_t*     dcrf;
    sICC_DCRF*  real;
    sDEV_BINFO  info;

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
                    info.devb_name = "DCRF 24C01 ICCARD";
                    break;

                case 256:
                    info.page_size = 8;
                    info.devb_name = "DCRF 24C02 ICCARD";
                    break;

                case 512:
                    info.page_size = 16;
                    info.devb_name = "DCRF 24C04 ICCARD";
                    break;

                case 1024:
                    info.page_size = 16;
                    info.devb_name = "DCRF 24C08 ICCARD";
                    break;

                case 2048:
                    info.page_size = 16;
                    info.devb_name = "DCRF 24C16 ICCARD";
                    break;

                case 4096:
                    info.page_size = 32;
                    info.devb_name = "DCRF 24C32 ICCARD";
                    break;

                case 8192:
                    info.page_size = 32;
                    info.devb_name = "DCRF 24C64 ICCARD";
                    break;

                case 16384:
                    info.page_size = 64;
                    info.devb_name = "DCRF 24C128 ICCARD";
                    break;

                default:
             /* case 32768: */
                    info.page_size = 64;
                    info.devb_name = "DCRF 24C256 ICCARD";
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
    打开 DCRF 24CXX IC 卡设备
---------------------------------------
*/
static devb_t
icc_24cxx_open (
  __CR_IN__ leng_t  size
    )
{
    sICC_DCRF*  rett;

    rett = struct_new(sICC_DCRF);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sICC_DCRF);
    rett->m_sbin = sbin_loadA("dcrf32.dll");
    if (rett->m_sbin == NULL)
        goto _failure1;
    rett->dc_init = sbin_exportT(rett->m_sbin, "dc_init", dc_init_t);
    rett->dc_exit = sbin_exportT(rett->m_sbin, "dc_exit", dc_exit_t);
    if (rett->dc_init == NULL)
        rett->dc_init = sbin_exportT(rett->m_sbin, "dc_init_w", dc_init_t);
    /* ----------------------------------------------------------------- */
    if (size <= 2048) {     /* 24C16 */
        rett->dc_read_24c = sbin_exportT(rett->m_sbin, "dc_read_24c",
                                         dc_read_24c_t);
        rett->dc_write_24c = sbin_exportT(rett->m_sbin, "dc_write_24c",
                                          dc_write_24c_t);
    }
    else {                  /* 24C64 */
        rett->dc_read_24c = sbin_exportT(rett->m_sbin, "dc_read_24c64",
                                         dc_read_24c_t);
        rett->dc_write_24c = sbin_exportT(rett->m_sbin, "dc_write_24c64",
                                          dc_write_24c_t);
    }
    if (rett->dc_init == NULL || rett->dc_read_24c  == NULL ||
        rett->dc_exit == NULL || rett->dc_write_24c == NULL)
        goto _failure2;
    rett->m_size = size;
    rett->m_port =  100;
    rett->m_baud = 9600;
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
    打开 DCRF 24C64 IC 卡设备
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
    打开 DCRF 24C128 IC 卡设备
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
const sDEV_BNODE _rom_ g_icc_dcrf_24c64 =
{
    "/iccard/dcrf/24c64", e24c64_open,
};

/* 24C128 */
const sDEV_BNODE _rom_ g_icc_dcrf_24c128 =
{
    "/iccard/dcrf/24c128", e24c128_open,
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
