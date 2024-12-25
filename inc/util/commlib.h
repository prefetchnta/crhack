/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-10-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_COMMLIB_H__
#define __CR_COMMLIB_H__

#include "../devlib.h"

/* 有用的函数回调类型 */
typedef void_t  (*comm_delay_t) (void_t);
typedef void_t  (*comm_crypto_t) (void_t*, leng_t);

/*****************************************************************************/
/*                                 通用协议                                  */
/*****************************************************************************/

/*
    基本协议格式 (BE)
    ~~~~~~~~~~~~~~~~~
    55 AA 长度(2B) 数据(0-65529) 校验(2B)
    长度只表示数据的长度, 不包括头和校验
    数据校验方式和加解密方法可自选或没有
*/
typedef struct
{
        /* 附加参数 */
        void_t*     ctx_param;

        /* 校验回调 */
        int16u  (*hasher) (const void_t*, leng_t);

        /* 加密回调 */
        void_t  (*encode) (void_t*, void_t*, leng_t);

        /* 解密回调 */
        void_t  (*decode) (void_t*, void_t*, leng_t);

} sCOMM_CTX;

CR_API bool_t   comm_sio_send (uint_t port, const sCOMM_CTX *parm,
                               const void_t *data, int16u size);
CR_API void_t*  comm_sio_recv (uint_t port, const sCOMM_CTX *parm,
                               uint_t *size);
CR_API void_t*  comm_sio_iorw (uint_t port, const sCOMM_CTX *parm,
                               uint_t *osize, const void_t *ibuff,
                               int16u isize, comm_delay_t delay);
CR_API bool_t   comm_tcp_send (socket_t netw, const sCOMM_CTX *parm,
                               const void_t *data, int16u size);
CR_API void_t*  comm_tcp_recv (socket_t netw, const sCOMM_CTX *parm,
                               uint_t *size);
CR_API void_t*  comm_tcp_iorw (socket_t netw, const sCOMM_CTX *parm,
                               uint_t *osize, const void_t *ibuff,
                               int16u isize, comm_delay_t delay);
CR_API bool_t   comm_udp_send (socket_t netw, const sCOMM_CTX *parm,
                               const ansi_t *addr, int16u port,
                               const void_t *data, int16u size);
CR_API void_t*  comm_udp_recv (socket_t netw, const sCOMM_CTX *parm,
                               uint_t *size);
CR_API void_t*  comm_udp_iorw (socket_t netw, const sCOMM_CTX *parm,
                               const ansi_t *addr, int16u port,
                               uint_t *osize, const void_t *ibuff,
                               int16u isize, comm_delay_t delay);

/*****************************************************************************/
/*                             DTM_NEW 通讯协议                              */
/*****************************************************************************/

/* 接收缓冲区需要多放2个字节的校验 */
CR_API bool_t   dtmn_sio_send (uint_t port, byte_t cmd_type1,
                               byte_t cmd_type2, byte_t frame_num,
                               int16u additions, const void_t *data,
                               int16u size);
CR_API bool_t   dtmn_sio_recv (uint_t port, void_t *data, uint_t *size,
                               byte_t *cmd_type1, byte_t *cmd_type2,
                               byte_t *frame_num, int16u *additions);
CR_API bool_t   dtmn_sio_iorw (uint_t port, byte_t cmd_type1,
                               byte_t cmd_type2, void_t *obuff,
                               uint_t *osize, int16u *retcode,
                               byte_t frame_num, int16u additions,
                               const void_t *ibuff, int16u isize,
                               comm_delay_t delay);

/*****************************************************************************/
/*                             DTM_OLD 通讯协议                              */
/*****************************************************************************/

/* 接收缓冲区需要多放1个字节的校验 */
CR_API bool_t   dtmo_sio_send (uint_t port, const void_t *data,
                               int16u size);
CR_API bool_t   dtmo_sio_recv (uint_t port, void_t *data, uint_t *size);
CR_API bool_t   dtmo_sio_iorw (uint_t port, void_t *obuff, uint_t *osize,
                               const void_t *ibuff, int16u isize,
                               comm_delay_t delay);

/*****************************************************************************/
/*                               FDU 通讯协议                                */
/*****************************************************************************/

/* FDU 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* FDU 文件信息项结构 */
CR_TYPEDEF struct
{
/*000*/ ansi_t  name[8];    /* 文件文件名 */
/*008*/ ansi_t  exts[3];    /* 文件扩展名 */
/*011*/ int32u  size;       /* 文件大小LE */
/*015*/
} CR_PACKED sFDU_ITM;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

