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
/*  >>>>>>>>>>>>>>>>>> DTM_NEXT JZT-969A 指纹仪驱动函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "finger.h"
#include "memlib.h"
#include "mtplib.h"
#include "plugin.h"

/* JZT-969A 指纹仪接口函数 */
#define FINGER_FMB_SIZE     256     /* 模板的大小 */
#define FINGER_PIC_WIDTH    152     /* 指纹图片宽 */
#define FINGER_PIC_HEIGHT   200     /* 指纹图片高 */
typedef sint_t  (CCCCALL *GFP_GetImage_t) (void_t*, uint_t*);
typedef sint_t  (CCCCALL *GFP_CheckFinger_t) (void_t*, uint_t);
typedef sint_t  (STDCALL *GFP_Extract_t) (void_t*, void_t*, uint_t*);
typedef sint_t  (STDCALL *GFP_Verify_t) (void_t*, void_t*, uint_t);
typedef sint_t  (STDCALL *GFP_Enroll_t) (byte_t**, void_t*, uint_t*);

/* JZT-969A 指纹仪内部结构 */
typedef struct
{
        /* 通用部分 */
        sDEV_BLOCK  devb;

        /* 个性部分 */
        sbin_t  m_sbin1;    /* 模块句柄 */
        sbin_t  m_sbin2;    /* 模块句柄 */
        int32u  m_param;    /* 压力系数 */

        /* 模块接口函数 */
        GFP_Enroll_t        GFP_Enroll;         /* 合成模板 */
        GFP_Verify_t        GFP_Verify;         /* 比较特征 */
        GFP_Extract_t       GFP_Extract;        /* 抽取特征 */
        GFP_GetImage_t      GFP_GetImage;       /* 采集图像 */
        GFP_CheckFinger_t   GFP_CheckFinger;    /* 校验指纹 */

} sFNG_JZT969;

/*****************************************************************************/
/*                                 通用部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    JZT-969A 采集指纹图像
---------------------------------------
*/
static leng_t
fng_jzt969a_get_pic (
  __CR_IO__ sFNG_JZT969*    real,
  __CR_OT__ void_t*         data
    )
{
    sint_t  ret = 0;
    uint_t  idx, size = 0;

    /* 重复采集10次 */
    for (idx = 0; idx < 10; idx++) {
        real->GFP_GetImage(data, &size);
        ret = real->GFP_CheckFinger(data, real->m_param);
        if (ret == 0)
            break;
        thread_sleep(500);
    }
    if (ret == 0) {
        real->devb.error = CR_DEVB_ERR_OK;
        return ((leng_t)size);
    }
    real->devb.error = CR_DEVB_ERR_FNG_JZT969A_PICT;
    return (0);
}

/*
---------------------------------------
    JZT-969A 采集指纹模板
---------------------------------------
*/
static leng_t
fng_jzt969a_get_des (
  __CR_IO__ sFNG_JZT969*    real,
  __CR_OT__ void_t*         data,
  __CR_IN__ bool_t          get_pic
    )
{
    sint_t  rett;
    uint_t  idx, pic_size, size = 0;
    byte_t  feature[3][512], *group[3];
    byte_t  pic[FINGER_PIC_WIDTH * FINGER_PIC_HEIGHT];

    for (idx = 0; idx < 3; idx++) {
        size = (uint_t)fng_jzt969a_get_pic(real, pic);
        if (size == 0)
            return (0);
        pic_size = size;
        rett = real->GFP_Extract(pic, feature[idx], &size);
        if (rett < 0) {
            real->devb.error = CR_DEVB_ERR_FNG_JZT969A_EXTR;
            return (0);
        }
        thread_sleep(1000);
    }
    group[0] = feature[0];
    group[1] = feature[1];
    group[2] = feature[2];
    rett = real->GFP_Enroll(group, data, &size);
    if (rett < 0) {
        real->devb.error = CR_DEVB_ERR_FNG_JZT969A_ENRL;
        return (0);
    }
    if (get_pic) {
        data = (byte_t*)data + size;
        mem_cpy(data, pic, pic_size);
        size += pic_size;
    }
    real->devb.error = CR_DEVB_ERR_OK;
    return ((leng_t)size);
}

