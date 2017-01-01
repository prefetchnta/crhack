/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-10  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 设备驱动函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "datlib.h"
#include "devlib.h"
#include "strlib.h"

/* 块设备管理器内部结构 */
typedef struct
{
        sCURBEAD    devs;   /* 块设备查找表 */

} sDEV_BLK_MGR;

/*
---------------------------------------
    成员比较回调
---------------------------------------
*/
static bool_t
function_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sDEV_BNODE* unit = (sDEV_BNODE*)obj;

    return (str_cmpA(unit->device_name, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    成员索引回调
---------------------------------------
*/
static uint_t
function_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/*****************************************************************************/
/*                               块设备管理组                                */
/*****************************************************************************/

/*
=======================================
    块设备管理器初始化
=======================================
*/
CR_API devb_mgr_t
devb_mgr_init (
  __CR_IN__ uint_t  count
    )
{
    sDEV_BLK_MGR*   rett;

    rett = struct_new(sDEV_BLK_MGR);
    if (rett == NULL)
        return (NULL);
    if (!curbead_initT(&rett->devs, sDEV_BNODE, count)) {
        mem_free(rett);
        return (NULL);
    }
    rett->devs.comp = function_comp;
    rett->devs.find = function_find;
    return ((devb_mgr_t)rett);
}

/*
=======================================
    块设备管理器释放
=======================================
*/
CR_API void_t
devb_mgr_free (
  __CR_IN__ devb_mgr_t  devb_mgr
    )
{
    sDEV_BLK_MGR*   real;

    real = (sDEV_BLK_MGR*)devb_mgr;
    curbead_freeT(&real->devs, sDEV_BNODE);
    mem_free((void_t*)devb_mgr);
}

/*
=======================================
    块设备驱动加载
=======================================
*/
CR_API bool_t
devb_mgr_load (
  __CR_IO__ devb_mgr_t          devb_mgr,
  __CR_IN__ const sDEV_BNODE*   devb_node
    )
{
    sDEV_BLK_MGR*   real;

    real = (sDEV_BLK_MGR*)devb_mgr;
    if (curbead_insertT(&real->devs, sDEV_BNODE,
            devb_node->device_name, devb_node) == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    块设备驱动卸载
=======================================
*/
CR_API bool_t
devb_mgr_unload (
  __CR_IO__ devb_mgr_t      devb_mgr,
  __CR_IN__ const ansi_t*   devb_name
    )
{
    sDEV_BLK_MGR*   real;

    real = (sDEV_BLK_MGR*)devb_mgr;
    return (curbead_deleteT(&real->devs, sDEV_BNODE, devb_name));
}

/*****************************************************************************/
/*                               块设备操作组                                */
/*****************************************************************************/

/*
=======================================
    块设备打开
=======================================
*/
CR_API devb_t
devb_open (
  __CR_IN__ devb_mgr_t      devb_mgr,
  __CR_IN__ const ansi_t*   devb_name,
  __CR_IN__ const ansi_t*   sub_type
    )
{
    sDEV_BNODE*     node;
    sDEV_BLK_MGR*   real;

    real = (sDEV_BLK_MGR*)devb_mgr;
    node = curbead_findT(&real->devs, sDEV_BNODE, devb_name);
    if (node == NULL || node->open == NULL)
        return (NULL);
    return (node->open(sub_type));
}

/*
=======================================
    块设备关闭
=======================================
*/
CR_API bool_t
devb_close (
  __CR_IN__ devb_t  devb
    )
{
    sDEV_BLOCK* real;

    real = (sDEV_BLOCK*)devb;
    if (real->close == NULL) {
        real->error = CR_DEVB_ERR_NO_CLOSE;
        return (FALSE);
    }
    real->close(devb);
    return (TRUE);
}

/*
=======================================
    返回最后的操作错误
=======================================
*/
CR_API int32u
devb_get_error (
  __CR_IN__ devb_t  devb
    )
{
    sDEV_BLOCK* real;

    real = (sDEV_BLOCK*)devb;
    return (real->error);
}

/*
=======================================
    设置最后的操作错误
=======================================
*/
CR_API void_t
devb_set_error (
  __CR_IO__ devb_t  devb,
  __CR_IN__ int32u  error
    )
{
    sDEV_BLOCK* real;

    real = (sDEV_BLOCK*)devb;
    real->error = error;
}

/*
=======================================
    块设备读取
=======================================
*/
CR_API leng_t
devb_read (
  __CR_IN__ devb_t  devb,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  addr,
  __CR_IN__ leng_t  size
    )
{
    sDEV_BLOCK* real;

    real = (sDEV_BLOCK*)devb;
    if (real->read == NULL) {
        real->error = CR_DEVB_ERR_NO_READ;
        return (CR_P_ERROR);
    }
    return (real->read(devb, data, addr, size));
}

/*
=======================================
    块设备写入
=======================================
*/
CR_API leng_t
devb_write (
  __CR_IN__ devb_t          devb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          addr,
  __CR_IN__ leng_t          size
    )
{
    sDEV_BLOCK* real;

    real = (sDEV_BLOCK*)devb;
    if (real->write == NULL) {
        real->error = CR_DEVB_ERR_NO_WRITE;
        return (CR_P_ERROR);
    }
    return (real->write(devb, data, addr, size));
}

/*
=======================================
    块设备控制
=======================================
*/
CR_API bool_t
devb_ioctl (
  __CR_IN__ devb_t          devb,
  __CR_IN__ uint_t          cmd_code,
  __CR_OT__ void_t*         obuff,
  __CR_IN__ leng_t          osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ leng_t          isize,
  __CR_OT__ leng_t*         ret_size
    )
{
    sDEV_BLOCK* real;

    real = (sDEV_BLOCK*)devb;
    if (real->ioctl == NULL) {
        real->error = CR_DEVB_ERR_NO_IOCTL;
        return (FALSE);
    }
    return (real->ioctl(devb, cmd_code, obuff, osize,
                  ibuff, isize, ret_size));
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
