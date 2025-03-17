/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE DECODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_decode.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image decode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"

#if  MEDIA_CFG_IMAGE_DECODE_EN  >0

/*******************************************************************************
* Function Name  : imageDecodeInit
* Description	 : image decode initial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0
*******************************************************************************/
int imageDecodeInit(void)
{
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : imageDecodeUninit
* Description	 : image decode uninitial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int imageDecodeUninit(void)
{
	return STATUS_OK;
}
/*******************************************************************************
* Function Name  : imageDecodeStart
* Description	 : image decode
* Input 		 : Image_ARG_T *arg : image arg
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageDecodeStart(Image_ARG_T *arg)
{
#define USE_TWICE_DECODE  1
    INT8U *jpegBuffer;
	INT32S addr,size,ret;
	volatile int timeout;

	if(arg == NULL)
		return STATUS_FAIL;
	if(arg->media.type == MEDIA_SRC_FS)
	{
		if(arg->media.src.fd<0)
			return STATUS_FAIL;
		jpegBuffer = (INT8U *)hal_sysMemMalloc(fs_size(arg->media.src.fd),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
		if(jpegBuffer)
		{
			size = fs_size(arg->media.src.fd);
			read(arg->media.src.fd,jpegBuffer,size);		
		}
		addr = 0;
	}
	else if(arg->media.type == MEDIA_SRC_NVFS)
	{
		addr = nv_open((int)arg->media.src.fd);
		if(addr<0)
			return STATUS_FAIL;

		jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(arg->media.src.fd),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
		if(jpegBuffer)
		{
			size = nv_size((int)arg->media.src.fd);
			nv_read(addr,jpegBuffer,size);					
		}

	}
	else if(arg->media.type == MEDIA_SRC_RAM)
	{
		if(arg->media.src.buff == 0)
			return STATUS_FAIL;
		jpegBuffer = (INT8U *)arg->media.src.buff;
		addr = 0;
	}
	else
		return STATUS_FAIL;
    
	if(jpegBuffer == NULL)
	{
		return STATUS_FAIL;
	}

	ret = 0;
	if(hal_mjpegHeaderParse(jpegBuffer)<0)
	{
		ret = -1;
		deg_Printf("headparse fail\n");
		goto END_INAGE_DECODE;
    }
	INT16U src_width = 0, src_height = 0;
	INT16U dst_width = 0, dst_height = 0;
	INT32U temp_yout = 0, temp_uvout = 0;
	INT32U decode_twice = 0;
	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	//deg_Printf("src_width:%d,src_height:%d\n",src_width,src_height);
//	deg_Printf("targ width:%d, targ height:%d\n",arg->target.width,arg->target.height);
#if USE_TWICE_DECODE
	if((src_height > arg->target.height * 4)&&(arg->media.type != MEDIA_SRC_RAM))
	{
		dst_width = (arg->target.width *2+0x1f)&(~0x1f) ;
		dst_height = arg->target.height *2;
		//dst_width = (src_width/2+0x1f)&(~0x1f) ;
		//dst_height = src_height/2;
		temp_yout = (u32)hal_sysMemMalloc(dst_width*dst_height*3/2,64);
		if(temp_yout ==0){
			deg_Printf("malloc buf fail, use direct decode\n");
			decode_twice = 0;
		}else{
			temp_uvout = temp_yout + dst_width*dst_height;
			decode_twice = 1;
			deg_Printf("use twice decode\n");
		}	
	}
	else{
		decode_twice = 0;
	}
#else
	decode_twice = 0;
#endif
//    if(jpeg_decode_picture(jpegBuffer,arg->yout,arg->uvout,arg->target.width,arg->target.height) == false)
	if(decode_twice == 0)
	{
		if(hal_mjpegDecodePicture(jpegBuffer,arg->yout,arg->uvout,arg->target.width,arg->target.height)<0)
		{
			ret = -1;
			goto END_INAGE_DECODE;
		}

		if(arg->wait) // wait decode end
		{
	//		timer_ticktimer_start();
			timeout = 0;
			while(hal_mjpegDecodeStatusCheck())
			{
				timeout++;
				if(timeout>=0x0fffff)
					break;
			}
	//	    ticktimer_stop();
		}

		if(hal_mjpegDecodeErrorCheck()!=0)
			ret = -1;
		goto END_INAGE_DECODE;
	}
	else
	{
		if(hal_mjpegDecodePicture_2(jpegBuffer,(u8*)temp_yout,(u8*)temp_uvout,dst_width,dst_height)<0)
		{
			ret = -1;
			goto END_INAGE_DECODE;
		}
		if(jpeg1_decode_check()<0)
		{
			ret = -1;
			goto END_INAGE_DECODE;
		}
		//ax32xx_mjpegDecodeStop();
		//ax32xx_mjpegDecodeEnable(0);
		
		if(ret>=0)
		{
			ax32xx_mjpegEncodeInit(1,0);
			ax32xx_mjpeg_inlinebuf_init((u8*)temp_yout,(u8*)temp_uvout);
			ax32xx_mjpegEncodeSizeSet(dst_width, dst_height, dst_width, dst_height);
			ax32xx_mjpegEncodeQuilitySet(JPEG_Q_75);
			ax32xx_mjpegEncodeBufferSet((u32)jpegBuffer,(u32)(jpegBuffer)+size);
			ax32xx_mjpegEncodeInfoSet(0);
			ax32xx_mjpeg_manual_on();
			if(ax32xx_mjpeg_encode_check()<0)
			{
				ret = -1;
				goto END_INAGE_DECODE;
			}
			
			if(hal_mjpegDecodePicture(jpegBuffer,arg->yout,arg->uvout,arg->target.width,arg->target.height)<0)
			{
				ret = -1;
				goto END_INAGE_DECODE;
			}

			if(arg->wait) // wait decode end
			{
		//		timer_ticktimer_start();
				timeout = 0;
				while(hal_mjpegDecodeStatusCheck())
				{
					timeout++;
					if(timeout>=0x0fffff)
						break;
				}
		//	    ticktimer_stop();
			}

			if(hal_mjpegDecodeErrorCheck()!=0)
				ret = -1;
			hal_mjpegDecodeSetResolution(src_width,src_height);
		}
	}
	
END_INAGE_DECODE:
	if(arg->media.type != MEDIA_SRC_RAM)
		hal_sysMemFree((void *)jpegBuffer);

		//shareMemFree(jpegBuffer);
	if(temp_yout)
		hal_sysMemFree((void *)temp_yout);
	temp_yout = temp_uvout = 0;
	
    if(ret<0)
	    return STATUS_FAIL;
	else
		return STATUS_OK;
}
/*******************************************************************************
* Function Name  : imageDecodeGetResolution
* Description	 : image decode get jpeg resolution
* Input 		 : INT16U *width,INT16U *height
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageDecodeGetResolution(INT16U *width,INT16U *height)
{
	hal_mjpegDecodeGetResolution(width,height);

	return 0;
}




#endif