CR_API void_t*  fdu_sio_rfile6 (uint_t port, const ansi_t *name,
                                leng_t *size, uint_t try_times,
                                comm_delay_t delay1, comm_delay_t delay2);
CR_API void_t*  fdu_sio_rfile8 (uint_t port, const ansi_t *name,
                                leng_t *size, uint_t try_times,
                                comm_delay_t delay1, comm_delay_t delay2);
CR_API bool_t   fdu_sio_wfile6 (uint_t port, const ansi_t *name,
                                const void_t *data, leng_t size,
                                uint_t try_times, comm_delay_t delay1,
                                comm_delay_t delay2);
CR_API bool_t   fdu_sio_wfile8 (uint_t port, const ansi_t *name,
                                const void_t *data, leng_t size,
                                uint_t try_times, comm_delay_t delay1,
                                comm_delay_t delay2);
CR_API sFDU_ITM*  fdu_sio_list (uint_t port, leng_t *count,
                                uint_t try_times, comm_delay_t delay1,
                                comm_delay_t delay2);

/*****************************************************************************/
/*                            ISO14443 通讯协议                              */
/*****************************************************************************/

/* 设置状态字 */
#define ISO14443_BELL   0x01    /* 蜂鸣器鸣叫 */
#define ISO14443_LED    0x02    /* LED 灯亮起 */
#define ISO14443_OKAY   0x10    /* 成功时亮并叫 */
#define ISO14443_AUTO   0x20    /* 自动检测卡 */

/* 密钥类型选择 */
#define ISO14443_KEYA   0x00    /* 使用 KeyA */
#define ISO14443_KEYB   0x80    /* 使用 KeyB */

CR_API ansi_t*  iso14443_sio_get_ver (uint_t port, byte_t addr);
CR_API bool_t   iso14443_sio_setup (uint_t port, byte_t addr, byte_t param);
CR_API bool_t   iso14443_sio_online (uint_t port, byte_t addr);
CR_API bool_t   iso14443_sio_set_addr (uint_t port, byte_t addr, byte_t naddr);
CR_API bool_t   iso14443_sio_rd_blk (uint_t port, byte_t addr, byte_t keyab,
                                  byte_t nkey, byte_t nblock, byte_t data[16]);
CR_API bool_t   iso14443_sio_wr_blk (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, const byte_t data[16]);
CR_API bool_t   iso14443_sio_stop (uint_t port, byte_t addr);
CR_API bool_t   iso14443_sio_get_uid (uint_t port, byte_t addr, byte_t uid[5]);
CR_API bool_t   iso14443_sio_set_eep (uint_t port, byte_t addr, byte_t nkey,
                                      const byte_t data[6]);
CR_API bool_t   iso14443_sio_set_key (uint_t port, byte_t addr, byte_t keyab,
                            byte_t nkey, byte_t nblock, const byte_t data[16]);
CR_API bool_t   iso14443_sio_vsub (uint_t port, byte_t addr, byte_t keyab,
                             byte_t nkey, byte_t nblock, const byte_t data[4]);
CR_API bool_t   iso14443_sio_vadd (uint_t port, byte_t addr, byte_t keyab,
                             byte_t nkey, byte_t nblock, const byte_t data[4]);
CR_API bool_t   iso14443_sio_rd_blks (uint_t port, byte_t addr, byte_t start,
                                      void_t *data, uint_t size);
CR_API bool_t   iso14443_sio_wr_blks (uint_t port, byte_t addr, byte_t start,
                                      const void_t *data, uint_t size);

/*****************************************************************************/
/*                              LLB3 通讯协议                                */
/*****************************************************************************/

CR_API fp32_t   llb3_sio_get_tc (uint_t port, byte_t addr); /* 获取温度 */
CR_API fp32_t   llb3_sio_get_ta (uint_t port, byte_t addr); /* 获取常温 */
CR_API fp32_t   llb3_sio_get_pc (uint_t port, byte_t addr); /* 获取压力 */
CR_API fp32_t   llb3_sio_get_dc (uint_t port, byte_t addr); /* 获取差压 */
CR_API fp32_t   llb3_sio_get_fc (uint_t port, byte_t addr); /* 获取流量 */
CR_API fp32_t   llb3_sio_get_hc (uint_t port, byte_t addr); /* 获取热量 */
CR_API fp32_t   llb3_sio_get_sq (uint_t port, byte_t addr); /* 累计流量 */
CR_API fp32_t   llb3_sio_get_sh (uint_t port, byte_t addr); /* 累计热量 */
CR_API int16u   llb3_sio_get_ad (uint_t port, byte_t addr,  /* 直接取值 */
                                 byte_t chnn, int16u *maxv);

/*****************************************************************************/
/*                              RFCMM 通讯协议                               */
/*****************************************************************************/

