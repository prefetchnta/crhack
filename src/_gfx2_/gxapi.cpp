/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-04-11  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack GAME API 图形绘制接口实现 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "pixels.h"
#include "strlib.h"
#include "gfx2/gxapi.h"

#include <windows.h>

#include "gapi/gx.h"

/* GAME API 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "gx.lib")
#endif

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放图形绘制接口
---------------------------------------
*/
static void_t
iGFX2_GX_release (
  __CR_IN__ iGFX2*  that
    )
{
    GXEndDraw();
    GXCloseDisplay();
    mem_free(that->__back__.data);
    mem_free(that);
}

/*
---------------------------------------
    获取扩展的绘制接口
---------------------------------------
*/
static void_t*
iGFX2_GX_getMore (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iGFX2::GX") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    改变屏幕大小
---------------------------------------
*/
static bool_t
iGFX2_GX_reset (
  __CR_IN__ iGFX2*  that
    )
{
    /* 全屏应用 */
    CR_NOUSE(that);
    return (FALSE);
}

/*
---------------------------------------
    锁住后台缓冲
---------------------------------------
*/
static sIMAGE*
iGFX2_GX_lock (
  __CR_IN__ iGFX2*  that
    )
{
    return (&that->__back__);
}

/*
---------------------------------------
    解锁后台缓冲
---------------------------------------
*/
static void_t
iGFX2_GX_unlock (
  __CR_IN__ iGFX2*  that
    )
{
    CR_NOUSE(that);
}

/*
---------------------------------------
    显示后台缓冲
---------------------------------------
*/
static bool_t
iGFX2_GX_flip (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    iGFX2_GX*   real = (iGFX2_GX*)that;

    CR_NOUSE(sync);
    mem_cpy(real->m_main, real->__back__.data, real->__back__.size);
    return (TRUE);
}

/*
---------------------------------------
    清除后台缓冲
---------------------------------------
*/
#define _image_clear12  iGFX2_GX_clear12
#define _image_clear15  iGFX2_GX_clear15
#define _image_clear16  iGFX2_GX_clear16
#define _image_clear24  iGFX2_GX_clear24

#include "clear2d.inl"

/*
---------------------------------------
    设置后台调色板
---------------------------------------
*/
static bool_t
iGFX2_GX_setPal (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ uint_t  start,
  __CR_IN__ uint_t  count
    )
{
    /* 无法使用 */
    CR_NOUSE(that);
    CR_NOUSE(start);
    CR_NOUSE(count);
    return (FALSE);
}

/* 接口虚函数表 */
static const iGFX2_vtbl _rom_ s_img12_vtbl =
{
    iGFX2_GX_release, iGFX2_GX_getMore,
    iGFX2_GX_reset, iGFX2_GX_lock, iGFX2_GX_unlock,
    iGFX2_GX_flip, iGFX2_GX_clear12, iGFX2_GX_setPal,
};

static const iGFX2_vtbl _rom_ s_img15_vtbl =
{
    iGFX2_GX_release, iGFX2_GX_getMore,
    iGFX2_GX_reset, iGFX2_GX_lock, iGFX2_GX_unlock,
    iGFX2_GX_flip, iGFX2_GX_clear15, iGFX2_GX_setPal,
};

static const iGFX2_vtbl _rom_ s_img16_vtbl =
{
    iGFX2_GX_release, iGFX2_GX_getMore,
    iGFX2_GX_reset, iGFX2_GX_lock, iGFX2_GX_unlock,
    iGFX2_GX_flip, iGFX2_GX_clear16, iGFX2_GX_setPal,
};

static const iGFX2_vtbl _rom_ s_img24_vtbl =
{
    iGFX2_GX_release, iGFX2_GX_getMore,
    iGFX2_GX_reset, iGFX2_GX_lock, iGFX2_GX_unlock,
    iGFX2_GX_flip, iGFX2_GX_clear24, iGFX2_GX_setPal,
};

/*
=======================================
    生成 GAME API 图形绘制接口
=======================================
*/
CR_API iGFX2_GX*
create_gx_canvas (
  __CR_IN__ HWND    hwnd
    )
{
    uint_t              w, h;
    iGFX2_GX*           rett;
    GXDisplayProperties prop;

    /* 只支持高彩色 */
    if (GXOpenDisplay(hwnd, GX_FULLSCREEN) == 0)
        return (NULL);
    prop = GXGetDisplayProperties();
    if (prop.cBPP <= 8)
        goto _failure1;

    /* 可能是翻转的形式 */
    if (prop.cbxPitch < 0)
        prop.cbxPitch = -prop.cbxPitch;
    if (prop.cbyPitch < 0)
        prop.cbyPitch = -prop.cbyPitch;

    /* 生成接口对象 */
    rett = struct_new(iGFX2_GX);
    if (rett == NULL)
        goto _failure1;
    struct_zero(rett, iGFX2_GX);
    if (prop.cbxPitch > prop.cbyPitch) {
        w = prop.cyHeight;
        h = prop.cxWidth;
        rett->__back__.bpc = prop.cbyPitch;
        rett->__back__.bpl = prop.cbxPitch;
    }
    else {
        w = prop.cxWidth;
        h = prop.cyHeight;
        rett->__back__.bpc = prop.cbxPitch;
        rett->__back__.bpl = prop.cbyPitch;
    }
    rect_set_wh(&rett->__back__.clip_win, 0, 0, w, h);
    rect_set_wh(&rett->__back__.position, 0, 0, w, h);
    rett->__back__.size  = h;
    rett->__back__.size *= rett->__back__.bpl;
    if (prop.ffFormat & kfDirect555) {
        rett->__back__.fmt = CR_ARGBX555;
        rett->__vptr__ = &s_img15_vtbl;
    }
    else
    if (prop.ffFormat & kfDirect565) {
        rett->__back__.fmt = CR_ARGB565;
        rett->__vptr__ = &s_img16_vtbl;
    }
    else
    if (prop.ffFormat & kfDirect888) {
        rett->__back__.fmt = CR_ARGB888;
        rett->__vptr__ = &s_img24_vtbl;
    }
    else
    if (prop.ffFormat & kfDirect444) {
        rett->__back__.fmt = CR_ARGB4444;
        rett->__vptr__ = &s_img12_vtbl;
    }
    else {
        goto _failure2;
    }

    /* 创建后台缓冲 */
    rett->__back__.data = (byte_t*)mem_malloc(rett->__back__.size + 16);
    if (rett->__back__.data == NULL)
        goto _failure2;

    /* 获取前台缓冲 */
    rett->m_main = GXBeginDraw();
    if (rett->m_main == NULL) {
        mem_free(rett->__back__.data);
        goto _failure2;
    }
    mem_zero(rett->m_main, rett->__back__.size);
    return (rett);

_failure2:
    mem_free(rett);
_failure1:
    GXCloseDisplay();
    return (NULL);
}

#ifndef _CR_DLL_TOGETHER_
#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取绘图插件接口 (同名)
=======================================
*/
CR_API iGFX2*
create_canvas (
  __CR_IN__ void_t*         handle,
  __CR_IN__ uint_t          scn_cw,
  __CR_IN__ uint_t          scn_ch,
  __CR_IN__ uint_t          scn_fmt,
  __CR_IN__ bool_t          full,
  __CR_IN__ const int32u*   param,
  __CR_IN__ uint_t          count
    )
{
    CR_NOUSE(full); CR_NOUSE(param);
    CR_NOUSE(count); CR_NOUSE(scn_cw);
    CR_NOUSE(scn_ch); CR_NOUSE(scn_fmt);
    return ((iGFX2*)create_gx_canvas((HWND)handle));
}
#endif  /* _CR_BUILD_DLL_ */
#endif  /* !_CR_DLL_TOGETHER_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
