#include "application.h"
extern msgDealInfor photoEncodeMsgDeal[];
extern sysTask taskPhotoEncode;
INT8U photoPipMode=DISP_PIP_FRONT;

static void task_image_arg(INT16U *width,INT16U *height,INT8U *timestramp)
{
// time stramp	
	*timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
// resolution	
    INT32U value = configValue2Int(CONFIG_ID_PRESLUTION);
    *width = value>>16;
	*height = value&0xffff;
	deg_Printf("take photo : [%d:%d]\n",*width,*height);
} 
int task_image_createfile(int channel,char **filename)
{
	int fHandle,ret;
	char *name;

	if(SysCtrl.sdcard!=SDC_STAT_NORMAL)
	{
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
		return -1;
	}

	ret = managerSpaceCheck(SysCtrl.jpg_list,FILEDIR_PHOTO,1024*5);
	if(ret<0)
	{
		SysCtrl.sdcard = SDC_STAT_FULL;
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
		return -2;
	}
//---------creater file name
	name = managerFileNameCreater(SysCtrl.jpg_list,SUFFIX_JPG,FILEDIR_PHOTO);
	if(name==NULL)
	{
		deg_Printf("photo : create file name fail.\n");
		SysCtrl.sdcard = SDC_STAT_FULL;
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
		return -2;
	}
//---------open file from file system
    hal_sdLock();
	fHandle = open(name,FA_CREATE_NEW | FA_WRITE | FA_READ);
	if(fHandle<0)
	{
		deg_Printf("photo : open file fail.%s\n",name);
		
		ret =  -3;
	}
    if(filename)
		*filename = name;
	return fHandle;
}

/*******************************************************************************
* Function Name  : image_take_photo
* Description    : take a photo by user config
* Input          : none
* Output         : none                                            
* Return         : int <0 fail
*******************************************************************************/
int image_take_photo(void)
{
	char *name;
	int fHandle;
	int ret=-1,flag;
	INT16U width,height;
	INT8U timestramp;

	task_image_arg(&width,&height,&timestramp);
#if USENSOR_RESOLUTION == 1
	hal_watermarkBPos_Adjust(480);
#elif USENSOR_RESOLUTION == 2
	hal_watermarkBPos_Adjust(720);
#elif USENSOR_RESOLUTION == 3
	hal_watermarkBPos_Adjust(960);
#else
	hal_watermarkBPos_Adjust(480);
#endif
	flag = 0;
//-------------channel A
    if((TAKE_PHOTO_MODE)||(photoPipMode == DISP_PIP_FRONT))
    {
	    fHandle = task_image_createfile(VIDEO_CH_A,&name);
		if(fHandle<0)
			goto TAKE_PHOTO_END;

		deg_Printf("take photoA : [%d:%d]\n",width,height);
		ret = imageEncodeStart((FHANDLE)fHandle,width,height,JPEG_Q_75,timestramp);
		if(ret<0)
		{
			deg_Printf("photo : take photo fail.<%d>\n",ret);
			close((FHANDLE)fHandle);
			f_unlink(name);
			ret =  -3;
			goto TAKE_PHOTO_END;
		}
				
		//===handle exif===
#if   (1 == JPEG_EXIF)
		int res;
		u8 *temp_ptr = (u32)hal_sysMemMalloc(fs_size(fHandle),32);
		if(NULL == temp_ptr)
		{
			deg_Printf("image encode : exif handle mem err.\n");
		}
		else
		{
			//memset(temp_ptr,0,fs_size(fHandle)+512);
			res = lseek(fHandle,0x14,0);
			res = read(fHandle,(void *)temp_ptr,fs_size(fHandle)-0x14); 
			u8 *head_ptr = (u32)hal_sysMemMalloc(0x2e8,32);
			if(NULL!=head_ptr)
			{
				u8 datatime[20];
				DATE_TIME_T *rtcTime = hal_rtcTimeGet();
				datatime[0] = (rtcTime->year/1000)%10 +'0';
				datatime[1] = (rtcTime->year/100)%10 +'0';
				datatime[2] = (rtcTime->year/10)%10 +'0';
				datatime[3] = (rtcTime->year)%10 +'0';
				datatime[4] = ':';
				datatime[5] = (rtcTime->month/10)%10 +'0';
				datatime[6] = (rtcTime->month)%10 +'0';
				datatime[7] = ':';
				datatime[8] = (rtcTime->day/10)%10 +'0';
				datatime[9] = (rtcTime->day)%10 +'0';
				datatime[10] = ' ';
				datatime[11] = (rtcTime->hour/10)%10 +'0';
				datatime[12] = (rtcTime->hour)%10 +'0';
				datatime[13] = ':';
				datatime[14] = (rtcTime->min/10)%10 +'0';
				datatime[15] = (rtcTime->min)%10 +'0';
				datatime[16] = ':';
				datatime[17] = (rtcTime->sec/10)%10 +'0';
				datatime[18] = (rtcTime->sec)%10 +'0';
				datatime[19] = 0;
				memset(head_ptr,0,0x2e8);
				res = jpeg_exif_head_init(head_ptr,width,height,datatime);
				head_ptr[0x2e6] = 0xff;
				head_ptr[0x2e7] = 0xD9;
				res = lseek(fHandle,0,0);
				res = write(fHandle,(void *)head_ptr,0x2e8);
				res = write(fHandle,(void *)temp_ptr,fs_size(fHandle)-0x14);
				hal_sysMemFree(head_ptr);
			}
			hal_sysMemFree(temp_ptr);
			deg_Printf("image encode : exif handle filesize 0x%x.\n",fs_size(fHandle));
		}

#endif
		//===end handle exif===

		deamon_fsSizeModify(-1,fs_size(fHandle));
		close(fHandle);
		managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&name[strlen(FILEDIR_PHOTO)]);
		deg_Printf("photoA : take photo ok.<%s>\n",name);
		flag|=1;
    }