/* RFCMM 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 区域屏遥控显示数值 (BE) */
CR_TYPEDEF struct
{
/*000*/ int16u  num_time;   /* 遥控显示时间 (秒)
                               0xFFFF 表示一直遥控显示 */
/*002*/ byte_t  arr_type;   /* 箭头方向颜色控制 */
/*003*/ byte_t  arr_time;   /* 箭头闪烁时间 (秒)
                               0xFF 表示一直闪烁 */
/*004*/ int32u  number;     /* 显示的数值 */
/*008*/ byte_t  flash[4];   /* 对应四个位的闪烁时间 */
/*012*/
} CR_PACKED sRFCMM_QYP_NUM;

/* 超声波状态结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  retcode;    /* 返回值 =0x00 */
/*001*/ byte_t  line1[32];  /* 第一路超声波状态 */
/*033*/ byte_t  line2[32];  /* 第二路超声波状态 */
/*065*/ byte_t  line3[32];  /* 第三路超声波状态 */
/*097*/
} CR_PACKED sRFCMM_YDP_CSB_STT;

/* 超声波车位状态结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  retcode;    /* 返回值 =0x00 */
/*001*/ byte_t  send_id;    /* 发送标识 =0x01 */
/*002*/ byte_t  line1[32];  /* 第一路超声波状态 */
/*034*/ byte_t  line2[32];  /* 第二路超声波状态 */
/*066*/ byte_t  line3[32];  /* 第三路超声波状态 */
/*098*/
} CR_PACKED sRFCMM_YDP_CSB_POS;

/* 超声波主机查询结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x01 */
/*002*/ byte_t  nouse;  /* 保留不用 */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_01;

/* 超声波查询序列号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x02 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_02;

/* 超声波设置序列号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x03 */
/*002*/ byte_t  sn[10]; /* 序列号 */
/*012*/ byte_t  sum8b;  /* 8位累加和 */
/*013*/
} CR_PACKED sRFCMM_CSB_03;

/* 超声波查询设备编号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x04 */
/*002*/ byte_t  sn[10]; /* 序列号 */
/*012*/ byte_t  sum8b;  /* 8位累加和 */
/*013*/
} CR_PACKED sRFCMM_CSB_04;

/* 超声波设置设备编号结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x05 */
/*002*/ byte_t  sn[10]; /* 序列号 */
/*012*/ byte_t  ndevid; /* 新设备编号 */
/*013*/ byte_t  sum8b;  /* 8位累加和 */
/*014*/
} CR_PACKED sRFCMM_CSB_05;

/* 超声波控制车位灯结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x06 */
/*002*/ byte_t  cntrl;  /* 灯状态控制字 */
/*003*/ byte_t  lastt;  /* 持续时间 (单位为30秒
                            0xFF 表示一直保持) */
/*004*/ byte_t  sum8b;  /* 8位累加和 */
/*005*/
} CR_PACKED sRFCMM_CSB_06;

/* 超声波查询去抖次数结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x07 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_07;

/* 超声波设置去抖次数结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x08 */
/*002*/ byte_t  times;  /* 去抖次数 */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_08;

/* 超声波查询基准距离结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x09 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_09;

/* 超声波设置基准距离结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0A */
/*002*/ byte_t  dists;  /* 基准距离 (单位0.1米) */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_0A;

/* 超声波查询预留车位结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0B */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_0B;

/* 超声波设置预留车位结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0C */
/*002*/ byte_t  value;  /* 预留车位值 */
/*003*/ byte_t  sum8b;  /* 8位累加和 */
/*004*/
} CR_PACKED sRFCMM_CSB_0C;

/* 超声波查询探测距离结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x0D */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_0D;

/* 超声波查询固件版本结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  devid;  /* 设备编号 */
/*001*/ byte_t  cmdid;  /* 指令编号 =0x90 */
/*002*/ byte_t  sum8b;  /* 8位累加和 */
/*003*/
} CR_PACKED sRFCMM_CSB_90;

/* 区域屏干接点计数值 (BE) */
CR_TYPEDEF struct
{
/*000*/ int32u  gan_cnt1;   /* 1号干接点计数值 */
/*004*/ int32u  gan_cnt2;   /* 2号干接点计数值 */
/*008*/ int32u  gan_cnt3;   /* 3号干接点计数值 */
/*012*/ int32u  gan_cnt4;   /* 4号干接点计数值 */
/*016*/
} CR_PACKED sRFCMM_QYP_GAN;

