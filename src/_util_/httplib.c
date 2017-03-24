/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-20  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>>> HTTPLIB 函数库 <<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "fileio.h"
#include "memlib.h"
#include "parser.h"
#include "strlib.h"
#include "util/httplib.h"

/* 内部结构 */
typedef struct
{
        /* 请求部分 */
        ansi_t*     urls;
        ansi_t*     host;
        ansi_t*     path;
        uint_t      port;
        uint_t      vers;
        int32s      tout;
        int32s      wout;
        int32s      rout;
        bool_t      keep;
        iDATOT*     head;
        socket_t    netw;

        /* 响应部分 */
        sINIu*      res_ini;
        uint_t      res_len;
        uint_t      res_max;
        void_t*     res_dat;
        ansi_t*     res_hdr;
        uint_t      res_ver;
        uint_t      res_num;
        ansi_t*     res_str;

        /* 回调部分 */
        void_t*         cb_parm;
        uint_t          cb_size;
        void_t*         cb_buff;
        chttp_recv_t    cb_func;

} sHTTPLIB;

/* HTTP 版本表 */
static const ansi_t* s_http_ver[] =
{
    "HTTP/1.0", "HTTP/1.1",
};

/* HTTP 命令表 */
static const ansi_t* s_http_cmd[] =
{
    "GET", "HEAD", "PUT", "POST", "TRACE", "OPTIONS", "DELETE",
};

