/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-03-25  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 串口通讯函数库 for POSIX <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"

#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/* 串口内部结构 */
typedef struct
{
        int             hfile;              /* 串口的句柄 */
        struct termios  param;              /* 串口参数表 */
        struct timeval  rd, *timeout_rd;    /* 超时参数表 */
        struct timeval  wr, *timeout_wr;    /* 超时参数表 */
} sCOMM;

/* 全局串口列表 */
static sCOMM    s_comm[_CR_SIO_COUNT_];
static ansi_t*  s_name[_CR_SIO_COUNT_];

/* 刷新串口状态模式 */
static const int s_flush_mode[] =
{
    TCIFLUSH, TCOFLUSH, TCIOFLUSH,
};

/*
=======================================
    加载串口库
=======================================
*/
CR_API void_t
sio_init (void_t)
{
    mem_zero(s_comm, sizeof(s_comm));
    mem_zero(s_name, sizeof(s_name));
}

/*
=======================================
    释放串口库
=======================================
*/
CR_API void_t
sio_free (void_t)
{
    uint_t  idx;

    for (idx = 0; idx < _CR_SIO_COUNT_; idx++) {
        if (s_comm[idx].hfile != 0)
            sio_close(idx);
    }
}

/*
=======================================
    设置设备名称
=======================================
*/
CR_API bool_t
sio_set_name (
  __CR_IN__ uint_t          port,
  __CR_IN__ const ansi_t*   name
    )
{
    /* 端口号 */
    if (port >= _CR_SIO_COUNT_)
        return (FALSE);
    s_name[port] = (ansi_t*)name;
    return (TRUE);
}

/*
=======================================
    设置串口整体参数
=======================================
*/
CR_API bool_t
sio_setup (
  __CR_IN__ uint_t  port,
  __CR_IN__ int32u  baud,
  __CR_IN__ uint_t  bits,
  __CR_IN__ uint_t  parity,
  __CR_IN__ uint_t  stop
    )
{
    sCOMM*          comm;
    speed_t         nbps;
    struct termios  param;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (FALSE);
    comm = &s_comm[port];
    mem_cpy(&param, &comm->param, sizeof(param));

    cfmakeraw(&param);
#if defined(CNEW_RTSCTS)
    param.c_cflag &= ~CNEW_RTSCTS;
#endif
    param.c_cflag |= (CLOCAL | CREAD);
    param.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    param.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* 校验位 */
    switch (parity)
    {
        case CR_SIO_NOP:
            param.c_cflag &= ~PARODD;
            param.c_cflag &= ~PARENB;
            param.c_iflag &= ~(INPCK | ISTRIP);
            break;

        case CR_SIO_ODD:
            param.c_cflag |=  PARODD;
            param.c_cflag |=  PARENB;
            param.c_iflag |= (INPCK | ISTRIP);
            break;

        case CR_SIO_EVEN:
            param.c_cflag &= ~PARODD;
            param.c_cflag |=  PARENB;
            param.c_iflag |= (INPCK | ISTRIP);
            break;

        default:
            return (FALSE);
    }

    /* 停止位 */
    switch (stop)
    {
        case CR_SIO_STOP10:
            param.c_cflag &= ~CSTOPB;
            break;

        case CR_SIO_STOP20:
            param.c_cflag |=  CSTOPB;
            break;

        default:
            return (FALSE);
    }

    /* 数据位 */
    param.c_cflag &= ~CSIZE;
    switch (bits)
    {
        case 5: param.c_cflag |= CS5; break;
        case 6: param.c_cflag |= CS6; break;
        case 7: param.c_cflag |= CS7; break;
        case 8: param.c_cflag |= CS8; break;

        default:
            return (FALSE);
    }

    /* 波特率 */
    switch (baud)
    {
        case 50: nbps = B50; break;
        case 75: nbps = B75; break;
        case 110: nbps = B110; break;
        case 134: nbps = B134; break;
        case 150: nbps = B150; break;
        case 200: nbps = B200; break;
        case 300: nbps = B300; break;
        case 600: nbps = B600; break;
        case 1200: nbps = B1200; break;
        case 1800: nbps = B1800; break;
        case 2400: nbps = B2400; break;
        case 4800: nbps = B4800; break;
        case 9600: nbps = B9600; break;
        case 19200: nbps = B19200; break;
        case 38400: nbps = B38400; break;
        case 57600: nbps = B57600; break;
        case 115200: nbps = B115200; break;

        default:
            return (FALSE);
    }
    cfsetispeed(&param, nbps);
    cfsetospeed(&param, nbps);

    if (tcsetattr(comm->hfile, TCSANOW, &param) != 0) {
        tcgetattr(comm->hfile, &comm->param);
        return (FALSE);
    }
    mem_cpy(&comm->param, &param, sizeof(param));
    return (TRUE);
}

/*
=======================================
    设置串口读取超时
=======================================
*/
CR_API bool_t
sio_set_rd_timeout (
  __CR_IN__ uint_t  port,
  __CR_IN__ uint_t  interval,
  __CR_IN__ uint_t  multiplier,
  __CR_IN__ uint_t  constant
    )
{
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (FALSE);

    /* 设置参数 */
    comm = &s_comm[port];
    if (constant == ((uint_t)-1)) {
        comm->timeout_rd = NULL;
    }
    else {
        comm->timeout_rd = &comm->rd;
        constant += interval * multiplier;
        comm->rd.tv_sec  = (constant / 1000);
        comm->rd.tv_usec = (constant % 1000) * 1000;
    }
    return (TRUE);
}