/* 引导屏管理信息结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  mgr_id;     /* 管理的引导屏序号 */
/*001*/ int16u  ydp_id;     /* 引导屏的编号 */
/*003*/ int16u  udp_port;   /* 网络通讯的端口 (6500) */
/*005*/ byte_t  ipv4[4];    /* 引导屏的 IP 地址 */
/*009*/ int32u  csb_mgr1;   /* 第一路超声波管理标志 */
/*013*/ int32u  csb_mgr2;   /* 第二路超声波管理标志 */
/*017*/ int32u  csb_mgr3;   /* 第三路超声波管理标志 */
/*021*/
} CR_PACKED sRFCMM_QYP_MGR;
/*
0xF0000000: 第1路编号1~4的超声波探测器纳入管理
0xF000000F: 第2路编号1~4,29~32的超声波探测器纳入管理
0xFFFFFFFF: 第3路所有超声波探测器纳入管理
*/

/* 载波通讯效率结构 (BE) */
CR_TYPEDEF struct
{
/*000*/ byte_t  retcode;    /* 返回值 =0x00 */
/*001*/ int32u  cnt_succ;   /* 接收成功次数 */
/*005*/ int32u  cnt_fail;   /* 接收错误次数 */
/*009*/ int32u  cnt_nack;   /* 无应答的次数 */
/*013*/ byte_t  rat_succ;   /* 最近120次中接收成功次数 */
/*014*/ byte_t  rat_fail;   /* 最近120次中接收错误次数 */
/*015*/ byte_t  rat_nack;   /* 最近120次中无应答的次数 */
/*016*/
} CR_PACKED sRFCMM_YDP_COUNT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* 设备类型取值 */
#define RFCMM_YDP   'g'     /* 引导屏 */
#define RFCMM_QYP   'b'     /* 区域屏 */
#define RFCMM_ATX   'P'     /* 上位机 */

/* 区域屏可管理引导屏的最大个数 */
#define RFCMM_QYP_MAX_YDP   80

/* 箭头方向颜色 */
#define RFCMM_ARR_RR    0x12    /* 红色向右 */
#define RFCMM_ARR_RL    0x0A    /* 红色向左 */
#define RFCMM_ARR_RU    0x05    /* 红色向上 */
#define RFCMM_ARR_GR    0x92    /* 绿色向右 */
#define RFCMM_ARR_GL    0x8A    /* 绿色向左 */
#define RFCMM_ARR_GU    0x85    /* 绿色向上 */

/* 超声波状态值 */
#define RFCMM_YDP_CSB_STT_PWRON     0x00    /* 上电状态 */
#define RFCMM_YDP_CSB_STT_XMISS     0x01    /* 不存在状态 */
#define RFCMM_YDP_CSB_STT_ONLINE    0x02    /* 在线状态 */
#define RFCMM_YDP_CSB_STT_OFFLINE   0x03    /* 断线状态 */

/* 超声波车位状态值 */
#define RFCMM_YDP_CSB_POS_PWRON     0xE0    /* 上电状态 */
#define RFCMM_YDP_CSB_POS_NOCAR     0xEA    /* 无车状态 */
#define RFCMM_YDP_CSB_POS_HVCAR     0xEF    /* 有车状态 */
#define RFCMM_YDP_CSB_POS_OFFLINE   0xE2    /* 车位断线 */
#define RFCMM_YDP_CSB_POS_XMISS     0xE7    /* 车位不存在 */

/* 超声波灯控制值 */
#define RFCMM_CSB_G_OFF     (0 << 0)    /* 绿灯灭 */
#define RFCMM_CSB_G_FLASH   (1 << 0)    /* 绿灯闪 */
#define RFCMM_CSB_G_XON     (3 << 0)    /* 绿灯亮 */
#define RFCMM_CSB_R_OFF     (0 << 2)    /* 红灯灭 */
#define RFCMM_CSB_R_FLASH   (1 << 2)    /* 红灯闪 */
#define RFCMM_CSB_R_XON     (3 << 2)    /* 红灯亮 */
#define RFCMM_CSB_L_AUTO    (0 << 6)    /* 自  控 */
#define RFCMM_CSB_L_HALF    (1 << 6)    /* 半自动 */
#define RFCMM_CSB_L_MANS    (3 << 6)    /* 遥  控 */

/* 超声波车位状态值 */
#define RFCMM_CSB_POS_HVCAR 0x0F    /* 有车 */
#define RFCMM_CSB_POS_NOCAR 0x0A    /* 无车 */
#define RFCMM_CSB_POS_XMISS 0x00    /* 未知 */
#define RFCMM_CSB_POS_RESVE 0x05    /* 预留 */

/*===== 显示屏接口 =====*/
CR_API bool_t   rfcmm_udp_all_online (socket_t netw, byte_t stype,
                                      byte_t dtype);
