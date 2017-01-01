/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-26  */
/*     #######          ###    ###      [HARD]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>> LLB3 MLR 流量结算仪目标板测试源文件 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "device.h"

#include <stdio.h>

/*
=======================================
    外部中断1
=======================================
*/
void_t  isr_ext1 (void_t) interrupt 2
{
}

/*
=======================================
    定时器1中断
=======================================
*/
void_t  isr_cnt1 (void_t) interrupt 3
{
}

/*
=======================================
    串口0中断
=======================================
*/
void_t  isr_sio0 (void_t) interrupt 4
{
}

/*
=======================================
    定时器2中断
=======================================
*/
void_t  isr_cnt2 (void_t) interrupt 5 using 3
{
    b_TF2 = 0;
    led_dpy_scan();
}

/*
=======================================
    外部中断2
=======================================
*/
void_t  isr_ext2 (void_t) interrupt 6
{
}

/*
=======================================
    外部中断3
=======================================
*/
void_t  isr_ext3 (void_t) interrupt 7
{
}

/*
---------------------------------------
    目标板初始化
---------------------------------------
*/
static void_t
board_init (void_t)
{
    /* 关总中断 */
    irqx_disable();

    /* 其他 */
    alarm_off();
    rs485_recv();
    x5045_wrdi();

    /* 总线 */
    spi0_init();
    spi1_init();
    ibt0_init();

    /* 键盘 */
    key_init();

    /* 显示 (刷新一个字符2ms) */
    led_refresh_0("8.8.8.8.8.8.8.8.");
    led_dpy_init(-2000);

    /* 串口 (9600bps 不开中断) */
    uart0_init(CR_0_B9600, FALSE);

    /* 开总中断 */
    irqx_enabled();

    /* 启动延时 */
    delay32(65536UL);
}

/*
---------------------------------------
    发送字符串到串口0
---------------------------------------
*/
static void_t
sio_print (
  __CR_IN__ const ansi_t*   str
    )
{
    rs485_send();
    while (*str != CR_AC(NIL))
        uart0_send(*str++);
    uart0_send(0x0D);
    uart0_send(0x0A);
    rs485_recv();
}

/*
=======================================
    测试主程序
=======================================
*/
void_t  main (void_t)
{
    int16u  addr;
    int16s  temp;
    int32u  count;
    ansi_t  buf[9];
    ubit_t  refresh;
    sint_t  vals = 0;
    byte_t  ukey = 0;

    board_init();
    icl7135_start();    /* 开始采集 */
    if (!ds18b20_do_cmd(DS18B20_CONVERT))
        sio_print("DS18B20 - FAILURE-1");
    addr = 0;
    count = 0UL;
    refresh = TRUE;
    for (;;)
    {
        /* 清看门狗 */
        x5045_clear_wdt();

        /* 按键测试 */
        ukey = key_get_up(1000);
        if (ukey & KEY_ENTER) {
            sio_print("<ENTER>");
        }
        if (ukey & KEY_LEAVE) {
            sio_print("<LEAVE>");
        }
        if (ukey & KEY_UP) {
            vals += 1;
            refresh = TRUE;
            sio_print("<UP>");
        }
        if (ukey & KEY_DOWN) {
            vals -= 1;
            refresh = TRUE;
            sio_print("<DOWN>");
        }
        if (ukey & KEY_LEFT) {
            vals += 100;
            refresh = TRUE;
            sio_print("<LEFT>");
        }
        if (ukey & KEY_RIGHT) {
            vals -= 100;
            refresh = TRUE;
            sio_print("<RIGHT>");
        }

        if (refresh)
        {
            /* 显示测试 */
            if (vals < 0)
                vals = 0;
            sprintf(buf, "%04u", vals);
            led_refresh_1(buf);
            refresh = FALSE;

            /* 报警测试 */
            if (vals > 500)
                alarm_xon();
            else
                alarm_off();

            /* DAC 测试 */
            tlc5615_dacvt(vals);
        }

        /* 温度 & 读写 & 采集测试 */
        if (ds18b20_okay() && count++ > 10000UL)
        {
            /* 温度测试 */
            count = 0UL;
            if (ds18b20_ram_read(buf, 2)) {
                ((byte_t*)(&temp))[0] = (byte_t)buf[1];
                ((byte_t*)(&temp))[1] = (byte_t)buf[0];
                sprintf(buf, "%05.1f", (fp32_t)temp * 0.0625f);
                led_refresh_2(buf);
                if (!ds18b20_do_cmd(DS18B20_CONVERT))
                    sio_print("DS18B20 - FAILURE-2");
            }
            else {
                sio_print("DS18B20 - FAILURE-3");
            }

            /* 读写测试 */
            buf[0] = 0x55;
            buf[1] = 0xAA;
            buf[2] = 0x0F;
            buf[3] = 0xF0;
            buf[4] = 0x00;
            if (!store_write(addr, buf, 5))
                sio_print("X5045 - FAILURE-W");
            buf[4] = 0x00;
            if (!store_read(addr, buf, 5))
                sio_print("X5045 - FAILURE-R");
            addr += 5;
            if (addr > 507)
                addr = 0;

            /* 采集测试 */
            temp = icl7135_get(0);
            sprintf(buf, "1-%u", temp);
            sio_print(buf);
            temp = icl7135_get(1);
            sprintf(buf, "2-%u", temp);
            sio_print(buf);
            temp = icl7135_get(2);
            sprintf(buf, "3-%u", temp);
            sio_print(buf);
        }
        icl7135_refresh();
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