/*
=======================================
    设置串口写入超时
=======================================
*/
CR_API bool_t
sio_set_wr_timeout (
  __CR_IN__ uint_t  port,
  __CR_IN__ uint_t  multiplier,
  __CR_IN__ uint_t  constant
    )
{
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (FALSE);

    /* 设置参数 */
    comm = &s_comm[port];
    if (constant == ((uint_t)-1)) {
        comm->timeout_wr = NULL;
    }
    else {
        comm->timeout_wr = &comm->wr;
        comm->wr.tv_sec  = (constant / 1000);
        comm->wr.tv_usec = (constant % 1000) * 1000;
    }
    CR_NOUSE(multiplier);
    return (TRUE);
}

/*
=======================================
    打开串口
=======================================
*/
CR_API bool_t
sio_open (
  __CR_IN__ uint_t  port
    )
{
    sCOMM*  comm;
    sint_t  hfile;
    ansi_t  name[16];
    /* ----------- */
    struct termios  param;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile != 0)
        return (FALSE);

    /* 打开串口 */
    if (s_name[port] != NULL) {
        hfile = open(s_name[port], O_RDWR | O_SYNC | O_NOCTTY | O_NDELAY);
    }
    else {
        sprintf(name, "/dev/ttyS%u", port);
        hfile = open(name, O_RDWR | O_SYNC | O_NOCTTY | O_NDELAY);
    }
    if (hfile <= 0)
        return (FALSE);
    if (fcntl(hfile, F_SETFL, 0) < 0)
        goto _failure;
    if (tcgetattr(hfile, &param) < 0)
        goto _failure;
    comm = &s_comm[port];
    struct_zero(comm, sCOMM);
    comm->hfile = hfile;
    mem_cpy(&comm->param, &param, sizeof(param));
    return (TRUE);

_failure:
    close(hfile);
    return (FALSE);
}

/*
=======================================
    关闭串口
=======================================
*/
CR_API bool_t
sio_close (
  __CR_IN__ uint_t  port
    )
{
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (FALSE);

    /* 释放句柄 */
    comm = &s_comm[port];
    close(comm->hfile);
    struct_zero(comm, sCOMM);
    return (TRUE);
}

/*
=======================================
    刷新串口状态
=======================================
*/
CR_API bool_t
sio_flush (
  __CR_IN__ uint_t  port,
  __CR_IN__ uint_t  flags
    )
{
    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (FALSE);

    /* 阻塞写入 */
    if (flags == CR_SIO_FLU_WR) {
        if (fsync(s_comm[port].hfile) == -1)
            return (FALSE);
        return (TRUE);
    }

    /* 过滤参数 */
    if (flags > CR_SIO_FLU_RT)
        return (FALSE);

    /* 刷新串口 */
    if (tcflush(s_comm[port].hfile, s_flush_mode[flags]) != 0)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    从串口读取一个字节
---------------------------------------
*/
static sint_t
sio_read_byte (
  __CR_IN__ const sCOMM*    comm,
  __CR_OT__ void_t*         pntr
    )
{
    fd_set          rset;
    struct timeval  tout;

    FD_ZERO(&rset);
    FD_SET(comm->hfile, &rset);
    mem_cpy(&tout, comm->timeout_rd, sizeof(tout));
    if (select(comm->hfile + 1, &rset, NULL, NULL, &tout) <= 0)
        return (-1);
    if (FD_ISSET(comm->hfile, &rset))
        return ((sint_t)read(comm->hfile, pntr, 1));
    return (-1);
}

/*
=======================================
    读取串口数据
=======================================
*/
CR_API uint_t
sio_read (
  __CR_IN__ uint_t  port,
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    sCOMM*  comm;
    uint_t  back;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (CR_U_ERROR);

    if (size == 0)
        return (0);
    comm = &s_comm[port];

    /* 读取数据 */
    if (comm->timeout_rd == NULL)
        return ((sint_t)read(comm->hfile, data, size));
    for (back = 0; back < size; back++) {
        if (sio_read_byte(comm, data) < 0)
            break;
        data = (byte_t*)data + 1;
    }
    return (back);
}

/*
=======================================
    写入串口数据
=======================================
*/
CR_API uint_t
sio_write (
  __CR_IN__ uint_t          port,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    int             retc;
    sCOMM*          comm;
    fd_set          wset;
    struct timeval  tout;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == 0)
        return (CR_U_ERROR);

    if (size == 0)
        return (0);
    comm = &s_comm[port];

    /* 写入数据 */
    if (comm->timeout_wr == NULL)
        return ((sint_t)write(comm->hfile, data, size));
    FD_ZERO(&wset);
    FD_SET(comm->hfile, &wset);
    mem_cpy(&tout, comm->timeout_wr, sizeof(tout));
    retc = select(comm->hfile + 1, NULL, &wset, NULL, &tout);
    if (retc < 0)
        return (CR_U_ERROR);
    if (retc == 0)
        return (0);
    if (FD_ISSET(comm->hfile, &wset))
        return ((sint_t)write(comm->hfile, data, size));
    return (0);
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