CR_API bool_t   rfcmm_udp_all_reset (socket_t netw, byte_t stype,
                                     byte_t dtype);
CR_API bool_t   rfcmm_udp_all_devid (socket_t netw, byte_t stype,
                                     byte_t dtype, uint_t *devid);
CR_API ansi_t*  rfcmm_udp_all_serial (socket_t netw, byte_t stype,
                                      byte_t dtype);
CR_API bool_t   rfcmm_udp_qyp_show (socket_t netw, byte_t stype,
                                    const sRFCMM_QYP_NUM *param);
CR_API bool_t   rfcmm_udp_ydp_csb_stt (socket_t netw, byte_t stype,
                                       sRFCMM_YDP_CSB_STT *state);
CR_API bool_t   rfcmm_udp_ydp_csb_pos (socket_t netw, byte_t stype,
                                       sRFCMM_YDP_CSB_POS *state);
CR_API bool_t   rfcmm_udp_all_set_arrow (socket_t netw, byte_t stype,
                                         byte_t dtype, byte_t *adirs,
                                         byte_t isize);
CR_API bool_t   rfcmm_udp_all_get_arrow (socket_t netw, byte_t stype,
                                         byte_t dtype, byte_t *adirs,
                                         uint_t *osize);
CR_API bool_t   rfcmm_udp_all_set_light (socket_t netw, byte_t stype,
                                         byte_t dtype, byte_t level);
CR_API bool_t   rfcmm_udp_all_set_color (socket_t netw, byte_t stype,
                                         byte_t dtype, byte_t color);
CR_API bool_t   rfcmm_udp_qyp_free (socket_t netw, byte_t stype,
                                    uint_t *free);
CR_API bool_t   rfcmm_udp_qyp_set_gan (socket_t netw, byte_t stype,
                                       const sRFCMM_QYP_GAN *param);
CR_API bool_t   rfcmm_udp_qyp_get_gan (socket_t netw, byte_t stype,
                                       sRFCMM_QYP_GAN *param);
CR_API bool_t   rfcmm_udp_qyp_set_ydp (socket_t netw, byte_t stype,
                                       const sRFCMM_QYP_MGR *param);
CR_API bool_t   rfcmm_udp_qyp_get_ydp (socket_t netw, byte_t stype,
                                       sRFCMM_QYP_MGR *param,
                                       byte_t mgr_id);
CR_API bool_t   rfcmm_udp_qyp_del_ydp (socket_t netw, byte_t stype,
                                       byte_t mgr_id);
CR_API bool_t   rfcmm_udp_qyp_del_all (socket_t netw, byte_t stype);
CR_API bool_t   rfcmm_udp_ydp_csb_cnt (socket_t netw, byte_t stype,
                                       sRFCMM_YDP_COUNT *count);
/*===== 超声波接口 =====*/
CR_API bool_t   rfcmm_udp_csb_query (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *state, const sRFCMM_CSB_01 *param);
CR_API bool_t   rfcmm_udp_csb_getsn (socket_t netw, byte_t stype, byte_t line,
                                   byte_t sn[10], const sRFCMM_CSB_02 *param);
CR_API bool_t   rfcmm_udp_csb_setsn (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_03 *param);
CR_API bool_t   rfcmm_udp_csb_getid (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *devid, const sRFCMM_CSB_04 *param);
CR_API bool_t   rfcmm_udp_csb_setid (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_05 *param);
CR_API bool_t   rfcmm_udp_csb_light (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_06 *param);
CR_API bool_t   rfcmm_udp_csb_getft (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *times, const sRFCMM_CSB_07 *param);
CR_API bool_t   rfcmm_udp_csb_setft (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_08 *param);
CR_API bool_t   rfcmm_udp_csb_getds (socket_t netw, byte_t stype, byte_t line,
                                   byte_t *dists, const sRFCMM_CSB_09 *param);
CR_API bool_t   rfcmm_udp_csb_setds (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_0A *param);
CR_API bool_t   rfcmm_udp_csb_getcp (socket_t netw, byte_t stype, byte_t line,
                                   byte_t val[2], const sRFCMM_CSB_0B *param);
CR_API bool_t   rfcmm_udp_csb_setcp (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_0C *param);
CR_API bool_t   rfcmm_udp_csb_lastd (socket_t netw, byte_t stype, byte_t line,
                                   int32u *dists, const sRFCMM_CSB_0D *param);
CR_API ansi_t*  rfcmm_udp_csb_fwver (socket_t netw, byte_t stype, byte_t line,
                                     const sRFCMM_CSB_90 *param);

/*****************************************************************************/
/*                              RFCON 通讯协议                               */
/*****************************************************************************/