/*
=======================================
    创建 HTTP 连接
=======================================
*/
CR_API chttp_t
chttp_open (
  __CR_IN__ const ansi_t*   host,
  __CR_IN__ int16u          port,
  __CR_IN__ uint_t          vers,
  __CR_IN__ int32s          timeout
    )
{
    leng_t      size;
    ansi_t*     temp;
    sHTTPLIB*   rett;

    /* 只支持 1.0 和 1.1 */
    if (vers > 1)
        return (NULL);

    /* 创建对象 */
    rett = struct_new(sHTTPLIB);
    if (rett == NULL)
        return (NULL);

    /* 解析输入 URL */
    rett->urls = str_url_splitA(host, NULL, NULL, NULL, &rett->host,
                                &rett->port, &rett->path, NULL, NULL);
    if (rett->urls == NULL)
        goto _failure1;
    if (rett->path == NULL) {
        rett->path = str_dupA("/");
        if (rett->path == NULL)
            goto _failure2;
    }
    else {
        size = str_sizeA(rett->path);
        temp = str_allocA(size + 1);
        if (temp == NULL)
            goto _failure2;
        temp[0] = CR_AC('/');
        mem_cpy(temp + 1, rett->path, size);
        rett->path = temp;
    }
    if (rett->port == 0) {
        rett->port = (uint_t)port;
        if (rett->port == 0)
            rett->port = 80;
    }
    rett->vers = vers;
    rett->tout = timeout;
    rett->wout = 5000;
    rett->rout = 60000;
    rett->head = create_buff_out(128);
    if (rett->head == NULL)
        goto _failure3;

    /* 连接到主机 */
    if (vers == 0) {
        rett->netw = NULL;  /* HTTP1.0 默认短连接 */
        rett->keep = FALSE;
    }
    else {                  /* HTTP1.1 默认长连接 */
        rett->netw = client_tcp_open(rett->host, (int16u)rett->port,
                                     timeout);
        if (rett->netw == NULL)
            goto _failure4;
        rett->keep = TRUE;
        socket_set_timeout(rett->netw, rett->wout, rett->rout);
    }
    rett->res_len = 0;
    rett->res_max = ((uint_t)-1);
    rett->res_dat = NULL;
    rett->res_hdr = NULL;
    rett->res_ini = NULL;
    rett->res_ver = 0;
    rett->res_num = 0;
    rett->res_str = "";
    rett->cb_parm = NULL;
    rett->cb_size = 0;
    rett->cb_buff = NULL;
    rett->cb_func = NULL;
    return ((chttp_t)rett);

_failure4:
    CR_VCALL(rett->head)->release(rett->head);
_failure3:
    mem_free(rett->path);
_failure2:
    mem_free(rett->urls);
_failure1:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    关闭 HTTP 连接
=======================================
*/
CR_API void_t
chttp_close (
  __CR_IN__ chttp_t conn
    )
{
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    CR_VCALL(real->head)->release(real->head);
    if (real->netw != NULL)
        socket_close(real->netw);
    if (real->res_ini != NULL)
        ini_closeU(real->res_ini);
    TRY_FREE(real->cb_buff);
    TRY_FREE(real->res_hdr);
    TRY_FREE(real->res_dat);
    mem_free(real->path);
    mem_free(real->urls);
    mem_free(real);
}

/*
=======================================
    设置 HTTP 超时
=======================================
*/
CR_API void_t
chttp_timeout (
  __CR_IN__ chttp_t conn,
  __CR_IN__ int32s  wr_time,
  __CR_IN__ int32s  rd_time
    )
{
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    if (real->netw != NULL)
        socket_set_timeout(real->netw, wr_time, rd_time);
    real->wout = wr_time;
    real->rout = rd_time;
}

/*
=======================================
    主动断开 HTTP 连接
=======================================
*/
CR_API void_t
chttp_disconn (
  __CR_IN__ chttp_t conn
    )
{
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    if (real->netw != NULL) {
        socket_close(real->netw);
        real->netw = NULL;
    }
}

/*
=======================================
    复位 HTTP 状态
=======================================
*/
CR_API void_t
chttp_restart (
  __CR_IN__ chttp_t conn
    )
{
    chttp_head_rst(conn);
    chttp_disconn(conn);
}

/*
=======================================
    设置 HTTP 接收安全大小
=======================================
*/
CR_API void_t
chttp_safe_size (
  __CR_IN__ chttp_t conn,
  __CR_IN__ sint_t  size
    )
{
    ((sHTTPLIB*)conn)->res_max = (uint_t)size;
}

/*
=======================================
    设置 HTTP 接收回调
=======================================
*/
CR_API bool_t
chttp_callback (
  __CR_IN__ chttp_t         conn,
  __CR_IN__ chttp_recv_t    func,
  __CR_IN__ void_t*         param,
  __CR_IN__ uint_t          cache
    )
{
    void_t*     buff;
    sHTTPLIB*   real;

    /* 先释放之前的 */
    real = (sHTTPLIB*)conn;
    if (func == NULL || cache == 0) {
        SAFE_FREE(real->cb_buff);
        real->cb_parm = NULL;
        real->cb_size = 0;
        real->cb_func = NULL;
        return (TRUE);
    }

    /* 新开一片缓冲区 */
    buff = mem_malloc(cache);
    if (buff == NULL)
        return (FALSE);
    TRY_FREE(real->cb_buff);
    real->cb_buff = buff;
    real->cb_parm = param;
    real->cb_size = cache;
    real->cb_func = func;
    return (TRUE);
}

/*
=======================================
    判断 HTTP 头参数
=======================================
*/
CR_API bool_t
chttp_hdr_check (
  __CR_IN__ const ansi_t*   line,
  __CR_IN__ const ansi_t*   key,
  __CR_IN__ const ansi_t*   value
    )
{
    /* 必须去头尾空白 */
    if (str_strIA(line, key) == line) {
        line += str_lenA(key);
        line = str_chrA(line, CR_AC(':'));
        if (line != NULL) {
            line = str_strIA(line + 1, value);
            if (line != NULL) {
                line += str_lenA(value);
                if (*line == 0x00)
                    return (TRUE);
            }
        }
    }
    return (FALSE);
}

/*
=======================================
    返回 HTTP 头参数
=======================================
*/
CR_API ansi_t*
chttp_hdr_value (
  __CR_IN__ const ansi_t*   line,
  __CR_IN__ const ansi_t*   key
    )
{
    /* 必须去头尾空白 */
    if (str_strIA(line, key) == line) {
        line += str_lenA(key);
        line = str_chrA(line, CR_AC(':'));
        if (line != NULL)
            return (skip_spaceA(line + 1));
    }
    return (NULL);
}

/*
=======================================
    添加 HTTP 头参数
=======================================
*/
CR_API bool_t
chttp_head_add (
  __CR_IN__ chttp_t         conn,
  __CR_IN__ const ansi_t*   lines
    )
{
    bool_t      ret;
    leng_t      idx;
    leng_t      cnt;
    ansi_t*     str;
    ansi_t**    lst;
    sHTTPLIB*   real;

    /* 用换行分割的参数 */
    str = str_dupA(lines);
    if (str == NULL)
        return (FALSE);
    lst = str_splitA(str, CR_AC('\n'), &cnt);
    if (lst == NULL) {
        mem_free(str);
        return (FALSE);
    }

    /* 扫描头参数 */
    real = (sHTTPLIB*)conn;
    for (ret = TRUE, idx = 0; idx < cnt; idx++)
    {
        /* 去头尾空白 */
        str_trimA(lst[idx]);

        /* 特殊命令处理 */
        if (chttp_hdr_check(lst[idx], "Connection", "Keep-Alive"))
            real->keep = TRUE;

        /* 加入参数和换行符 */
        if (!CR_VCALL(real->head)->putsA(real->head, lst[idx], 0) ||
            CR_VCALL(real->head)->write(real->head, "\r\n", 2) != 2) {
            chttp_head_rst(conn);
            ret = FALSE;
            break;
        }
    }
    mem_free(lst);
    mem_free(str);
    return (ret);
}

/*
=======================================
    结束 HTTP 头参数
=======================================
*/
CR_API bool_t
chttp_head_fin (
  __CR_IN__ chttp_t conn
    )
{
    sHTTPLIB*   real = (sHTTPLIB*)conn;

    return (CR_VCALL(real->head)->putb_no(real->head, 0));
}

/*
=======================================
    清除 HTTP 头参数
=======================================
*/
CR_API void_t
chttp_head_rst (
  __CR_IN__ chttp_t conn
    )
{
    sHTTPLIB*   real = (sHTTPLIB*)conn;

    CR_VCALL(real->head)->reput(real->head, 0);
}

/*
=======================================
    发送 HTTP 请求 (字符串)
=======================================
*/
CR_API bool_t
chttp_req_str (
  __CR_IN__ chttp_t         conn,
  __CR_IN__ uint_t          method,
  __CR_IN__ const ansi_t*   path,
  __CR_IN__ const ansi_t*   body
    )
{
    uint_t  size = (body == NULL) ? 0 : (uint_t)str_lenA(body);

    return (chttp_req_bin(conn, method, path, body, size));
}

/*
---------------------------------------
    查找 HTTP 头结尾
---------------------------------------
*/
static uint_t
chttp_find_head_end (
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    uint_t  idx, len = size - 4;

    for (idx = 0; idx <= len; idx++) {
        if (mem_cmp(data, "\r\n\r\n", 4) == 0)
            return (idx);
        data = (byte_t*)data + 1;
    }
    return (size);
}

/*
---------------------------------------
    处理 HTTP 头命令
---------------------------------------
*/
static bool_t
chttp_do_head_cmd (
  __CR_IO__ sHTTPLIB*   conn
    )
{
    leng_t  idx;
    bool_t  shut;
    ansi_t* line;

    /* 按行拆分 */
    conn->res_ini = ini_parseU(conn->res_hdr);
    if (conn->res_ini == NULL)
        return (FALSE);
    if (conn->res_ini->count < 3) {
        ini_closeU(conn->res_ini);
        conn->res_ini = NULL;
        return (FALSE);
    }
    shut = !conn->keep;
    conn->res_ini->count -= 2;
    for (idx = 0; idx < conn->res_ini->count; idx++)
    {
        /* 去头尾空白 */
        line = conn->res_ini->lines[idx];
        str_trimA(line);

        if (chttp_hdr_check(line, "Connection", "Close"))
            shut = TRUE;
        else
        if (chttp_hdr_check(line, "Connection", "Keep-Alive"))
            shut = FALSE;
    }

    /* 短连接关闭连接 */
    if (shut) chttp_disconn((chttp_t)conn);

    /* 解析响应行 */
    line = conn->res_ini->lines[0];
    if (chr_cmpA(line, "HTTP/1.1 ", 9) == 0) {
        conn->res_ver = 1;
    }
    else
    if (chr_cmpA(line, "HTTP/1.0 ", 9) == 0) {
        conn->res_ver = 0;
    }
    else {
        ini_closeU(conn->res_ini);
        conn->res_ini = NULL;
        return (FALSE);
    }
    line += 9;
    conn->res_num = str2intA(line, &idx);
    conn->res_str = skip_spaceA(line + idx);
    return (TRUE);
}

/*
---------------------------------------
    接收 HTTP 回调处理
---------------------------------------
*/
static bool_t
chttp_do_recv (
  __CR_IN__ sHTTPLIB*   conn,
  __CR_IN__ uint_t      size
    )
{
    uint_t  idx;

    /* 分块接收 */
    idx = size / conn->cb_size;
    for (; idx != 0; idx--) {
        if (socket_tcp_recv(conn->netw, conn->cb_buff,
                conn->cb_size) != conn->cb_size)
            return (FALSE);
        conn->cb_func(conn->cb_parm, conn->cb_buff,
                      conn->cb_size, conn->res_len);
    }

    /* 处理剩余 */
    idx = size % conn->cb_size;
    if (idx != 0) {
        if (socket_tcp_recv(conn->netw, conn->cb_buff, idx) != idx)
            return (FALSE);
        conn->cb_func(conn->cb_parm, conn->cb_buff, idx, conn->res_len);
    }
    return (TRUE);
}

/*
=======================================
    发送 HTTP 请求 (二进制)
=======================================
*/
CR_API bool_t
chttp_req_bin (
  __CR_IN__ chttp_t         conn,
  __CR_IN__ uint_t          method,
  __CR_IN__ const ansi_t*   path,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    uint_t      leng;
    uint_t      back;
    void_t*     temp;
    ansi_t*     head;
    iDATOT*     buff;
    sHTTPLIB*   real;

    /* 参数过滤 */
    if (method > HTTPLIB_DELETE)
        return (FALSE);
    real = (sHTTPLIB*)conn;
    if (path == NULL)
        path = real->path;

    /* 检查用户头是否封闭 */
    leng = (uint_t)dato_get_size(real->head);
    if (leng == 0) {
        chttp_head_fin(conn);
    }
    else {
        head = CR_VCALL(real->head)->flush(real->head);
        if (head[leng - 1] != 0x00)
            chttp_head_fin(conn);
    }

    /* 合成命令头 */
    head = CR_VCALL(real->head)->flush(real->head);
    if (data == NULL) {
        head = str_fmtA("%s %s %s\r\nHost: %s\r\n%s\r\n", s_http_cmd[method],
                        path, s_http_ver[real->vers], real->host, head);
    }
    else {
        head = str_fmtA("%s %s %s\r\nHost: %s\r\n%s"
                        "Content-Length: %u\r\n\r\n", s_http_cmd[method],
                        path, s_http_ver[real->vers], real->host, head, size);
    }
    if (head == NULL)
        return (FALSE);

    /* 发送 Request */
    if (real->netw == NULL) {
        real->netw = client_tcp_open(real->host, (int16u)real->port,
                                     real->tout);
        if (real->netw == NULL) {
            mem_free(head);
            return (FALSE);
        }
        socket_set_timeout(real->netw, real->wout, real->rout);
    }
    leng = (uint_t)str_lenA(head);
    if (socket_tcp_send(real->netw, head, leng) != leng) {
        mem_free(head);
        goto _failure1;
    }
    if (data != NULL && size != 0) {
        if (socket_tcp_send(real->netw, data, size) != size) {
            mem_free(head);
            goto _failure1;
        }
    }

    /* 接收 Response */
    buff = create_buff_out(128);
    if (buff == NULL) {
        mem_free(head);
        goto _failure1;
    }
    back = leng + 1;
    for (;;)
    {
        /* 一次尽可能多的读入数据 */
        if (!socket_input_size2(real->netw, &leng)) {
            mem_free(head);
            goto _failure2;
        }
        if (leng > back) {
            mem_free(head);
            head = str_allocA(leng);
            if (head == NULL)
                goto _failure2;
            back = leng;
        }
        socket_tcp_recv(real->netw, head, leng);
        if (CR_VCALL(buff)->write(buff, head, leng) != leng) {
            mem_free(head);
            goto _failure2;
        }

        /* 查找是否有两个 "\r\n" */
        leng = (uint_t)dato_get_size(buff);
        if (leng <= 4 + 8)
            continue;
        temp = CR_VCALL(buff)->flush(buff);
        size = chttp_find_head_end(temp, leng);

        /* 找到了直接出去, size + 4 = 头的大小 */
        if (size != leng) {
            size += 4;
            mem_free(head);
            break;
        }
    }

    /* 复制头数据 */
    real->res_len = 0;
    SAFE_FREE(real->res_dat);
    SAFE_FREE(real->res_hdr);
    real->res_ver = 0;
    real->res_num = 0;
    real->res_str = "";
    if (real->res_ini != NULL) {
        ini_closeU(real->res_ini);
        real->res_ini = NULL;
    }
    real->res_hdr = str_allocA(size + 1);
    if (real->res_hdr == NULL)
        goto _failure2;
    temp = CR_VCALL(buff)->flush(buff);
    mem_cpy(real->res_hdr, temp, size);
    real->res_hdr[size] = 0x00;

    /* 查找本体数据长度 */
    head = str_strIA(real->res_hdr, "Content-Length");
    if (head != NULL) {
        head += str_lenA("Content-Length");
        head = str_chrA(head, CR_AC(':'));
        if (head != NULL)
            real->res_len = str2intA(head + 1, NULL);
    }

    /* HEAD 方法直接返回 */
    if (method != HTTPLIB_HEAD)
    {
        /* 安全检查 */
        if (real->res_len > real->res_max)
            goto _failure3;

        /* 读取剩余的主体数据 */
        if (real->res_len != 0)
        {
            /* 获取之前在收头的时候收到的数据 */
            leng = (uint_t)dato_get_size(buff) - size;
            if (real->res_len < leng)
                goto _failure3;

            /* 接收本体数据 */
            if (real->cb_func == NULL)
            {
                /* 没有回调直接接收在内存里 */
                real->res_dat = mem_malloc(real->res_len + 1);
                if (real->res_dat == NULL)
                    goto _failure3;
                if (leng != 0)
                    mem_cpy(real->res_dat, (byte_t*)temp + size, leng);
                size = real->res_len - leng;
                if (size != 0) {
                    temp = (byte_t*)real->res_dat + leng;
                    if (socket_tcp_recv(real->netw, temp, size) != size)
                        goto _failure4;
                }
                ((byte_t*)(real->res_dat))[real->res_len] = 0x00;
            }
            else
            {
                /* 使用接收回调处理 */
                if (leng != 0) {
                    real->cb_func(real->cb_parm, (byte_t*)temp + size,
                                  leng, real->res_len);
                }
                size = real->res_len - leng;
                if (size != 0) {
                    if (!chttp_do_recv(real, size))
                        goto _failure3;
                }
                real->res_len = 0;
            }
        }
    }
    CR_VCALL(buff)->release(buff);
    return (chttp_do_head_cmd(real));

_failure4:
    mem_free(real->res_dat);
    real->res_dat = NULL;
_failure3:
    mem_free(real->res_hdr);
    real->res_hdr = NULL;
    real->res_len = 0;
_failure2:
    CR_VCALL(buff)->release(buff);
_failure1:
    socket_close(real->netw);
    real->netw = NULL;
    return (FALSE);
}

/*
=======================================
    获取 HTTP 返回的头
=======================================
*/
CR_API ansi_t*
chttp_res_hdr (
  __CR_IN__ chttp_t conn
    )
{
    return ((ansi_t*)((sHTTPLIB*)conn)->res_hdr);
}

/*
=======================================
    获取 HTTP 返回的主体 (字符串)
=======================================
*/
CR_API ansi_t*
chttp_res_str (
  __CR_IN__ chttp_t conn
    )
{
    return ((ansi_t*)((sHTTPLIB*)conn)->res_dat);
}

/*
=======================================
    获取 HTTP 返回的主体 (二进制)
=======================================
*/
CR_API void_t*
chttp_res_bin (
  __CR_IN__ chttp_t conn,
  __CR_OT__ uint_t* size
    )
{
    if (size != NULL)
        *size = ((sHTTPLIB*)conn)->res_len;
    return ((void_t*)((sHTTPLIB*)conn)->res_dat);
}

/*
=======================================
    返回 HTTP 响应行内容
=======================================
*/
CR_API uint_t
chttp_response (
  __CR_IN__ chttp_t     conn,
  __CR_OT__ uint_t*     vers,
  __CR_OT__ ansi_t**    text
    )
{
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    if (vers != NULL)
        *vers = real->res_ver;
    if (text != NULL)
        *text = real->res_str;
    return (real->res_num);
}

/*
=======================================
    返回 HTTP 响应头行数
=======================================
*/
CR_API uint_t
chttp_hdr_num (
  __CR_IN__ chttp_t conn
    )
{
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    if (real->res_ini == NULL)
        return (0);
    return ((uint_t)real->res_ini->count);
}

/*
=======================================
    返回 HTTP 响应头列表
=======================================
*/
CR_API ansi_t**
chttp_hdr_lst (
  __CR_IN__ chttp_t conn
    )
{
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    if (real->res_ini == NULL)
        return (NULL);
    return ((ansi_t**)real->res_ini->lines);
}

/*
=======================================
    查找 HTTP 响应头指定键值
=======================================
*/
CR_API ansi_t*
chttp_hdr_find (
  __CR_IN__ chttp_t         conn,
  __CR_IN__ const ansi_t*   key
    )
{
    leng_t      idx;
    ansi_t*     val;
    sHTTPLIB*   real;

    real = (sHTTPLIB*)conn;
    if (real->res_ini == NULL)
        return (NULL);
    for (idx = 1; idx < real->res_ini->count; idx++) {
        val = chttp_hdr_value(real->res_ini->lines[idx], key);
        if (val != NULL)
            return (val);
    }
    return (NULL);
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
