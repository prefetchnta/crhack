/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-04-26  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack GPIO 函数库 for Linux <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/*
=======================================
    打开 GPIO 管脚
=======================================
*/
CR_API sint_t
gpio_open (
  __CR_IN__ uint_t  port,
  __CR_IN__ bool_t  dir_out
    )
{
    FILE*   fp;
    sint_t  fd;
    ansi_t  str[64];

    /* 设定 GPIO 方向 */
    fp = fopen("/sys/class/gpio/export", "w");
    if (fp == NULL)
        return (-1);
    fprintf(fp, "%u", port);
    fclose(fp);
    sprintf(str, "/sys/class/gpio/gpio%u/direction", port);
    fp = fopen(str, "rb+");
    if (fp == NULL)
        return (-1);
    if (dir_out)
        fprintf(fp, "out");
    else
        fprintf(fp, "in");
    fclose(fp);

    /* 打开控制文件 */
    sprintf(str, "/sys/class/gpio/gpio%u/value", port);
    if (dir_out)
        fd = open(str, O_WRONLY);
    else
        fd = open(str, O_RDONLY);
    return ((fd < 0) ? -1 : fd);
}

/*
=======================================
    关闭 GPIO 管脚
=======================================
*/
CR_API void_t
gpio_close (
  __CR_IN__ sint_t  gpio
    )
{
    close(gpio);
}

/*
=======================================
    设置 GPIO 管脚电平
=======================================
*/
CR_API bool_t
gpio_set (
  __CR_IN__ sint_t  gpio,
  __CR_IN__ bool_t  value
    )
{
    byte_t  cha = value ? 0x31 : 0x30;

    lseek(gpio, 0, SEEK_SET);
    if (write(gpio, &cha, 1) != 1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取 GPIO 管脚电平
=======================================
*/
CR_API bool_t
gpio_get (
  __CR_IN__ sint_t  gpio,
  __CR_OT__ bool_t* value
    )
{
    byte_t  cha = 0x31;

    lseek(gpio, 0, SEEK_SET);
    if (read(gpio, &cha, 1) != 1)
        return (FALSE);
    *value = cha & 1;
    return (TRUE);
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