/* RFCON 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* RFCON 输入信息结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  inp[4];     /* 输入电平 */
/*004*/ byte_t  ups_sta;    /* 市电 < 80V = 0
                               市电 > 80V = 1 */
/*005*/
} CR_PACKED sRFCON_INP;

/* RFCON 输出单元结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  id;     /* 端口编号 */
/*001*/ byte_t  stat;   /* 输出电平 */
/*002*/ byte_t  time;   /* 维持时间(秒, 0xFF 表示常开) */
/*003*/
} CR_PACKED sRFCON_OUT;

/* RFCON 纸币操作结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  guid;   /* 通讯唯一编号 */
/*001*/ byte_t  cash;   /* 纸币面值/纸币处理 */
/*002*/
} CR_PACKED sRFCON_CASH_OP;

/* RFCON 纸币结果结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  guid;   /* 通讯唯一编号 */
/*001*/ byte_t  ends;   /* 纸币处理结果 */
/*002*/ byte_t  cash;   /* 纸币面值的值 */
/*003*/
} CR_PACKED sRFCON_CASH_RS;

/* RFCON 硬币投入结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  guid;   /* 通讯唯一编号 */
/*001*/ byte_t  coin;   /* 投入的硬币数 */
/*002*/
} CR_PACKED sRFCON_COIN_IN;

/* RFCON 找零信息结构 */
CR_TYPEDEF struct
{
/*000*/ byte_t  changed;    /* 已找零数量 */
/*001*/ byte_t  unchanged;  /* 未找零数量 */
/*002*/ byte_t  status;     /* 硬币余量信息 */
/*003*/
} CR_PACKED sRFCON_COIN_OT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* RFCON 串口板返回结构 */
typedef struct
{
        uint_t  type;   /* 帧类型 */
        uint_t  size;   /* 帧长度 */
        byte_t  cmds;   /* 命令值 */

        /* 共用的数据结构 */
        union
        {
            byte_t          error;      /* 错误值 */
            byte_t          id[32];     /* 条码值 */
            int32u          cardid;     /* 卡号值 */
            sRFCON_INP      input;      /* 输入口 */
            sRFCON_CASH_OP  cashop;     /* 纸币机 */
            sRFCON_CASH_RS  cashrs;     /* 纸币机 */
            sRFCON_COIN_IN  coinin;     /* 硬币机 */
            sRFCON_COIN_OT  coinot;     /* 吐币机 */

        } data;

} sRFCON_INFO;

/* 设备类型取值 */
#define RFCON_ID    'A' /* 条码枪 */
#define RFCON_M1    'R' /* 读卡机 */
#define RFCON_INP   'I' /* 输入口 */
#define RFCON_OUT   'O' /* 输出口 */
#define RFCON_UPS   'U' /* UPS 电源 */
#define RFCON_CASH  'B' /* 纸币机 */
#define RFCON_COIN  'C' /* 硬币机 */
#define RFCON_SPIT  'G' /* 吐币机 */
#define RFCON_PRNT  'P' /* 打印机 */
#define RFCON_STAT  'S' /* 状态切换 */
#define RFCON_WDOG  'V' /* 心跳检测 */
#define RFCON_XDIR  'X' /* 直接透传 */

/* 交易阶段代码 */
#define RFCON_R_IDLE    0x00    /* 空闲 */
#define RFCON_R_FEES    0x01    /* 收费 */
#define RFCON_R_CHNG    0x02    /* 找零 */
#define RFCON_R_PRNT    0x03    /* 打印 */

/* 纸币吞吐控制 */
#define RFCON_CASH_IN   0x01    /* 纸币吞入 */
#define RFCON_CASH_OT   0x02    /* 纸币吐出 */

/* 打印机状态字 */
#define RFCON_PRNT_BUSY     0x01    /* 忙碌 */
#define RFCON_PRNT_OFFLINE  0x02    /* 离线 */
#define RFCON_PRNT_NOPAPER  0x04    /* 缺纸 */
#define RFCON_PRNT_LEAKPPR  0x08    /* 纸将尽 */
#define RFCON_PRNT_COM_ERR  0x80    /* 通讯错 */

/* 读卡器认证方式 */
#define RFCON_IC_KEYS   0x00    /* 密码认证 */
#define RFCON_IC_KEYA   0x01    /* KEYA认证 */
#define RFCON_IC_KEYB   0x02    /* KEYB认证 */

