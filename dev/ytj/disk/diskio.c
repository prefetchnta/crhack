/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ffconf.h"
#include "diskio.h"     /* FatFs lower layer API */
#include "device.h"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    if (store_check())
        return (RES_OK);
    pdrv = pdrv;
    return (STA_NOINIT);
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    if (store_check())
        return (RES_OK);
    pdrv = pdrv;
    return (STA_NOINIT);
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    count  *= FF_MAX_SS;
    sector *= FF_MAX_SS;
    if (store_read(sector, buff, count))
        return (RES_OK);
    pdrv = pdrv;
    return (RES_ERROR);
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    count  *= FF_MAX_SS;
    sector *= FF_MAX_SS;
    if (store_write(sector, buff, count))
        return (RES_OK);
    pdrv = pdrv;
    return (RES_ERROR);
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    switch (cmd)
    {
        default:
        case CTRL_TRIM:
            return (RES_PARERR);

        case CTRL_SYNC:
            break;

        case GET_SECTOR_COUNT:
            *(DWORD*)buff = CR_K2B(512) / FF_MAX_SS;
            break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = FF_MAX_SS;
            break;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = MX25LXX_SSEC_SIZE / FF_MAX_SS;
            break;
    }
    pdrv = pdrv;
    return (RES_OK);
}