//--------channel B
    if(!api_backrec_status())
		goto TAKE_PHOTO_END;
#if TAKE_PHOTO_MODE == 0
    if((photoPipMode != DISP_PIP_BACK))
#else
	//if((photoPipMode != DISP_PIP_FRONT))
#endif		
	{
	    fHandle = task_image_createfile(VIDEO_CH_B,&name);
		if(fHandle<0)
			goto TAKE_PHOTO_END;
	#if USENSOR_RESOLUTION == 1
		width = 640;
		height = 480;
	#elif USENSOR_RESOLUTION == 2
		width = 1280;
		height = 720;
	#elif USENSOR_RESOLUTION == 3
		width = 1280;
		height = 960;
	#else
		width = 640;
		height = 480;
	#endif
		deg_Printf("take photoB : [%d:%d]\n",width,height);
		ret=imageEncodeStartB((FHANDLE)fHandle,width,height,JPEG_Q_75,timestramp);
		if(ret<0)
		{
			deg_Printf("photo : take photo fail.<%d>\n",ret);
			close((FHANDLE)fHandle);
			f_unlink(name);
			ret =  -3;
			goto TAKE_PHOTO_END;
		}
	//-------close file & add to file list
	    deamon_fsSizeModify(-1,fs_size(fHandle));
		close((FHANDLE)fHandle);
		managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&name[strlen(FILEDIR_PHOTO)]);
		deg_Printf("photoB : take photo ok.<%s>\n",name);
		flag|=2;
	}

    hal_sdStop();	
    ret = 0;
TAKE_PHOTO_END:
	hal_sdUnlock();
    if(flag)
		return 0;
	else
		return -1;
//	return ret;	
}

void taskPhotoEncodeOpen(uint32 arg)
{
	deg_Printf("image encode task enter.\n");
	//SysCtrl.pip_mode=DISP_PIP_FRONT;
    imageEncodeInit();
	dispLayerInit(DISP_LAYER_VIDEO); // preview
	photoPipMode=DISP_PIP_FRONT;
    dispLayerSetPIPMode(photoPipMode);
	//msgFuncRegister(photoEncodeMsgDeal);
	uiOpenWindow(&photoEncodeWindow,0);
	
}

void taskPhotoEncodeService(uint32 arg)
{
#if FILE_SYNC_DELETE > 0
    f_unlink_ext_service();
#endif
}
void taskPhotoEncodeClose(uint32 arg)
{
	dispLayerSetPIPMode(SysCtrl.pip_mode);
#if FILE_SYNC_DELETE > 0
    while(f_unlink_ext_service());
#endif	
	dispLayerUninit(DISP_LAYER_VIDEO);
	imageEncodeUninit();
	videoRecordUninit();
	if(SysCtrl.sdcard == SDC_STAT_FULL)
	{
		SysCtrl.sdcard = SDC_STAT_NORMAL;
	}
	//winDestroy(&taskPhotoEncode.handle);
	deg_Printf("image encode task exit.\n");
}
sysTask taskPhotoEncode=
{
	"Photo Encode",
	(u32)NULL,
	taskPhotoEncodeOpen,
	taskPhotoEncodeClose,
	taskPhotoEncodeService,
};


