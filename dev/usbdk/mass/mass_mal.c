/**
  ******************************************************************************
  * @file    mass_mal.c [port]
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Medium Access Layer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "platform_config.h"
#include "mass_mal.h"
#include "device.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size[MAX_LUN + 1];     /* 总字节大小 */
uint32_t Mass_Block_Size[MAX_LUN + 1];      /* 一页的大小 */
uint32_t Mass_Block_Count[MAX_LUN + 1];     /* 总的页数量 */

/* 虚拟一个 360KB 的 RAM DISK */
byte_t  g_ram_disk[360 * 1024];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
    /* 一个存储设备初始化 */
    switch (lun)
    {
        default:
        case 0: /* RAM DISK */
            break;

        case 1: /* NAND DISK */
            if (!nand_init())
                return (MAL_FAIL);
            break;
    }
    return (MAL_OK);
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{
    /* 一个存储设备写数据 */
    switch (lun)
    {
        default:
        case 0: /* RAM DISK */
            memcpy(&g_ram_disk[Memory_Offset], Writebuff, Transfer_Length);
            break;

        case 1: /* NAND DISK */
            if (!nand_write(Memory_Offset, Writebuff, Transfer_Length))
                return (MAL_FAIL);
            break;
    }
    return (MAL_OK);
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{
    /* 一个存储设备读数据 */
    switch (lun)
    {
        default:
        case 0: /* RAM DISK */
            memcpy(Readbuff, &g_ram_disk[Memory_Offset], Transfer_Length);
            break;

        case 1: /* NAND DISK */
            if (!nand_read(Memory_Offset, Readbuff, Transfer_Length))
                return (MAL_FAIL);
            break;
    }
    return (MAL_OK);
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
    /* 设置一个存储设备信息 */
    switch (lun)
    {
        default:
        case 0: /* RAM DISK */
            Mass_Block_Size[0]  = 512;
            Mass_Memory_Size[0] = sizeof(g_ram_disk);
            Mass_Block_Count[0] = sizeof(g_ram_disk) / 512;
            break;

        case 1: /* NAND DISK */
            if (!nand_state(&Mass_Memory_Size[1]))
                return (MAL_FAIL);
            Mass_Block_Size[1]  = 512;
            Mass_Block_Count[1] = Mass_Memory_Size[1] / 512;
            break;
    }
    return (MAL_OK);
}

/*******************************************************************************
* Function Name  : MAL_Format
* Description    : Low format
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Format (uint8_t lun)
{
    /* 低级格式化一个存储设备 */
    switch (lun)
    {
        default:
        case 0: /* RAM DISK */
            break;

        case 1: /* NAND DISK */
            nand_format();
            break;
    }
    return (MAL_OK);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

