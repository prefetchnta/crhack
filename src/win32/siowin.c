/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-04-08  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 串口通讯函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"

#include <tchar.h>
#include <windows.h>

/* 串口内部结构 */
typedef struct
{
        HANDLE          hfile;      /* 串口的句柄 */
        DCB             param;      /* 串口参数表 */
        COMMTIMEOUTS    timeout;    /* 超时参数表 */
} sCOMM;

/* 全局串口列表 */
static sCOMM    s_comm[_CR_SIO_COUNT_];
static ansi_t*  s_name[_CR_SIO_COUNT_];

/* 刷新串口状态模式 */
static const DWORD s_flush_mode[] =
{
    PURGE_RXABORT | PURGE_RXCLEAR,  PURGE_TXABORT | PURGE_TXCLEAR,
    PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR,
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
        if (s_comm[idx].hfile != NULL)
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
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 校验位 */
    if (parity > CR_SIO_SPCE)
        return (FALSE);

    /* 停止位 */
    if (stop > CR_SIO_STOP20)
        return (FALSE);

    /* 数据位 */
    switch (bits)
    {
        case 4: case 5:
        case 6: case 7: case 8:
            break;

        default:
            return (FALSE);
    }

    /* 波特率 */
    switch (baud)
    {
        case 110: case 300: case 600:
        case 1200: case 2400: case 4800: case 9600:
        case 14400: case 19200: case 38400: case 57600:
        case 115200: case 128000: case 256000:
            break;

        default:
            return (FALSE);
    }

    /* 设置参数 */
    comm = &s_comm[port];
    comm->param.Parity = (BYTE)parity;
    comm->param.BaudRate = (UINT)baud;
    comm->param.ByteSize = (BYTE)bits;
    comm->param.StopBits = (BYTE)stop;
    if (!SetCommState(comm->hfile, &comm->param)) {
        GetCommState(comm->hfile, &comm->param);
        return (FALSE);
    }
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
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 设置参数 */
    comm = &s_comm[port];
    comm->timeout.ReadIntervalTimeout = interval;
    comm->timeout.ReadTotalTimeoutMultiplier = multiplier;
    comm->timeout.ReadTotalTimeoutConstant = constant;
    if (!SetCommTimeouts(comm->hfile, &comm->timeout)) {
        GetCommTimeouts(comm->hfile, &comm->timeout);
        return (FALSE);
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
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 设置参数 */
    comm = &s_comm[port];
    comm->timeout.WriteTotalTimeoutMultiplier = multiplier;
    comm->timeout.WriteTotalTimeoutConstant = constant;
    if (!SetCommTimeouts(comm->hfile, &comm->timeout)) {
        GetCommTimeouts(comm->hfile, &comm->timeout);
        return (FALSE);
    }
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
    sCOMM*          comm;
    HANDLE          hfile;
    DCB             param;
    COMMTIMEOUTS    timeout;
    TCHAR           name[16];

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile != NULL)
        return (FALSE);

    /* 打开串口 */
    if (s_name[port] != NULL) {
        hfile = CreateFileA(s_name[port], GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else {
#if defined(_CR_OS_WINCE_)
        wsprintf(name, _T("COM%u:"), port);
#else
        wsprintf(name, _T("\\\\.\\COM%u"), port);
#endif
        hfile = CreateFile(name, GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    if (hfile == INVALID_HANDLE_VALUE)
        return (FALSE);

    /* 读取参数 */
    struct_zero(&param, DCB);
    param.DCBlength = sizeof(DCB);
    if (!GetCommState(hfile, &param))
        goto _failure;

    /* 读取超时 */
    if (!GetCommTimeouts(hfile, &timeout))
        goto _failure;

    /* 设置结构参数 */
    comm = &s_comm[port];
    comm->hfile = hfile;
    struct_cpy(&comm->param, &param, DCB);
    struct_cpy(&comm->timeout, &timeout, COMMTIMEOUTS);
    return (TRUE);

_failure:
    CloseHandle(hfile);
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
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 释放句柄 */
    comm = &s_comm[port];
    CloseHandle(comm->hfile);
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
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 阻塞写入 */
    if (flags == CR_SIO_FLU_WR) {
        if (!FlushFileBuffers(s_comm[port].hfile))
            return (FALSE);
        return (TRUE);
    }

    /* 过滤参数 */
    if (flags > CR_SIO_FLU_RT)
        return (FALSE);

    /* 刷新串口 */
    if (!PurgeComm(s_comm[port].hfile, s_flush_mode[flags]))
        return (FALSE);
    return (TRUE);
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
    DWORD   back;
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (CR_U_ERROR);

    if (size == 0)
        return (0);
    comm = &s_comm[port];

    /* 读取数据 */
    ReadFile(comm->hfile, data, size, &back, NULL);
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
    DWORD   back;
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (CR_U_ERROR);

    if (size == 0)
        return (0);
    comm = &s_comm[port];

    /* 写入数据 */
    WriteFile(comm->hfile, data, size, &back, NULL);
    return (back);
}

/*
=======================================
    设置串口读写缓冲区大小
=======================================
*/
CR_API bool_t
sio_set_buffer (
  __CR_IN__ uint_t  port,
  __CR_IN__ uint_t  rx_size,
  __CR_IN__ uint_t  tx_size
    )
{
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 设置参数 */
    comm = &s_comm[port];
    if (!SetupComm(comm->hfile, rx_size, tx_size))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    清除串口错误
=======================================
*/
CR_API bool_t
sio_clear_error (
  __CR_IN__ uint_t  port
    )
{
    DWORD   errs;
    sCOMM*  comm;
    COMSTAT stat;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 清除错误 */
    comm = &s_comm[port];
    if (!ClearCommError(comm->hfile, &errs, &stat))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取串口读写缓冲里剩余的字节数
=======================================
*/
CR_API bool_t
sio_rw_rest (
  __CR_IN__ uint_t  port,
  __CR_OT__ leng_t* rx_len,
  __CR_OT__ leng_t* tx_len
    )
{
    DWORD   errs;
    sCOMM*  comm;
    COMSTAT stat;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 获取串口状态 */
    comm = &s_comm[port];
    if (!ClearCommError(comm->hfile, &errs, &stat))
        return (FALSE);
    if (rx_len != NULL)
        *rx_len = stat.cbInQue;
    if (tx_len != NULL)
        *tx_len = stat.cbOutQue;
    return (TRUE);
}

/*
=======================================
    设置串口数据的第9位
=======================================
*/
CR_API bool_t
sio_set_bit9 (
  __CR_IN__ uint_t  port,
  __CR_IN__ bool_t  onoff
    )
{
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 设置参数 */
    comm = &s_comm[port];
    comm->param.Parity = onoff ? MARKPARITY : SPACEPARITY;
    if (!SetCommState(comm->hfile, &comm->param)) {
        GetCommState(comm->hfile, &comm->param);
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    读取串口辅助 IO 脚状态
=======================================
*/
CR_API bool_t
sio_io_get (
  __CR_IN__ uint_t  port,
  __CR_OT__ uint_t* value
    )
{
    DWORD   stat;
    sCOMM*  comm;

    /* 端口号 */
    *value = 0;
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 读取串口状态 */
    comm = &s_comm[port];
    if (!GetCommModemStatus(comm->hfile, &stat))
        return (FALSE);
    if (stat & MS_CTS_ON)
        *value |= CR_SIO_I_CTS;
    if (stat & MS_DSR_ON)
        *value |= CR_SIO_I_DSR;
    if (stat & MS_RING_ON)
        *value |= CR_SIO_I_RING;
    if (stat & MS_RLSD_ON)
        *value |= CR_SIO_I_RLSD;
    return (TRUE);
}

/*
=======================================
    设置串口辅助 IO 脚状态
=======================================
*/
CR_API bool_t
sio_io_set (
  __CR_IN__ uint_t  port,
  __CR_IN__ uint_t  pin,
  __CR_IN__ bool_t  onoff
    )
{
    DWORD   func;
    sCOMM*  comm;

    /* 端口号 */
    if (port >= _CR_SIO_COUNT_ ||
        s_comm[port].hfile == NULL)
        return (FALSE);

    /* 设置串口状态 */
    if (pin == CR_SIO_O_DTR)
        func = onoff ? SETDTR : CLRDTR;
    else
    if (pin == CR_SIO_O_RTS)
        func = onoff ? SETRTS : CLRRTS;
    else
        return (FALSE);
    comm = &s_comm[port];
    if (!EscapeCommFunction(comm->hfile, func))
        return (FALSE);
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
