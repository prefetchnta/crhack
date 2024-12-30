
/* 头文件组 */
#include "crhack.h"

/*
---------------------------------------
    出错处理的回调
---------------------------------------
*/
static void_t
errs_hook (
  __CR_IN__ const sERROR*   errs
    )
{
    CR_NOUSE(errs);
}

/*
=======================================
    main 程序入口
=======================================
*/
int main (int argc, char *argv[])
{
    CR_NOUSE(argc);
    CR_NOUSE(argv);

    /* 生成系统 */
    crhack_core_init();
    error_hook(errs_hook);

    /* 设置为 CUI 程序 */
    if (!set_app_type(CR_APP_CUI)) {
        return (-1);
    }

    /************/
    /* 程序内容 */
    /************/

    /* 释放系统 */
    crhack_core_free();
    return (TRUE);
}
