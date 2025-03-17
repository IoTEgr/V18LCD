/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         SPI HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_spi.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#include "../inc/hal.h"

//static u8 bSFBusy_Flag=0;
/*******************************************************************************
* Function Name  : hal_spiIOShare
* Description	 : share spi io for other application using
* Input 		 : 
* Output		 : None
* Return		 : 
*******************************************************************************/
u8 hal_spiIOShare(void)
{
//	u8 temp;
//	
//	temp = bSFBusy_Flag;
//	bSFBusy_Flag = 1;
//
//	return temp;
    return 0;
}
/*******************************************************************************
* Function Name  : hal_spiIOCheck
* Description	 : share spi io check
* Input 		 : 
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_spiIOCheck(void)
{
//	if(bSFBusy_Flag)
//		ax32xx_spi0PinConfig(HAL_CFG_SPI_BUS_MODE);
//	bSFBusy_Flag = 0;
}
/*******************************************************************************
* Function Name  : hal_spiInit
* Description	 : spi initial
* Input 		 : none
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiInit(void)
{
#if HAL_CFG_DEFALT_SPI == SPI0
    ax32xx_spi0Init(HAL_CFG_SPI_BAUD_RATE,HAL_CFG_SPI_BUS_MODE);   
#else
    #error "no spi1 driver support!"
#endif
}
/*******************************************************************************
* Function Name  : hal_spiSendByte
* Description	 : spi send one byte data
* Input 		 : u8 : byte
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiSendByte(u8 byte)
{
#if HAL_CFG_DEFALT_SPI == SPI0
     ax32xx_spi0SendByte(byte);
#else
     #error "no spi1 driver support!"
#endif
}
/*******************************************************************************
* Function Name  : hal_spiSend
* Description	 : spi send data using DMA
* Input 		 : u32 addr : buffer addr
                    u32 len   : data length
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiSend(u32 addr,u32 len)
{
#if HAL_CFG_DEFALT_SPI == SPI0
    
    
    ax32xx_spi0Send0((void *)addr,len);
    
#else
    #error "no spi1 driver support!"
#endif
}
/*******************************************************************************
* Function Name  : hal_spiRecvByte
* Description	 : spi recv one byte data
* Input 		 : none
* Output		 : None
* Return		 : u8 : 
*******************************************************************************/
u8 hal_spiRecvByte(void)
{
#if HAL_CFG_DEFALT_SPI == SPI0
    return ax32xx_spi0RecvByte();
#else
    #error "no spi1 driver support!"
#endif
}
/*******************************************************************************
* Function Name  : hal_spiRecv
* Description	 : spi recv data using DMA
* Input 		 : u32 addr : buffer addr
                    u32 len   : data length
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiRecv(u32 addr,u32 len)
{
#if HAL_CFG_DEFALT_SPI == SPI0
    if(len&0x0f)
		len = (len&(~0x0f))+0x10;
    
    ax32xx_spi0Recv0((void *)addr,len);
#else
    #error "no spi1 driver support!"
#endif
}
/*******************************************************************************
* Function Name  : hal_spiCSConfig
* Description	 : spi cs config
* Input 		 : u8 level : cs level
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiCSConfig(u8 level)
{
#if HAL_CFG_DEFALT_SPI == SPI0
    ax32xx_spi0CS0Config(level);
#else
    #error "no spi1 driver support!"
#endif
}
/*******************************************************************************
* Function Name  : hal_spiFlashReadID
* Description	 : spi flash read id
* Input 		 : none
* Output		 : None
* Return		 : u32 : id
*******************************************************************************/
u32 hal_spiFlashReadID(void)
{
	u32 u32Result=0;

	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
	
	hal_spiSendByte(SF_READ_ID);
	u32Result = hal_spiRecvByte();
	u32Result = (u32Result<<8)+hal_spiRecvByte();
	u32Result = (u32Result<<8)+hal_spiRecvByte();
	hal_spiCSConfig(1);						//CS HIGH
	
	return u32Result;	
}
/*******************************************************************************
* Function Name  : hal_spiFlashWriteEnable
* Description	 : spi flash write enable
* Input 		 : none
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiFlashWriteEnable(void)
{	
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
    hal_spiSendByte(SF_WRITE_ENABLE);
    hal_spiCSConfig(1);					//CS HIGH	

}
/*******************************************************************************
* Function Name  : hal_spiFlashWait
* Description	 : wait spi flash idle
* Input 		 : none
* Output		 : None
* Return		 : s32 : >=0 success 
                                  <   timeout
*******************************************************************************/
s32 hal_spiFlashWait(void)
{
	volatile u32 iTOCnt = 0;
	
	hal_spiIOCheck();
    hal_spiCSConfig(0);					//CS LOW
    hal_spiSendByte(SF_READ_STATUS);
	while (hal_spiRecvByte()&1)
	{
		if ((iTOCnt++) > SF_STATUS_TIMEOUT)		//2s
		{
			hal_spiCSConfig(1);			//CS HIGH
			return -1;
		}
	}
    hal_spiCSConfig(1);					//CS HIGH
	return 0;		
}
/*******************************************************************************
* Function Name  : hal_spiFlashReadPage
* Description	 : read spi flash one page
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashReadPage(u32 addr,u32 buffer)
{
	u8 *buff = (u8 *)buffer;
    u32 i;
	
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
	hal_spiSendByte(SF_READ_DATA);
	hal_spiSendByte((addr>>16)&0xff);
	hal_spiSendByte((addr>>8 )&0xff);
	hal_spiSendByte((addr    )&0xff);

	if(HAL_CFG_SPI_DMA_USE==0)
	{
		for(i=0;i<SF_PAGE_SIZE;i++)
		{
			*buff++ = hal_spiRecvByte();
		}
	}
	else
	{
		hal_spiRecv(buffer,SF_PAGE_SIZE);
	}

	hal_spiCSConfig(1);					//CS HIGH

	return 0;	
}
/*******************************************************************************
* Function Name  : hal_spiFlashRead
* Description	 : read spi flash
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
                    u32 len   :  data length
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashRead(u32 addr,u32 buffer,u32 len)
{
	u8 *buff = (u8 *)buffer;
    u32 len0 = len;
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
	hal_spiSendByte(SF_READ_DATA);
	hal_spiSendByte((addr>>16)&0xff);
	hal_spiSendByte((addr>>8 )&0xff);
	hal_spiSendByte((addr    )&0xff);
/*    
	if(SPI_DMA_USE==0)
	{
		if(len>SPI_DMA_THROD)
		{
			hal_spiRecv(buffer,len);
			AX32XX_SPI_CS_H;
			return 0;
		}
	}
*/	
	if((len<16) || (HAL_CFG_SPI_DMA_USE==0))
	{
		while(len--)
		{
			*buff++ = hal_spiRecvByte();
		}
		ax32xx_sysDcacheWback(buffer,len0);
	}
	else
	{
        hal_spiRecv(buffer,len);
	}

	hal_spiCSConfig(1);					//CS HIGH

	return 0;
}
/*******************************************************************************
* Function Name  : hal_spiFlashWritePage
* Description	 : write spi flash one page
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashWritePage(u32 addr,u32 buffer)
{
	u8 *buff = (u8 *)buffer;
    u32 i;
	addr &= ~(SF_PAGE_SIZE-1);  // page align
	hal_spiFlashWriteEnable();
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
	hal_spiSendByte(SF_WRITE_DATA);
	hal_spiSendByte((addr>>16)&0xff);
	hal_spiSendByte((addr>>8 )&0xff);
	hal_spiSendByte((addr    )&0xff);
	
	if(1)//HAL_CFG_SPI_DMA_USE==0)
	{
		for(i=0;i<SF_PAGE_SIZE;i++)
			hal_spiSendByte(buff[i]);
	}
	else
	{
		hal_spiSend(buffer,SF_PAGE_SIZE);
	}
	
	hal_spiCSConfig(1);					//CS HIGH	

    return hal_spiFlashWait();
}
/*******************************************************************************
* Function Name  : hal_spiFlashWrite
* Description	 : write spi flash
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
                    u32 len   : data length
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashWrite(u32 addr,u32 buffer,u32 len)
{
	int i,cnt;
	if(len&(SF_PAGE_SIZE-1))
	{
		len = (len&(~(SF_PAGE_SIZE-1)))+SF_PAGE_SIZE;// page align
	}
    addr &= ~(SF_PAGE_SIZE-1);  // page align
	
	cnt = len/SF_PAGE_SIZE;
	for(i=0;i<cnt;i++)
	{
		if(hal_spiFlashWritePage(addr,buffer)<0)
			return -1;
		addr+=SF_PAGE_SIZE;
		buffer+=SF_PAGE_SIZE;
	}

	return 0;
}
/*******************************************************************************
* Function Name  : hal_spiFlashEraseSector
* Description	 : erase spi flash one sector
* Input 		 : u32 blockAddr : sector addr aglin in byte
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashEraseSector(u32 sectorAddr)
{
	sectorAddr&=~(SF_SECTOR_SIZE-1);
	
	hal_spiFlashWriteEnable();
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
	
	hal_spiSendByte(SF_ERASE_SECTOR);
	hal_spiSendByte((sectorAddr>>16)&0xff);
	hal_spiSendByte((sectorAddr>>8 )&0xff);
	hal_spiSendByte((sectorAddr    )&0xff);
	
	hal_spiCSConfig(1);					//CS HIGH	
	
    return hal_spiFlashWait();
}
/*******************************************************************************
* Function Name  : hal_spiFlashEraseBlock
* Description	 : erase spi flash one block
* Input 		 : u32 blockAddr : block addr aglin in byte
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashEraseBlock(u32 blockAddr)
{
	blockAddr&=~(SF_BLOCK_SIZE-1);
		
	hal_spiFlashWriteEnable();
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW
	
	hal_spiSendByte(SF_ERASE_BLOCK);
	hal_spiSendByte((blockAddr>>16)&0xff);
	hal_spiSendByte((blockAddr>>8 )&0xff);
	hal_spiSendByte((blockAddr    )&0xff);
	
	hal_spiCSConfig(1);					//CS HIGH	
	
    return hal_spiFlashWait();
}
/*******************************************************************************
* Function Name  : hal_spiFlashEraseChip
* Description	 : erase spi flash all chip
* Input 		 : 
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashEraseChip(void)
{
	hal_spiFlashWriteEnable();
	hal_spiIOCheck();
	hal_spiCSConfig(0);					//CS LOW

	hal_spiSendByte(SF_ERASE_CHIP);
	
	hal_spiCSConfig(1);					//CS HIGH	
	
    return hal_spiFlashWait();
}





