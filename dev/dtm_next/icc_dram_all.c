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
/*  >>>>>>>>>>>>>>>>> DTM_NEXT DRAM ICCARD 设备驱动函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "iccard.h"
#include "memlib.h"

/* DRAM ICCARD 设备内部结构 */
typedef struct
{
        /* 通用部分 */
        sDEV_BLOCK  devb;

        /* 个性部分 */
        leng_t  m_size;     /* 内存大小 */
        byte_t* m_data;     /* 内存缓存 */

} sICC_DRAM;

/*****************************************************************************/
/*                                 接口部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    关闭 DRAM IC 卡设备
---------------------------------------
*/
static void_t
icc_dram_close (
  __CR_IN__ devb_t  devb
    )
{
    sICC_DRAM*  real;

    real = (sICC_DRAM*)devb;
    mem_free((void_t*)real->m_data);
    mem_free((void_t*)devb);
}

/*
---------------------------------------
    读取 DRAM IC 卡设备
---------------------------------------
*/
static leng_t
icc_dram_read (
  __CR_IN__ devb_t  devb,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  addr,
  __CR_IN__ leng_t  size
    )
{
    sICC_DRAM*  real;

    real = (sICC_DRAM*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    real->devb.error = CR_DEVB_ERR_OK;
    mem_cpy(data, real->m_data + addr, size);
    return ((leng_t)size);
}

/*
---------------------------------------
    写入 DRAM IC 卡设备
---------------------------------------
*/
static leng_t
icc_dram_write (
  __CR_IN__ devb_t          devb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          addr,
  __CR_IN__ leng_t          size
    )
{
    sICC_DRAM*  real;

    real = (sICC_DRAM*)devb;
    if (size == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return (0);
    }
    if (addr >= real->m_size) {
        real->devb.error = CR_DEVB_ERR_ADDR_OVERFLOW;
        return ((leng_t)-1);
    }
    if (size > real->m_size - addr)
        size = real->m_size - addr;
    real->devb.error = CR_DEVB_ERR_OK;
    mem_cpy(real->m_data + addr, data, size);
    return ((leng_t)size);
}

/*
---------------------------------------
    控制 DRAM IC 卡设备
---------------------------------------
*/
static bool_t
icc_dram_ioctl (
  __CR_IN__ devb_t          devb,
  __CR_IN__ uint_t          cmd_code,
  __CR_OT__ void_t*         obuff,
  __CR_IN__ leng_t          osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ leng_t          isize,
  __CR_OT__ leng_t*         ret_size
    )
{
    sICC_DRAM*  real;
    sDEV_BINFO  info;

    if (ret_size != NULL)
        *ret_size = 0x00;
    real = (sICC_DRAM*)devb;
    switch (cmd_code)
    {
        /* 自动设置端口和速度 */
        case ICC_AUTO_SET_PORT:
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
            info.devb_name = "DRAM ICCARD";
            struct_cpy(obuff, &info, sDEV_BINFO);
            *ret_size = sizeof(sDEV_BINFO);
            break;

        /* 不支持的控制码 */
        default:
            real->devb.error = CR_DEVB_ERR_CONTROL_CODE;
            return (FALSE);
    }
    CR_NOUSE(ibuff);
    CR_NOUSE(isize);
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*
---------------------------------------
    打开 DRAM IC 卡设备
---------------------------------------
*/
static devb_t
icc_dram_open (
  __CR_IN__ leng_t  size
    )
{
    sICC_DRAM*  rett;

    rett = struct_new(sICC_DRAM);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sICC_DRAM);
    rett->m_data = (byte_t*)mem_malloc(size);
    if (rett->m_data == NULL) {
        mem_free(rett);
        return (NULL);
    }
    rett->m_size = size;
    rett->devb.read  = icc_dram_read;
    rett->devb.write = icc_dram_write;
    rett->devb.ioctl = icc_dram_ioctl;
    rett->devb.close = icc_dram_close;
    return ((devb_t)rett);
}

/*****************************************************************************/
/*                               S50/S70 ICC                                 */
/*****************************************************************************/

/*
---------------------------------------
    打开 DRAM S50 IC 卡设备
---------------------------------------
*/
static devb_t
m1_s50_open (
  __CR_IN__ const ansi_t*   name
    )
{
    /* (16 * 3 - 1) * 16 = 752 */
    CR_NOUSE(name);
    return (icc_dram_open(752));
}

/*
---------------------------------------
    打开 DRAM S70 IC 卡设备
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
    return (icc_dram_open(3440));
}

/*****************************************************************************/
/*                                24CXX ICC                                  */
/*****************************************************************************/

/*
---------------------------------------
    打开 DRAM 24C64 IC 卡设备
---------------------------------------
*/
static devb_t
e24c64_open (
  __CR_IN__ const ansi_t*   name
    )
{
    CR_NOUSE(name);
    return (icc_dram_open(CR_K2B(8)));
}

/*
---------------------------------------
    打开 DRAM 24C128 IC 卡设备
---------------------------------------
*/
static devb_t
e24c128_open (
  __CR_IN__ const ansi_t*   name
    )
{
    CR_NOUSE(name);
    return (icc_dram_open(CR_K2B(16)));
}

/*****************************************************************************/
/*                                驱动注册表                                 */
/*****************************************************************************/

/* S50 */
const sDEV_BNODE _rom_ g_icc_dram_s50 =
{
    "/iccard/dram/s50", m1_s50_open,
};

/* S70 */
const sDEV_BNODE _rom_ g_icc_dram_s70 =
{
    "/iccard/dram/s70", m1_s70_open,
};

/* 24C64 */
const sDEV_BNODE _rom_ g_icc_dram_24c64 =
{
    "/iccard/dram/24c64", e24c64_open,
};

/* 24C128 */
const sDEV_BNODE _rom_ g_icc_dram_24c128 =
{
    "/iccard/dram/24c128", e24c128_open,
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
