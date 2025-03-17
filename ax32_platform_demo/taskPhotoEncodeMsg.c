#include "application.h"
#include "taskPhotoEncodeRes.c"


int Photo_flag = false;
extern int image_take_photo(void);
static int photoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
		ret = image_take_photo();
		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
		if((ret>=0)&&SysCtrl.f_keysound)
		{
			//ret = logo_music_show(RES_TAKE_PHOTO_MUSIC);
		}
		photoRemainNumShow(handle);
	}
	return 0;
}
extern INT8U photoPipMode;
static int photoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.usensorshow == USENSOR_SHOW_ON)
		{
			if(photoPipMode == DISP_PIP_FRONT)
				photoPipMode=DISP_PIP_BACK;
			else
				photoPipMode=DISP_PIP_FRONT;
    		dispLayerSetPIPMode(photoPipMode);
		}
		else
		{
			#if (1 == LCD_CROP_SCALE)
			if(SysCtrl.pip_mode == 0)
			{
				if(SysCtrl.crop_level<6)
					SysCtrl.crop_level++;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
			}
			#endif
		}

	}
	return 0;
}
static int photoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		/*if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_ON)
		{
			configSet(CONFIG_ID_AUDIOREC,R_ID_STR_COM_OFF);
			
		}
		else
		{
			configSet(CONFIG_ID_AUDIOREC,R_ID_STR_COM_ON);
		}
		photoMicShow(handle);
		*/
		#if (1 == LCD_CROP_SCALE)
			//== lcd scale==
			if(SysCtrl.pip_mode == 0)
			{
				if(SysCtrl.crop_level>0)
					SysCtrl.crop_level--;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
			}
		#endif

	}
	return 0;
}
extern menu MENU(movie);
static int photoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uiOpenWindow(&menuItemWindow,1,&MENU(movie));
	}
	return 0;
}
static int photoKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskChange();
	}
	return 0;
}

static int photoKeyMsgPhotoLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		photoKeyMsgOk(handle, parameNum, parame);
        Photo_flag = 1;
	}
	return 0;
}

static int photoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoSDShow(handle);
	photoRemainNumShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}
static int photoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int photoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Open Win!!!\n");
	layout_version_get();
	winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTO);
	#if SMALL_PANEL_SUPPORT==0
	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	photoRemainNumShow(handle);
    photoPictureShow(handle);

	return 0;
}
static int photoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Close Win!!!\n");
	return 0;
}
static int photoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo WinChild Close!!!\n");
	photoRemainNumShow(handle);
	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	#if SMALL_PANEL_SUPPORT==0
	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	return 0;
}
static int photoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static uint32 flag=0;
    static int delay = 0;

    if(Photo_flag == 1)
    {
        delay++;
        if(delay == 2)
        {
            Photo_flag = 0;
            delay = 0;
            taskStart(TASK_VIDEO_RECORD,0);
        }
    }

	winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	#if SMALL_PANEL_SUPPORT==0
	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	photoIrLedShow(handle);
	/*if(SysCtrl.usb == USB_STAT_DCIN)
	{
		if(winIsVisible(winItem(handle,PHOTO_BATERRY_ID)))
			winSetVisible(winItem(handle,PHOTO_BATERRY_ID),false);
		else
		{
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
			winSetVisible(winItem(handle,PHOTO_BATERRY_ID),true);
		}	
	}*/
	if(SysCtrl.usb != NULL)
	{
        if(SysCtrl.battery == BATTERY_STAT_MAX)
        {
			SysCtrl.battery_charge = BAT_CHARGE_FULL_ICON;
        }
		else if(SysCtrl.battery_charge >= 4 )
		{
			SysCtrl.battery_charge = 1;
		}
		else
		{
			SysCtrl.battery_charge++;
		}
		photoBaterryShow_charge(handle); 
	}

	return 0;
}


msgDealInfor photoEncodeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,photoOpenWin},
	{SYS_CLOSE_WINDOW,photoCloseWin},
	{SYS_CHILE_COLSE,photoWinChildClose},
	{/*KEY_EVENT_OK*/KEY_EVENT_PHOTO,photoKeyMsgOk},
	{KEY_EVENT_UP,photoKeyMsgUp},
	{KEY_EVENT_DOWN,photoKeyMsgDown},
	{KEY_EVENT_MENU,photoKeyMsgMenu},
	{KEY_EVENT_MODE,photoKeyMsgMode},
	{KEY_EVENT_ESC,photoKeyMsgPhotoLong},
	{SYS_EVENT_SDC,photoSysMsgSD},
	{SYS_EVENT_USB,photoSysMsgUSB},
	{SYS_EVENT_BAT,photoSysMsgBattery},
	{SYS_EVENT_MD,photoSysMsgMD},
	{SYS_EVENT_1S,photoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(photoEncodeWindow,photoEncodeMsgDeal,photoEncodeWin)