/*
---------------------------------------
    JZT-969A 验证当前指纹
---------------------------------------
*/
static bool_t
fng_jzt969a_check (
  __CR_IO__ sFNG_JZT969*    real,
  __CR_IN__ void_t*         data
    )
{
    sint_t  rett;
    uint_t  size;
    byte_t  feature[512];
    byte_t  pic[FINGER_PIC_WIDTH * FINGER_PIC_HEIGHT];

    size = (uint_t)fng_jzt969a_get_pic(real, pic);
    if (size == 0)
        return (FALSE);
    rett = real->GFP_Extract(pic, feature, &size);
    if (rett < 0) {
        real->devb.error = CR_DEVB_ERR_FNG_JZT969A_EXTR;
        return (FALSE);
    }
    rett = real->GFP_Verify(feature, data, 3);
    if (rett < 0) {
        real->devb.error = CR_DEVB_ERR_FNG_JZT969A_VERY;
        return (FALSE);
    }
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*****************************************************************************/
/*                                 接口部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    关闭 JZT-969A 指纹仪设备
---------------------------------------
*/
static void_t
fng_jzt969a_close (
  __CR_IN__ devb_t  devb
    )
{
    sFNG_JZT969*    real;

    real = (sFNG_JZT969*)devb;
    sbin_unload(real->m_sbin2);
    sbin_unload(real->m_sbin1);
    mem_free((void_t*)devb);
}

/*
---------------------------------------
    控制 JZT-969A 指纹仪设备
---------------------------------------
*/
static bool_t
fng_jzt969a_ioctl (
  __CR_IN__ devb_t          devb,
  __CR_IN__ uint_t          cmd_code,
  __CR_OT__ void_t*         obuff,
  __CR_IN__ leng_t          osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ leng_t          isize,
  __CR_OT__ leng_t*         ret_size
    )
{
    leng_t          size;
    sFNG_JZT969*    real;
    sFINGER_INFO    info;

    if (ret_size != NULL)
        *ret_size = 0x00;
    real = (sFNG_JZT969*)devb;
    switch (cmd_code)
    {
        /* 获取设备信息 */
        case FNG_GET_INFOR:
            if (ret_size == NULL) {
                real->devb.error = CR_DEVB_ERR_CONTROL_RETTS;
                return (FALSE);
            }
            if (osize < sizeof(sFINGER_INFO)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_OSIZE;
                return (FALSE);
            }
            info.dsize = FINGER_FMB_SIZE;
            info.pic_w = FINGER_PIC_WIDTH;
            info.pic_h = FINGER_PIC_HEIGHT;
            struct_cpy(obuff, &info, sFINGER_INFO);
            *ret_size = sizeof(sFINGER_INFO);
            break;

        /* 设置采集系数 */
        case FNG_SET_PARAM:
            if (isize != sizeof(int32u)) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            mem_cpy(&real->m_param, ibuff, sizeof(int32u));
            break;

        /* 采集指纹图像 */
        case FNG_GET_PICTURE:
            if (ret_size == NULL) {
                real->devb.error = CR_DEVB_ERR_CONTROL_RETTS;
                return (FALSE);
            }
            if (osize < FINGER_PIC_WIDTH * FINGER_PIC_HEIGHT) {
                real->devb.error = CR_DEVB_ERR_CONTROL_OSIZE;
                return (FALSE);
            }
            size = fng_jzt969a_get_pic(real, obuff);
            if (size == 0)
                return (FALSE);
            *ret_size = size;
            break;

        /* 采集指纹模板 */
        case FNG_GET_DESCOMP:
            if (ret_size == NULL) {
                real->devb.error = CR_DEVB_ERR_CONTROL_RETTS;
                return (FALSE);
            }
            if (osize < FINGER_FMB_SIZE) {
                real->devb.error = CR_DEVB_ERR_CONTROL_OSIZE;
                return (FALSE);
            }
            size = fng_jzt969a_get_des(real, obuff, FALSE);
            if (size == 0)
                return (FALSE);
            *ret_size = size;
            break;

        /* 采集图像模板 */
        case FNG_GET_ALL_DATA:
            if (ret_size == NULL) {
                real->devb.error = CR_DEVB_ERR_CONTROL_RETTS;
                return (FALSE);
            }
            if (osize < FINGER_FMB_SIZE +
                        FINGER_PIC_WIDTH * FINGER_PIC_HEIGHT) {
                real->devb.error = CR_DEVB_ERR_CONTROL_OSIZE;
                return (FALSE);
            }
            size = fng_jzt969a_get_des(real, obuff, TRUE);
            if (size == 0)
                return (FALSE);
            *ret_size = size;
            break;

        /* 验证当前指纹 */
        case FNG_CHECK_FINGER:
            if (isize < FINGER_FMB_SIZE) {
                real->devb.error = CR_DEVB_ERR_CONTROL_ISIZE;
                return (FALSE);
            }
            if (!fng_jzt969a_check(real, (void_t*)ibuff))
                return (FALSE);
            break;

        /* 不支持的控制码 */
        default:
            real->devb.error = CR_DEVB_ERR_CONTROL_CODE;
            return (FALSE);
    }
    real->devb.error = CR_DEVB_ERR_OK;
    return (TRUE);
}

/*****************************************************************************/
/*                             JZT-969A 指纹仪                               */
/*****************************************************************************/

/*
---------------------------------------
    打开 JZT-969A 指纹仪设备
---------------------------------------
*/
static devb_t
fng_jzt969a_open (
  __CR_IN__ const ansi_t*   name
    )
{
    sFNG_JZT969*    rett;

    CR_NOUSE(name);
    rett = struct_new(sFNG_JZT969);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sFNG_JZT969);
    rett->m_sbin1 = sbin_loadA("JZT_998PFB_Api.dll");
    if (rett->m_sbin1 == NULL)
        goto _failure1;
    rett->m_sbin2 = sbin_loadA("gfpv20_v3_0_2_for_win.dll");
    if (rett->m_sbin2 == NULL)
        goto _failure2;
    rett->GFP_Enroll = sbin_exportT(rett->m_sbin2, "GFP_Enroll",
                                    GFP_Enroll_t);
    rett->GFP_Verify = sbin_exportT(rett->m_sbin2, "GFP_Verify",
                                    GFP_Verify_t);
    rett->GFP_Extract = sbin_exportT(rett->m_sbin2, "GFP_Extract",
                                     GFP_Extract_t);
    rett->GFP_GetImage = sbin_exportT(rett->m_sbin1, "GFP_GetImage",
                                      GFP_GetImage_t);
    rett->GFP_CheckFinger = sbin_exportT(rett->m_sbin1, "GFP_CheckFinger",
                                         GFP_CheckFinger_t);
    if (rett->GFP_Enroll == NULL || rett->GFP_Extract  == NULL ||
        rett->GFP_Verify == NULL || rett->GFP_GetImage == NULL ||
        rett->GFP_CheckFinger == NULL)
        goto _failure3;
    rett->m_param = 40;
    rett->devb.ioctl = fng_jzt969a_ioctl;
    rett->devb.close = fng_jzt969a_close;
    return ((devb_t)rett);

_failure3:
    sbin_unload(rett->m_sbin2);
_failure2:
    sbin_unload(rett->m_sbin1);
_failure1:
    mem_free(rett);
    return (NULL);
}

/*****************************************************************************/
/*                                驱动注册表                                 */
/*****************************************************************************/

/* 24C64 */
const sDEV_BNODE _rom_ g_fng_jzt969a =
{
    "/finger/jzt969a", fng_jzt969a_open,
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
