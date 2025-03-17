/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX user port MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : max_user.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib user port managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../bwlib.h"
//#include "max.h"
//#include "maxlib.h"


static INT8U *iconCache;
static INT8U *iconTempBuffer;




extern int fontDrawString(INT8U *dest,INT8U num,INT16S x,INT16S y,INT16U width,INT8U color);
extern int fontGetStringInfo(INT8U num,INT16U *width,INT16U *height);
extern int iconGetData(int num,INT8U *buffer,int len);
extern int iconGetAddr(int num);
extern int iconGetSize(int num,INT16U *width,INT16U *height);
extern int systemEventService(INT16U *value,INT16U *event);

s32_t user_string_draw(void *frame,u32_t id,s16_t x,s16_t y,u16_t width,u32_t color)
{
    return fontDrawString((u8_t *)frame,id,x,y, width,color);

}
s32_t user_string_size_get(u32_t id,u16_t *width,u16_t *height)
{
     return fontGetStringInfo(id,width,height);
 //    return 0;
}
u8_t *user_string_data_get(u32_t id)
{
	return NULL;
}
s32_t user_get_icon(u32_t name,u32_t buff,u32_t size)
{
	iconGetData(name,(u8_t *)buff,size);
	
    return 1;
}
s32_t user_get_icon_addr(u32_t name)
{
	return iconGetAddr(name);
}
void *user_get_icon_line(u32_t addr,u32_t size)
{
	if(nv_read(addr,(void *)iconCache,size)<0)
		return NULL;

	return iconCache;
}
s32_t user_get_iconSize(u32_t name,u16_t *width,u16_t *height)
{
    iconGetSize(name,width,height);
    return 0;
}
s32_t R_userLoadeString(R_STRING_T *table)
{
    return 0;
}
void user_show_frame(u32_t outbuffer)
{

}

#if MAXLIB_MENU_USE >0
s32_t menuGetKey(u32_t *keyValue,u32_t *keyEvent)
{
	u16_t value,event;
	int ret=0;

    event = 0;	
	ret = systemEventService(&value,&event);
	if(ret<0)
		return -1;
	if((ret == 1) && (value&0x20))
	{
		value = menuKeyChange(value);

		if(keyValue)
			*keyValue = value;
		if(*keyEvent)
			keyEvent = MENU_EVE_KEY;
		return 1;
	}
	else if(ret == MENU_EVE_URGENT)  // menu exit for emengcy event
	{
		if(keyValue)
			*keyValue = 0;
		if(keyEvent)
			*keyEvent = MENU_EVE_URGENT;
		return 1;
	}
	else
	{
		if(keyValue)
			*keyValue = value;
		if(*keyEvent)
			*keyEvent = event;
	}
	return 1;
}
s32_t user_init_hook(void)
{
#if CFG_R_ICON_LOAD>0
    iconCache = hal_sysMemMalloc(R_SCREEN_WIDTH*R_SCREEN_HEIGHT,32);
	iconCacheIndex = 0;
#else
	iconCache = halSysMemLineCache;
    iconTempBuffer = hal_sysMemMalloc(R_SCREEN_WIDTH*R_SCREEN_HEIGHT,32);
#endif	
    //debg("[memory]iconTempBuffer:%x,%x\n",(u32)iconTempBuffer,R_SCREEN_WIDTH*R_SCREEN_HEIGHT);
    return 0;
}

