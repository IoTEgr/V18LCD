/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA LAYER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : USER_MEDIA.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
* 2017-04-24  : mark
*      <1>.Modify memory define,to hal_sys.c,share memory width sytem other application.
******************************************************************************/
#include "media.h"
#include "multimedia.h"


//---------------memory define for multimedia------------------------------------------------

//SECTION(".big_buffer") ALIGNED(32) INT8U aviIdx1Cache[AVI_CFG_IDX1_BUFF_SIZE];  // for avi record & play,cache idx1 table

//SECTION(".big_buffer") ALIGNED(32) INT8U jpegCache[JPEG_CFG_CACHE_SIZE*JPEG_CFG_CACHE_NUM]; // for jpeg decode cache

//SECTION(".big_buffer") ALIGNED(32) INT8U audioCache[AUDIO_CFG_BUFFER_SIZE*AUDIO_CFG_BUFFER_NUM]; // for audio decode cache

//SECTION(".big_buffer") ALIGNED(64) INT8U thumbnailYBuffer[THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT],thumbnailUVBuffer[THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT/2];

MSG_T audsIStack[AUDIO_CFG_BUFFER_NUM],audsTStack[AUDIO_CFG_BUFFER_NUM],audsBStack[AUDIO_CFG_BUFFER_NUM],vidsIStack[VIDEO_CFG_BUFFER_NUM],vidsBStack[VIDEO_CFG_BUFFER_NUM],jpegIStack[JPEG_CFG_CACHE_NUM],jpegBStack[JPEG_CFG_CACHE_NUM];
//------------memory share width other application.[mark modify .2017/04/24]
INT8U *jpegCache;// = (INT8U *)&halSysMemMJPEGBuffer;
INT8U *audioCache;// = (INT8U *)&halSysMemPCMBuffer;
INT8U *aviIdx1Cache;// = halSysMemIdxBuffer;  
INT8U *aviIdx1CacheB;
// thumbnail using the frame of jpeg cache buffer
INT8U *thumbnailYBuffer;// = ((INT8U *)&halSysMemMJPEGBuffer)+JPEG_CFG_CACHE_SIZE;
INT8U *thumbnailUVBuffer;// = (((INT8U *)&halSysMemMJPEGBuffer)+JPEG_CFG_CACHE_SIZE+THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT);
#if 0

typedef struct SHARE_MEM_S
{
	void *addr;
	INT32U size;
}SHARE_MEM_T;

#define  SHARE_MEM_NUM     3
static SHARE_MEM_T SHARE_MEM_NODE[SHARE_MEM_NUM]=
{
	{// avi idx cache buffer
		.addr = (void *)&halSysMemIdxBuffer, // aviIdx1Cache
		.size = AVI_CFG_IDX1_BUFF_SIZE,
    },
    {// jpeg file cache buffer
		.addr = (void *)&halSysMemMJPEGBuffer, // jpegCache
		.size = JPEG_CFG_CACHE_SIZE*JPEG_CFG_CACHE_NUM,
    },
    {// audio cache buffer
		.addr = (void *)&halSysMemPCMBuffer,// audioCache,
		.size = AUDIO_CFG_BUFFER_SIZE*AUDIO_CFG_BUFFER_NUM,
    }

};

int multimediaInit(void)
{
    return 0;
}
/*******************************************************************************
* Function Name  : shareMemMalloc
* Description    : memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : INT32U size : need memory size 
* Output         : none
* Return         : 
*******************************************************************************/
void *shareMemMalloc(INT32U size)
{
	int idx,i;

	idx = SHARE_MEM_NUM;
	for(i=0;i<SHARE_MEM_NUM;i++)
	{
		if((SHARE_MEM_NODE[i].size&0x80000000)==0)
		{
			if(size == SHARE_MEM_NODE[i].size)
				break;
			else if(size<SHARE_MEM_NODE[i].size)
			{
				if(idx == SHARE_MEM_NUM)
					idx = i;
			}
		}
	}

	if(i>=SHARE_MEM_NUM && idx<SHARE_MEM_NUM)
		i = idx;
	if(i<SHARE_MEM_NUM)
	{
		SHARE_MEM_NODE[i].size|=0x80000000;
		return SHARE_MEM_NODE[i].addr;
	}

	return NULL;
}
/*******************************************************************************
* Function Name  : shareMemFree
* Description    : memory share for other application,memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : void *mm : memory addr
* Output         : none
* Return         : 
*******************************************************************************/
void shareMemFree(void *mm)
{
	int i;

	for(i=0;i<SHARE_MEM_NUM;i++)
	{
		if(SHARE_MEM_NODE[i].addr == mm)
			SHARE_MEM_NODE[i].size&=~0x80000000;
	}	
}


#endif