/* 读卡器错误代码 */
#define TCR_ERR_OVERTIME        0x10    /* 超时 */
#define TCR_ERR_DEVICE          0x12    /* 吞/吐卡机故障, 无响应 */
#define TCR_READ_FM1702_ERR     0x70    /* FM1702 出错 */
#define TCR_READ_SN_ERR         0X71    /* 卡错误, 读不了 */
#define TCR_CARD_KEY_ERR        0x72    /* 密码错误 */
#define TCR_READ_CARD_ERR       0x73    /* 读卡扇区块错误 */
#define TCR_WRITE_CARD_ERR      0x74    /* 写卡错误 */
#define TCR_SECTORS_NUM_ERR     0x75    /* 扇区超范围 */
#define TCR_PARA_ERR            0x76    /* 参数(数据)错误 */
#define TCR_LEN_ERR             0x77    /* 长度不对 */
#define TCR_DATA_ERR            0x78    /* 数据错误 */
#define TCR_MCAH_BLOCK_ERR      0x79    /* 卡机阻塞 */

CR_API uint_t   rfcon_get_money (byte_t cash);
CR_API bool_t   rfcon_sio_input (uint_t port, sRFCON_INFO *info);
CR_API bool_t   rfcon_sio_output (uint_t port, const sRFCON_OUT *gpio,
                                  uint_t count);
CR_API bool_t   rfcon_sio_alarm (uint_t port, byte_t on_off, byte_t delays);
CR_API bool_t   rfcon_sio_sound (uint_t port, byte_t on_off);
CR_API bool_t   rfcon_sio_switch (uint_t port, byte_t state, byte_t param);
CR_API bool_t   rfcon_sio_ic_read (uint_t port, byte_t secs, byte_t dat[48]);
CR_API bool_t   rfcon_sio_ic_write (uint_t port, byte_t secs,
                                    const byte_t dat[48]);
CR_API bool_t   rfcon_sio_ic_cardio (uint_t port, bool_t getin);
CR_API bool_t   rfcon_sio_ic_cardid (uint_t port, byte_t rcv[8], uint_t *size);
CR_API bool_t   rfcon_sio_ic_setpass (uint_t port, const byte_t pass[8]);
CR_API bool_t   rfcon_sio_ic_access (uint_t port, byte_t type,
                                     const byte_t key[6]);
CR_API uint_t   rfcon_sio_print_stt (uint_t port);
CR_API bool_t   rfcon_sio_print_txt (uint_t port, const byte_t *cmds,
                                     byte_t size);
CR_API bool_t   rfcon_sio_ups (uint_t port, bool_t on_off);
CR_API bool_t   rfcon_sio_xsend (uint_t port, byte_t xcom, const void_t *data,
                                 byte_t size);
CR_API bool_t   rfcon_sio_xiorw (uint_t port, byte_t xcom, void_t *obuff,
                            uint_t *osize, const void_t *ibuff, byte_t isize);

/*****************************************************************************/
/*                              RFLCK 通讯协议                               */
/*****************************************************************************/

/*
*********************************************
b15         NS - 车位锁的网络状态
                    1: 车位锁在网
                    0: 车位锁不在网
*********************************************
b14:b12     LS[2:0] - 锁的当前状态
                    110: 延续运行
                    101: 下降运动
                    100: 上升运动
                    011: 停在中间
                    010: 停在下限位
                    001: 停在上限位
                    000: 其他
*********************************************
b11:b09     OPSRC[2:0] - 开关锁的信号源
                    101: 遇阻停止
                    100: 遇阻反转
                    011: 协议控制
                    010: 口线控制
                    001: 遥控
                    000: 其他
*********************************************
b08         SCAU - 电机停止原因
                    1: 超时
                    0: 限位
*********************************************
b07         LED - LED 是否闪烁标志
                    1: 报警
                    0: 无
*********************************************
b06:b05     BEEP[1:0] - 蜂鸣器鸣叫标志
                    11: 外力推开
                    10: 停止工作
                    01: 电量不足
                    00: 其他
*********************************************
b04:b01     WS[3:0] - 当前供电源状态
                    1000: 过充
                    0111: 没有电池
                    0110: 掉电(外电不足)
                    0101: 充电
                    0100: 休眠
                    0011: 电量不足
                    0010: 电池
                    0001: 外电
                    0000: 开始
*********************************************
b00         SEN - 电感检测器状态
                    1: 有信号
                    0: 无信号
*********************************************
*/

/* RFLCK 电机控制参数 */
#define RFLCK_GO_UP     'R'     /* 上升 */
#define RFLCK_GO_DOWN   'F'     /* 下降 */
#define RFLCK_GO_STOP   'S'     /* 停止 */

CR_API uint_t   rflck_moto_ctrl (ansi_t *str, byte_t line, byte_t id,
                                 ansi_t stt);
CR_API bool_t   rflck_udp_lck_iorw (socket_t netw, void_t *obuff,
                                    uint_t *osize, const ansi_t *ibuff);
