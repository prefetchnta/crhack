/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 快速排序函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 用到的宏 */
#define __CUTOFF     8
#define __STKSIZ    (8 * sizeof(void_t*) - 2)

/*
---------------------------------------
    交换两个元素
---------------------------------------
*/
static void_t
qs_swap (
  __CR_IO__ byte_t* a,
  __CR_IO__ byte_t* b,
  __CR_IN__ leng_t  width
    )
{
    byte_t  temp;

    if (a != b) {
        for (; width != 0; width--) {
            temp = *a;
            *a++ = *b;
            *b++ = temp;
        }
    }
}

/*
---------------------------------------
    短数组排序
---------------------------------------
*/
static void_t
qs_shortsort (
  __CR_IO__ byte_t* lo,
  __CR_IO__ byte_t* hi,
  __CR_IN__ leng_t  width,
  __CR_IN__ sint_t  (*comp)(const void_t*, const void_t*)
    )
{
    byte_t  *ptr, *max;

    while (hi > lo) {
        max = lo;
        for (ptr = lo + width; ptr <= hi; ptr += width) {
            if (comp(ptr, max) > 0)
                max = ptr;
        }
        qs_swap(max, hi, width);
        hi -= width;
    }
}

/*
---------------------------------------
    快速排序
---------------------------------------
*/
static void_t
qsort2 (
  __CR_IO__ void_t* base,
  __CR_IN__ leng_t  num,
  __CR_IN__ leng_t  width,
  __CR_IN__ sint_t  (*comp)(const void_t*, const void_t*)
    )
{
    leng_t  size;
    dist_t  stkptr;
    byte_t  *mid, *lo, *hi;
    byte_t  *loguy, *higuy;
    byte_t  *lostk[__STKSIZ];
    byte_t  *histk[__STKSIZ];

    /* 安全检查 */
    if (num < 2 || width == 0)
        return;
    stkptr = 0;
    lo = (byte_t*)base;
    hi = (byte_t*)base + width * (num - 1);

_recurse:
    size = (leng_t)(hi - lo) / width + 1;
    if (size <= __CUTOFF) {
        qs_shortsort(lo, hi, width, comp);
    }
    else {
        mid = lo + (size / 2) * width;
        if (comp(lo, mid) > 0)
            qs_swap(lo, mid, width);
        if (comp(lo,  hi) > 0)
            qs_swap(lo,  hi, width);
        if (comp(mid, hi) > 0)
            qs_swap(mid, hi, width);

        loguy = lo;
        higuy = hi;
        for (;;) {
            if (mid > loguy) {
                do {
                    loguy += width;
                } while (loguy < mid && comp(loguy, mid) <= 0);
            }

            if (mid <= loguy) {
                do {
                    loguy += width;
                } while (loguy <= hi && comp(loguy, mid) <= 0);
            }

            do {
                higuy -= width;
            } while (higuy > mid && comp(higuy, mid) > 0);

            if (higuy < loguy)
                break;
            qs_swap(loguy, higuy, width);

            if (mid == higuy)
                mid = loguy;
        }

        higuy += width;
        if (mid < higuy) {
            do {
                higuy -= width;
            } while (higuy > mid && comp(higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do {
                higuy -= width;
            } while (higuy > lo && comp(higuy, mid) == 0);
        }

        if (higuy - lo >= hi - loguy) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }

            if (loguy < hi) {
                lo = loguy;
                goto _recurse;
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;
            }

            if (lo < higuy) {
                hi = higuy;
                goto _recurse;
            }
        }
    }

    if (--stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto _recurse;
    }
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
