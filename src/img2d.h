/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 2D 图形图像内部头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_IMG2D_H__
#define __CR_IMG2D_H__

#include "gfx2.h"

/* 代码简化宏 (SRC) */
#define CR_MAKE_SRC(dx, dy, N) \
        if (!src->gdi) { \
            sbpl = src->bpl; \
            stmp = pixel_addr##N(src, (dx), (dy)); \
        } else { \
            sbpl = 0 - src->bpl; \
            stmp = pixel_addr##N(src, (dx), src->clip_win.y2 - (dy)); \
        }

/* 代码简化宏 (DST) */
#define CR_MAKE_DST(dx, dy, N) \
        if (!dst->gdi) { \
            dbpl = dst->bpl; \
            dtmp = pixel_addr##N(dst, (dx), (dy)); \
        } else { \
            dbpl = 0 - dst->bpl; \
            dtmp = pixel_addr##N(dst, (dx), dst->clip_win.y2 - (dy)); \
        }

/* 公用的内部函数 */
extern sIMAGE*  safe_convert (sIMAGE *dst, uint_t dst_fmt,
                    const sIMAGE *src, uint_t src_fmt, uint_t *dx, uint_t *dy,
                              uint_t *width, uint_t *height);

#endif  /* !__CR_IMG2D_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