CR_API bool_t   rflck_udp_lck_read (socket_t netw, int16u state[96]);
CR_API bool_t   rflck_udp_lck_feed (socket_t netw, int16u state[96]);

/*****************************************************************************/
/*                              SDT11 通讯协议                               */
/*****************************************************************************/

/* 接收缓冲区需要多放2个字节的校验 */
CR_API bool_t   sdt11_sio_send (uint_t port, byte_t addr,
                                const void_t *data, byte_t size);
CR_API bool_t   sdt11_sio_recv (uint_t port, byte_t addr,
                                void_t *data, uint_t *size);
CR_API bool_t   sdt11_sio_iorw (uint_t port, byte_t daddr,
                                byte_t saddr, void_t *obuff,
                                uint_t *osize, const void_t *ibuff,
                                byte_t isize, comm_delay_t delay);

/*****************************************************************************/
/*                              SDT22 通讯协议                               */
/*****************************************************************************/

/* 接收缓冲区需要多放2个字节的校验 */
CR_API bool_t   sdt22_sio_send (uint_t port, int16u addr,
                                const void_t *data, int16u size);
CR_API bool_t   sdt22_sio_recv (uint_t port, int16u addr,
                                void_t *data, uint_t *size);
CR_API bool_t   sdt22_sio_iorw (uint_t port, int16u daddr,
                                int16u saddr, void_t *obuff,
                                uint_t *osize, const void_t *ibuff,
                                int16u isize, comm_delay_t delay);

/*****************************************************************************/
/*                              SFRW 通讯协议                                */
/*****************************************************************************/

/* SFRW 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 文件信息结构 */
CR_TYPEDEF struct
{
/*000*/ int32u  size;   /* 文件大小 */
/*004*/ int32u  crc32;  /* 文件校验 */
/*008*/
} CR_PACKED sSFRW_INFO;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* 功能支持模式 */
#define SFRW_DOWN   0x01    /* 下载支持 */
#define SFRW_UPPE   0x02    /* 上传支持 */

CR_API bool_t   sfrw_tcp_srv_loop (socket_t netw, uint_t flags,
                                   const ansi_t *down, const ansi_t *uppe);
CR_API bool_t   sfrw_tcp_cli_find (socket_t netw, sSFRW_INFO *info,
                                   const ansi_t *path);
CR_API bool_t   sfrw_tcp_cli_pull (socket_t netw, const ansi_t *path,
                                   const ansi_t *name);
CR_API bool_t   sfrw_tcp_cli_push (socket_t netw, const ansi_t *path,
                                   const ansi_t *name);

/*****************************************************************************/
/*                              TX10I 通讯协议                               */
/*****************************************************************************/

/* TX10I 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  vers;   /* 协议版本 (当前为0x01) */
/*001*/ int16u  size;   /* 全协议长度 (包含校验) */
/*003*/ int16u  tags;   /* 包识别码 (会话的编号) */
/*005*/ byte_t  ctype;  /* 命令类型 (bit0: 1-请求 0-应答)
                                    (bit1: 1-密文 0-明文) */
/*006*/ byte_t  stype;  /* 源类型 */
/*007*/ int16u  srcid;  /* 源编号 */
/*009*/ byte_t  dtype;  /* 目标类型 */
/*010*/ int16u  dstid;  /* 目标编号 */
/*012*/
} CR_PACKED sTX10I_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

#define TX10I_ACK   0x00    /* 应答位 */
#define TX10I_REQ   0x01    /* 请求位 */
#define TX10I_TXT   0x00    /* 明文位 */
#define TX10I_CRY   0x02    /* 密文位 */

/* TX10I 通讯上下文对象 */
typedef void_t*     tx10i_t;

/* 接收和发送的数据为 sTX10I_OPS 结构 */
CR_API void_t   tx10i_make_key (tx10i_t ctx, int32u rnd, bool_t recv);
CR_API tx10i_t  tx10i_net_open (socket_t netw, const byte_t key_recv[16],
                                               const byte_t key_send[16]);
CR_API void_t   tx10i_net_close (tx10i_t ctx);
CR_API bool_t   tx10i_udp_send (tx10i_t ctx, sTX10I_HDR *head,
                                const void_t *data, int16u size);
CR_API void_t*  tx10i_udp_recv (tx10i_t ctx, sTX10I_HDR *head,
                                uint_t *size);
CR_API void_t*  tx10i_udp_iorw (tx10i_t ctx, sTX10I_HDR *ssend,
                                uint_t *osize, const void_t *ibuff,
                                int16u isize, comm_delay_t delay);

#endif  /* !__CR_COMMLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