extern void *dispLayerGetFrame(INT8U layer);
extern int dispLayerSetFrame(INT8U layer,u32 addr);
s32_t drawServiceHook(u8_t checkflag)
{
	static INT32U lastaddr=0;
	INT32U addr,ret,size;
	INT8U *src,*tar,pixel;
	INT16U tar_w,tar_h,i,j,count;
	INT32U *tar32;

	if((checkflag&DRAW_FORCE) && lastaddr)
		addr = lastaddr;
	else
	    addr = (INT32U)dispLayerGetFrame(1);  // DISP_LAYER_OSD0	    

#if  CFG_R_ICON_LOAD > 0
	ret = drawService(addr,checkflag&0x7f);
#else
    ret = drawService((u32_t )iconTempBuffer,checkflag&0x7f);

    if(!ret)
		return 0;

	src = iconTempBuffer;
	tar = (INT8U *)addr;
	ret = hal_lcdGetShowingRotate();
	if(ret == LCD_ROTATE_0)
	{
	    ax32xx_dmaMemKick((void *)addr,(void *)src,R_SCREEN_WIDTH*R_SCREEN_HEIGHT);
	}
	else if(ret == LCD_ROTATE_270)
	{
		tar_w = R_SCREEN_HEIGHT;
		tar_h = R_SCREEN_WIDTH;
		size = tar_w*tar_h;
		count = 0;
		tar32 = (INT32U *)addr;
		for(i=0;i<tar_h;i++)
		{
			tar = (INT8U *)(i*tar_w+addr);
			src = (INT8U *)(iconTempBuffer+size-tar_h+i);
			if(i==0)
				pixel = *src;
            for(j=0;j<tar_w;j++)
            {
			#if 1	
				if(*src == pixel)
					count++;
				else
				{
					if(count<1)
						count = 1;
					*tar32++ = 0xff000000|(pixel<<16)|(count-1);
					pixel = *src;
					count = 1;
				}
			#else	
				tar[j] = *src;
			#endif
				 src -= tar_h;
            }
			ax32xx_sysDcacheWback((u32)(i*tar_w+addr),tar_w);
		}
		if(count)
		   *tar32++ = 0xff000000+(pixel<<16)+count-1;
	}
	else if(ret == LCD_ROTATE_180)
	{
		tar_h = R_SCREEN_HEIGHT;
		tar_w = R_SCREEN_WIDTH;
		for(i=0;i<tar_h;i++)
		{
			src = (INT8U *)((tar_h-i)*tar_w-1+iconTempBuffer);
			tar = (INT8U *)(i*tar_w+addr);
			for(j=0;j<tar_w;j++)
			{
				*tar++ = *src--;
			}
			ax32xx_sysDcacheWback((u32)(i*tar_w+addr),tar_w);
		}
	}
	else if(ret == LCD_ROTATE_90)
	{
		tar_w = R_SCREEN_HEIGHT;
		tar_h = R_SCREEN_WIDTH;
		size = tar_w*tar_h;
		for(i=0;i<tar_h;i++)
		{
			tar = (INT8U *)(i*tar_w+addr);
			src = (INT8U *)(iconTempBuffer+tar_h-i-1);
            for(j=0;j<tar_w;j++)
            {
				*tar++ = *src;
				 src +=tar_h;
            }
			ax32xx_sysDcacheWback((u32)(i*tar_w+addr),tar_w);
		}
	}
	else if(ret == LCD_ROTATE_VMIRROR)
	{
		tar_h = R_SCREEN_HEIGHT;
		tar_w = R_SCREEN_WIDTH;
		size = tar_w*tar_h;
		for(i=0;i<tar_h;i++)
		{
			src = (INT8U *)(size-tar_w-tar_w*i+iconTempBuffer);
			tar = (INT8U *)(i*tar_w+addr);
			for(j=0;j<tar_w;j++)
			{
				*tar++ = *src++;
			}
			ax32xx_sysDcacheWback((u32)(i*tar_w+addr),tar_w);
		}
	}
	else if(ret == LCD_ROTATE_HMIRROR)
	{
		tar_h = R_SCREEN_HEIGHT;
		tar_w = R_SCREEN_WIDTH;
		size = tar_w*tar_h;
		for(i=0;i<tar_h;i++)
		{
			src = (INT8U *)(tar_w*i+tar_w-1+iconTempBuffer);
			tar = (INT8U *)(i*tar_w+addr);
			for(j=0;j<tar_w;j++)
			{
				*tar++ = *src--;
			}
			ax32xx_sysDcacheWback((u32)(i*tar_w+addr),tar_w);
		}
	}

    
    ax32xx_deUpdate();
#endif
	return 0;//dispLayerSetFrame(1,addr);
}

void LOG_HOOK(void)
{
	drawServiceHook(DRAW_SI_CHECK);
}
#endif
/*******************************************************************************
* Function Name  : user_time_get,user_time_set
* Description    : get/set time         
* Input          :  
* Output        : 
* Return         : 
*******************************************************************************/
#if MAXLIB_TIME_USE >0 
u32_t user_time_get(void)
{
	t_rtc rtc;
	u32_t t;  
    
    rtc_time_get(&rtc);
    rtc_day_get(&rtc);

	t = TIMET_MAKE(rtc.rtc_day,rtc.rtc_hour,rtc.rtc_min,rtc.rtc_sec);

	return t;
}

void user_time_set(u32_t t)
{
//	u32_t t;
	t_rtc rtc;

	rtc.rtc_day = TIMED_GET(t);
	rtc.rtc_hour= TIMEH_GET(t);
	rtc.rtc_min = TIMEM_GET(t);
	rtc.rtc_sec = TIMES_GET(t);

	rtc_time_set(&rtc);
    rtc_day_set(&rtc);
}
#endif
void *max_mem_malloc(int len)
{
#if CFG_R_ICON_LOAD >0	
	u32_t idx;
	
	if((len+iconCacheIndex)<ICON_CACHE_SIZE)
	{
	     idx = iconCacheIndex;
		 iconCacheIndex+=len;

		 return &iconCache[idx];
	}
#endif
    return NULL;
}
void max_mem_free(void * addr)
{
    // mem_free(addr);
}
void max_mem_cpy(s8_t *dest, s8_t *src, u32_t Len)
{
    // memcpy(dest,src,Len);
}
/*******************************************************************************
* Function Name  : dialogKeyChange
* Description    : change user key to dialog key.user should finish this function            
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t user_key_change_dialog(u32_t keyValue,u32_t keyEvent)
{

	return 0;
}