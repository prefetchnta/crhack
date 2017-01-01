/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack LZHUF 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* LZHUF 常数表 */
static const byte_t _rom_ s_elen[64] =
{
    0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};

static const byte_t _rom_ s_ecode[64] =
{
    0x00, 0x20, 0x30, 0x40, 0x50, 0x58, 0x60, 0x68,
    0x70, 0x78, 0x80, 0x88, 0x90, 0x94, 0x98, 0x9C,
    0xA0, 0xA4, 0xA8, 0xAC, 0xB0, 0xB4, 0xB8, 0xBC,
    0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
    0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
    0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

static const byte_t _rom_ s_dlen[256] =
{
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};

static const byte_t _rom_ s_dcode[256] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
    0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
    0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
    0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
    0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
    0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23,
    0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
    0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B,
    0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

/* LZHUF 常数 */
#define LZHUF_N             4096
#define LZHUF_F             60
#define LZHUF_THRESHOLD     2
#define LZHUF_NIL           LZHUF_N
#define LZHUF_NCHAR         (256 - LZHUF_THRESHOLD + LZHUF_F)
#define LZHUF_T             (LZHUF_NCHAR * 2 - 1)
#define LZHUF_R             (LZHUF_T - 1)
#define LZHUF_MAXFREQ       0x8000
#define LZHUF_INIT(handle)  handle->getLen = 0; \
                            handle->getBuf = 0; \
                            handle->putLen = 0; \
                            handle->putBuf = 0; \
                            handle->i_ptr  = 0; \
                            handle->o_ptr  = 0; \
                            handle->s_len  = srclen; \
                            handle->d_len  = dstlen; \
                            handle->i_buf  = (uchar*)src; \
                            handle->o_buf  = (uchar*)dst;
/* LZHUF 内部结构 */
typedef struct
{
        /* LZSS 部分 */
        uint_t  matchPos;
        uint_t  matchLen;
        uint_t  mdad[LZHUF_N + 1];
        uint_t  lson[LZHUF_N + 1];
        uint_t  rson[LZHUF_N + 257];
        byte_t  textBuf[LZHUF_N + LZHUF_F - 1];

        /* 哈夫曼编码部分 */
        uint_t  getLen;
        uint_t  putLen;
        uint_t  getBuf;
        uint_t  putBuf;
        uint_t  sonh[LZHUF_T];
        uint_t  freq[LZHUF_T + 1];
        uint_t  prnt[LZHUF_T + LZHUF_NCHAR];

        /* 内存访问部分 */
        uchar*  i_buf;
        uchar*  o_buf;
        leng_t  i_ptr, s_len;
        leng_t  o_ptr, d_len;

} sLZHUF;

/*
---------------------------------------
    获取一位
---------------------------------------
*/
static uint_t
lzhuf_get_bit (
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  cha;

    while (handle->getLen <= 8)
    {
        if (handle->i_ptr < handle->s_len)
            cha = handle->i_buf[handle->i_ptr];
        else
            cha = 0;
        handle->i_ptr++;
        handle->getBuf |= cha << (8 - handle->getLen);
        handle->getLen += 8;
    }

    cha = handle->getBuf;
    handle->getLen--;
    handle->getBuf <<= 1;
    return ((int16s)cha < 0);
}

/*
---------------------------------------
    获取一个字节
---------------------------------------
*/
static uint_t
lzhuf_get_byte (
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  cha;

    while (handle->getLen <= 8)
    {
        if (handle->i_ptr < handle->s_len)
            cha = handle->i_buf[handle->i_ptr];
        else
            cha = 0;
        handle->i_ptr++;
        handle->getBuf |= cha << (8 - handle->getLen);
        handle->getLen += 8;
    }

    cha = handle->getBuf;
    handle->getLen  -= 8;
    handle->getBuf <<= 8;
    return ((cha >> 8) & 0xFF);
}

/*
---------------------------------------
    输出码值
---------------------------------------
*/
static void_t
lzhuf_put_code (
  __CR_IN__ uint_t  length,
  __CR_IN__ uint_t  code,
  __CR_IO__ sLZHUF* handle
    )
{
    handle->putBuf |= code >> handle->putLen;
    handle->putLen += length;

    if (handle->putLen >= 8) {
        if (handle->o_ptr < handle->d_len)
            handle->o_buf[handle->o_ptr] = (uchar)(handle->putBuf >> 8);
        handle->o_ptr++;

        handle->putLen -= 8;
        if (handle->putLen >= 8) {
            if (handle->o_ptr < handle->d_len)
                handle->o_buf[handle->o_ptr] = (uchar)handle->putBuf;
            handle->o_ptr++;

            handle->putLen -= 8;
            handle->putBuf  = code << (length - handle->putLen);
        }
        else {
            handle->putBuf <<= 8;
        }
    }
}

/*
---------------------------------------
    初始化 LZSS 树
---------------------------------------
*/
static void_t
lzhuf_init_tree (
  __CR_OT__ sLZHUF* handle
    )
{
    uint_t  idx;

    for (idx = 0; idx < LZHUF_N; idx++)
        handle->mdad[idx] = LZHUF_NIL;

    for (idx = LZHUF_N + 1; idx <= LZHUF_N + 256; idx++)
        handle->rson[idx] = LZHUF_NIL;
}

/*
---------------------------------------
    插入 LZSS 树节点
---------------------------------------
*/
static void_t
lzhuf_insert_node (
  __CR_IN__ uint_t  r,
  __CR_IO__ sLZHUF* handle
    )
{
    uchar*  key;
    uint_t  idx;
    uint_t  ppp;
    uint_t  tmp;
    sint_t  cmp;

    cmp = 1;
    key = &handle->textBuf[r];
    ppp = LZHUF_N + 1 + key[0];

    handle->matchLen = 0;
    handle->lson[r] = LZHUF_NIL;
    handle->rson[r] = LZHUF_NIL;

    for (;;)
    {
        if (cmp >= 0)
        {
            if (handle->rson[ppp] == LZHUF_NIL)
            {
                handle->mdad[r] = ppp;
                handle->rson[ppp] = r;
                return;
            }
            ppp = handle->rson[ppp];
        }
        else
        {
            if (handle->lson[ppp] == LZHUF_NIL)
            {
                handle->mdad[r] = ppp;
                handle->lson[ppp] = r;
                return;
            }
            ppp = handle->lson[ppp];
        }

        for (idx = 1; idx < LZHUF_F; idx++)
        {
            cmp = key[idx] - handle->textBuf[ppp + idx];
            if (cmp != 0)
                break;
        }

        if (idx > LZHUF_THRESHOLD)
        {
            if (idx > handle->matchLen)
            {
                handle->matchLen = idx;
                handle->matchPos = ((r - ppp) & (LZHUF_N - 1)) - 1;

                if (idx >= LZHUF_F)
                    break;
            }
            else
            if (idx == handle->matchLen)
            {
                tmp = ((r - ppp) & (LZHUF_N - 1)) - 1;

                if (tmp < handle->matchPos)
                    handle->matchPos = tmp;
            }
        }
    }
    handle->mdad[r] = handle->mdad[ppp];
    handle->lson[r] = handle->lson[ppp];
    handle->rson[r] = handle->rson[ppp];

    handle->mdad[handle->lson[ppp]] = r;
    handle->mdad[handle->rson[ppp]] = r;

    if (handle->rson[handle->mdad[ppp]] == ppp)
        handle->rson[handle->mdad[ppp]] = r;
    else
        handle->lson[handle->mdad[ppp]] = r;
    handle->mdad[ppp] = LZHUF_NIL;
}

/*
---------------------------------------
    删除 LZSS 树节点
---------------------------------------
*/
static void_t
lzhuf_delete_node (
  __CR_IN__ uint_t  p,
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  qq;

    if (handle->mdad[p] == LZHUF_NIL)
        return;

    if (handle->rson[p] == LZHUF_NIL)
    {
        qq = handle->lson[p];
    }
    else
    if (handle->lson[p] == LZHUF_NIL)
    {
        qq = handle->rson[p];
    }
    else
    {
        qq = handle->lson[p];
        if (handle->rson[qq] != LZHUF_NIL)
        {
            do
            {
                qq = handle->rson[qq];
            } while (handle->rson[qq] != LZHUF_NIL);

            handle->rson[handle->mdad[qq]] = handle->lson[qq];
            handle->mdad[handle->lson[qq]] = handle->mdad[qq];
            handle->lson[qq] = handle->lson[p];
            handle->mdad[handle->lson[p]] = qq;
        }
        handle->rson[qq] = handle->rson[p];
        handle->mdad[handle->rson[p]] = qq;
    }
    handle->mdad[qq] = handle->mdad[p];

    if (handle->rson[handle->mdad[p]] == p)
        handle->rson[handle->mdad[p]] = qq;
    else
        handle->lson[handle->mdad[p]] = qq;
    handle->mdad[p] = LZHUF_NIL;
}

/*
---------------------------------------
    开始哈夫曼编码
---------------------------------------
*/
static void_t
lzhuf_start_huff (
  __CR_OT__ sLZHUF* handle
    )
{
    uint_t  ii, jj;

    for (ii = 0; ii < LZHUF_NCHAR; ii++)
    {
        handle->freq[ii] = 1;
        handle->sonh[ii] = ii + LZHUF_T;
        handle->prnt[ii + LZHUF_T] = ii;
    }
    ii = 0;
    jj = LZHUF_NCHAR;

    while (jj <= LZHUF_R)
    {
        handle->sonh[jj] = ii;
        handle->freq[jj] = handle->freq[ii] + handle->freq[ii + 1];
        handle->prnt[ii] = handle->prnt[ii + 1] = jj;
        ii += 2;
        jj++;
    }
    handle->freq[LZHUF_T] = 0xFFFF;
    handle->prnt[LZHUF_R] = 0x0000;
}

/*
---------------------------------------
    重新计算频率
---------------------------------------
*/
static void_t
lzhuf_reconst (
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  ii, jj, kk;
    uint_t  ff, ll, tt;

    for (ii = 0, jj = 0; ii < LZHUF_T; ii++)
    {
        if (handle->sonh[ii] >= LZHUF_T)
        {
            handle->sonh[jj] =  handle->sonh[ii];
            handle->freq[jj] = (handle->freq[ii] + 1) / 2;
            jj++;
        }
    }

    for (ii = 0, jj = LZHUF_NCHAR; jj < LZHUF_T; ii += 2, jj++)
    {
        kk = ii + 1;
        ff = handle->freq[jj] = handle->freq[ii] + handle->freq[kk];

        for (kk = jj - 1; ff < handle->freq[kk]; kk--);

        kk++;
        ll = (jj - kk) * 2;

        for (tt = kk + ll - 1; tt >= kk; tt--)
        {
            handle->sonh[tt + 1] = handle->sonh[tt];
            handle->freq[tt + 1] = handle->freq[tt];
        }
        handle->sonh[kk] = ii;
        handle->freq[kk] = ff;
    }

    for (ii = 0; ii < LZHUF_T; ii++)
    {
        if ((kk = handle->sonh[ii]) >= LZHUF_T)
            handle->prnt[kk] = ii;
        else
            handle->prnt[kk] = handle->prnt[kk + 1] = ii;
    }
}

/*
---------------------------------------
    更新模型
---------------------------------------
*/
static void_t
lzhuf_update (
  __CR_IN__ uint_t  code,
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  ii, jj;
    uint_t  kk, ll;

    if (handle->freq[LZHUF_R] == LZHUF_MAXFREQ)
        lzhuf_reconst(handle);

    code = handle->prnt[code + LZHUF_T];

    do
    {
        kk = ++(handle->freq[code]);

        if (kk > handle->freq[ll = code + 1])
        {
            while (kk > handle->freq[++ll]);

            ll--;
            handle->freq[code] = handle->freq[ll];
            handle->freq[ll] = kk;

            ii = handle->sonh[code];

            handle->prnt[ii] = ll;
            if (ii < LZHUF_T)
                handle->prnt[ii + 1] = ll;

            jj = handle->sonh[ll];
            handle->sonh[ll] = ii;

            handle->prnt[jj] = code;
            if (jj < LZHUF_T)
                handle->prnt[jj + 1] = code;

            handle->sonh[code] = jj;
            code = ll;
        }
    } while ((code = handle->prnt[code]) != 0);
}

/*
---------------------------------------
    编码一个字符
---------------------------------------
*/
static void_t
lzhuf_encode_char (
  __CR_IN__ uint_t  code,
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  ii = 0;
    uint_t  jj = 0;
    uint_t  kk = handle->prnt[code + LZHUF_T];

    do
    {
        ii >>= 1;
        if ((kk & 1) != 0)
            ii += 0x8000;
        jj++;
    } while ((kk = handle->prnt[kk]) != LZHUF_R);

    lzhuf_put_code(jj, ii, handle);
    lzhuf_update(code, handle);
}

/*
---------------------------------------
    编码一个位置
---------------------------------------
*/
static void_t
lzhuf_encode_position (
  __CR_IN__ uint_t  code,
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  idx = code >> 6;

    lzhuf_put_code(s_elen[idx], (uint_t)s_ecode[idx] << 8, handle);
    lzhuf_put_code(6, (code & 0x3F) << 10, handle);
}

/*
---------------------------------------
    编码结束
---------------------------------------
*/
static void_t
lzhuf_encode_end (
  __CR_IO__ sLZHUF* handle
    )
{
    if (handle->putLen && handle->o_ptr < handle->d_len)
        handle->o_buf[handle->o_ptr] = (uchar)(handle->putBuf >> 8);
    handle->o_ptr++;
}

/*
---------------------------------------
    解码一个字符
---------------------------------------
*/
static uint_t
lzhuf_decode_char (
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  cha;

    cha = handle->sonh[LZHUF_R];
    while (cha < LZHUF_T)
    {
        cha += lzhuf_get_bit(handle);
        cha  = handle->sonh[cha];
    }

    cha -= LZHUF_T;
    lzhuf_update(cha, handle);
    return (cha);
}

/*
---------------------------------------
    解码一个位置
---------------------------------------
*/
static uint_t
lzhuf_decode_position (
  __CR_IO__ sLZHUF* handle
    )
{
    uint_t  ii = lzhuf_get_byte(handle);
    uint_t  jj = s_dlen[ii] - 2;
    uint_t  cc = (uint_t)s_dcode[ii] << 6;

    while (jj-- != 0)
        ii = (ii << 1) + lzhuf_get_bit(handle);
    return (cc | (ii & 0x3F));
}

/*
=======================================
    LZHUF 压缩
=======================================
*/
CR_API leng_t
compr_lzhuf (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          fill
    )
{
    byte_t  cha;
    uint_t  rrr, sss;
    uint_t  idx, len;
    uint_t  last_match_length;
    sLZHUF* handle;

    if (dst == NULL)
        return (srclen + srclen / 8 + 512);

    handle = struct_new(sLZHUF);
    if (handle == NULL)
        return (0);
    LZHUF_INIT(handle)
    lzhuf_init_tree(handle);
    lzhuf_start_huff(handle);

    sss = 0;
    rrr = LZHUF_N - LZHUF_F;
    for (idx = 0; idx < rrr; idx++)
        handle->textBuf[idx] = (uchar)fill;

    for (len = 0; len < LZHUF_F && handle->i_ptr < srclen; len++)
        handle->textBuf[rrr + len] = handle->i_buf[handle->i_ptr++];

    for (idx = 1; idx <= LZHUF_F; idx++)
        lzhuf_insert_node(rrr - idx, handle);
    lzhuf_insert_node(rrr, handle);

    do
    {
        if (handle->matchLen > len)
            handle->matchLen = len;

        if (handle->matchLen <= LZHUF_THRESHOLD)
        {
            handle->matchLen = 1;
            lzhuf_encode_char(handle->textBuf[rrr], handle);
        }
        else
        {
            lzhuf_encode_char(255 - LZHUF_THRESHOLD +
                              handle->matchLen, handle);
            lzhuf_encode_position(handle->matchPos, handle);
        }
        last_match_length = handle->matchLen;

        for (idx = 0; idx < last_match_length && handle->i_ptr < srclen; idx++)
        {
            cha = handle->i_buf[handle->i_ptr++];

            lzhuf_delete_node(sss, handle);

            handle->textBuf[sss] = cha;
            if (sss < LZHUF_F - 1)
                handle->textBuf[sss + LZHUF_N] = cha;

            sss = (sss + 1) & (LZHUF_N - 1);
            rrr = (rrr + 1) & (LZHUF_N - 1);
            lzhuf_insert_node(rrr, handle);
        }

        while (idx++ < last_match_length)
        {
            lzhuf_delete_node(sss, handle);
            sss = (sss + 1) & (LZHUF_N - 1);
            rrr = (rrr + 1) & (LZHUF_N - 1);

            if (--len != 0)
                lzhuf_insert_node(rrr, handle);
        }
    } while (len > 0);

    lzhuf_encode_end(handle);

    if (handle->o_ptr > dstlen) {
        mem_free(handle);
        return (0);
    }
    dstlen = handle->o_ptr;
    mem_free(handle);
    return (dstlen);
}

/*
=======================================
    LZHUF 解压
=======================================
*/
CR_API leng_t
uncompr_lzhuf (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          fill
    )
{
    uint_t  ii, jj;
    uint_t  kk, rr, cha;
    sLZHUF* handle;

    handle = struct_new(sLZHUF);
    if (handle == NULL)
        return (0);
    LZHUF_INIT(handle)
    lzhuf_start_huff(handle);

    rr = LZHUF_N - LZHUF_F;
    for (ii = 0; ii < rr; ii++)
        handle->textBuf[ii] = (uchar)fill;

    while (handle->o_ptr < dstlen)
    {
        cha = lzhuf_decode_char(handle);
        if (cha < 256)
        {
            if (handle->o_ptr >= dstlen)
                goto _failure;
            handle->textBuf[rr++]          = (uchar)cha;
            handle->o_buf[handle->o_ptr++] = (uchar)cha;

            rr &= (LZHUF_N - 1);
        }
        else
        {
            jj = cha - 255 + LZHUF_THRESHOLD;
            ii = (rr - lzhuf_decode_position(handle) - 1) & (LZHUF_N - 1);

            for (kk = 0; kk < jj; kk++)
            {
                if (handle->o_ptr >= dstlen)
                    goto _failure;
                cha = handle->textBuf[(ii + kk) & (LZHUF_N - 1)];

                handle->textBuf[rr++]          = (uchar)cha;
                handle->o_buf[handle->o_ptr++] = (uchar)cha;

                rr &= (LZHUF_N - 1);
            }
        }
    }
    dstlen = handle->o_ptr;
    mem_free(handle);
    return (dstlen);

_failure:
    mem_free(handle);
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
