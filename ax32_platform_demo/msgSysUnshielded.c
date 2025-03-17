#include "application.h"

static uint32 usbOutPowerOffCnt=0;
static uint32 batlowPowerOffCnt=0;

extern uint8 g_clock_flag;


static int unshieldedKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//deamon_auto_poweroff(1);
		//deamon_screen_save(1);
	}
	return 0;
}
static int unshieldedKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        //deamon_clock_check(1);
		//deamon_auto_poweroff(1);
		//deamon_screen_save(1);
#if 0
	#if KEY_PRESSED_POWER_TAP
	if( windowIsOpen(&selfclockWindow )&& ( get_key_press_power_clock() == 0 ) )
	#else
		if(windowIsOpen(&selfclockWindow))
	#endif
	    {
	        winDestroy(&handle);	
	        return 0;
	    }
#endif		
	}
	return 0;
}

static int unshieldedKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        deamon_clock_check(1);
		deamon_auto_poweroff(1);
		//deamon_screen_save(1);
		 if(get_suspended_lcd_value())
        {
            return 0;
        }

	#if KEY_PRESSED_POWER_TAP
	if( windowIsOpen(&selfclockWindow )&& ( get_key_press_power_clock() == 0 ) )
	#else
		if(windowIsOpen(&selfclockWindow))
	#endif
	    {
	        winDestroy(&handle);	
	        return 0;
	    }

	if(windowIsOpen(&menuItemWindow))
	{

		winDestroy(&handle);
		XOSTimeDly(50);
		taskStart(TASK_VIDEO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_RECORD, KEY_PRESSED));	
		return 0;
	}
		
	}
	return 0;
}


static int unshieldedKeyAudioMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        deamon_clock_check(1);
		deamon_auto_poweroff(1);
		//deamon_screen_save(1);
		 if(get_suspended_lcd_value())
        {
            return 0;
        }

	if(windowIsOpen(&menuItemWindow))
	{
		winDestroy(&handle);
		XOSTimeDly(50);
		taskStart(TASK_AUDIO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_AUDIO, KEY_PRESSED));	
		return 0;
	}
		
	}
	return 0;
}

static int unshieldedKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}
static int unshieldedKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}

static int unshieldedKeyMsgMenuLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		//deamon_screen_save(1);
		 if(get_suspended_lcd_value())
        {
            return 0;
        }

		if( windowIsOpen(&selfclockWindow) &&( !g_clock_flag ))
	    {
	        winDestroy(&handle);
	    }
	}
	return 0;
}

static int unshieldedKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}

static int unshieldedKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyMsgUp(handle, parameNum, parame);
}

static int unshieldedKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyMsgUp(handle, parameNum, parame);
}

static int unshieldedKeyMsgPhotoLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyMsgUp(handle, parameNum, parame);
}

#if 0
static int unshieldedKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyMsgRecord(handle, parameNum, parame);
}
#endif

static int unshieldedKeyMsgAudio(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyAudioMsgUp(handle, parameNum, parame);
}

static int unshieldedKeyMsgRecordLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyMsgUp(handle, parameNum, parame);
}

static int unshieldedKeyMsgRecordPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	return unshieldedKeyMsgUp(handle, parameNum, parame);
}

extern void setKeyEn(bool en);
static int unshieldedKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 lcdOn=0,irState=0; 
	uint32 keyState=KEY_STATE_INVALID;

    //return 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	#if 0
		deamon_auto_poweroff(1);
		#if IR_MENU_EN>0
		boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&irState);
		if(irState==0)
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
			configSet(CONFIG_ID_IR_LED,R_ID_STR_COM_ON);
		}
		else
		{
		  	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
		  	configSet(CONFIG_ID_IR_LED,R_ID_STR_COM_OFF);
		}
		#else
		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn&&SysCtrl.astern == ASTERN_OFF)
			; //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0);
		else
		{
			deamon_screen_save(1);
			//setKeyEn(true);
		}
		#endif

		#else
		
		#endif
		deg_Printf("sys deal:powerkey\n");
	}
	return 0;
}
static int unshieldedKeyMsgPowerOff(winHandle handle,uint32 parameNum,uint32* parame)
{
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
	{
		XMsgQPost(SysCtrl.sysQ,makeEvent(SELFCLOCK_EVENT_POWROFF,0));
    	return 0;
	}
#endif

//	uint32 lcdOn=0;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_POWER_OFF,0);
		deg_Printf("sys deal:power off\n");
	}
	return 0;
}
extern void setSoftwareDetachUsensor(BOOL en);
static int unshieldedMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	deamon_screen_save(1);
	setKeyEn(true);
	if((SysCtrl.usb != USB_STAT_NULL))  // DC IN
	{
		setSoftwareDetachUsensor(FALSE);
		usbOutPowerOffCnt=0;
		batlowPowerOffCnt=0;
	}
	else
		usbOutPowerOffCnt=BAT_SAVE_POWER;
	if((SysCtrl.usb == USB_STAT_PC))
	{
		uiOpenWindow(&menuUsbWindow,0);
	}
	return 0;
}
static int unshieldedMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{      
	int ret;
	deamon_screen_save(1);
	setKeyEn(true);
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		ret = open((const char *)"SELFTEST.bin",FA_READ);
		if(ret>=0)
		{
			close(ret);
			uiOpenWindow(&selfTestWindow,0);
		}
	}
	return 0;
}
static int unshieldedMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
    {
        return 0;
    }
#endif

	if(SysCtrl.battery <= BATTERY_STAT_2)
	{
		setSoftwareDetachUsensor(TRUE);
		if(backrec_adpt())
		{
			backrec_stop();
			deg_Printf("software detach Usensor!!!\n\n\n\n");
			if(SysCtrl.battery < BATTERY_STAT_2)
				uiOpenWindow(&tips1Window,2,TIPS_POWER_LOW,TIPS_KEEP_SHOWING);
			else
				uiOpenWindow(&tips1Window,2,TIPS_USENSOR_POWER_LOW,TIPS_KEEP_SHOWING);
		}
	}
#if 0	//by henry 20220623
	if(SysCtrl.battery <= BATTERY_STAT_1)
	{
		//if(batlowPowerOffCnt==0) 
		//	batlowPowerOffCnt=5;
        if(get_clock_state() == 0)
    		uiOpenWindow(&tips1Window,2,TIPS_POWER_LOW,TIPS_KEEP_SHOWING);
	}
#endif
	return 0;
}
extern uint32 uvcIntCnt;
static int unshieldedMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{

	if(usbOutPowerOffCnt)
	{
		usbOutPowerOffCnt--;
		if(usbOutPowerOffCnt==0)
		{
			deg_Printf("sys deal:usb out,auto power off\n");
			taskStart(TASK_POWER_OFF,0);
		}
	}
#if 0 //by henry 20220623
	if(batlowPowerOffCnt)
	{
		batlowPowerOffCnt--;
		if(batlowPowerOffCnt==0)
		{
			deg_Printf("sys deal:battery low,auto power off\n");
			taskStart(TASK_POWER_OFF,0);
		}
	}
#endif

	return 0;
}


static int unshieldevideoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}


msgDealInfor sysUnshieldedMsgDeal[]=
{
	{KEY_EVENT_OK,unshieldedKeyMsgOk},
	{KEY_EVENT_UP,unshieldedKeyMsgUp},
	{KEY_EVENT_DOWN,unshieldedKeyMsgDown},
	{KEY_EVENT_MENU,unshieldedKeyMsgMenu},
	{/*KEY_EVENT_MENULONG*/KEY_EVENT_DOWNLONG,unshieldedKeyMsgMenuLong},
	{KEY_EVENT_MENULONG,unshieldedKeyMsgMenuLong},
	{KEY_EVENT_UPLONG,unshieldedKeyMsgMenuLong},
	{KEY_EVENT_MODE,unshieldedKeyMsgMode},
	{KEY_EVENT_POWER,unshieldedKeyMsgPower},
    {KEY_EVENT_PHOTO,unshieldedKeyMsgPhoto},
    {KEY_EVENT_PHOTOLONG,unshieldedKeyMsgPhotoLong},
    {KEY_EVENT_RECORD,unshieldedKeyMsgRecord},
    {KEY_EVENT_AUDIO,unshieldedKeyMsgAudio},
    {KEY_EVENT_RECORDLONG,unshieldedKeyMsgRecordLong},
    {KEY_EVENT_RECORD_PHOTO,unshieldedKeyMsgRecordPhoto},
	{KEY_EVENT_ESC,unshieldedKeyMsgEsc},
	{KEY_EVENT_POWEROFF,unshieldedKeyMsgPowerOff},
	{SYS_EVENT_SDC,unshieldedMsgSD},
	{SYS_EVENT_USB,unshieldedMsgUSB},
	{SYS_EVENT_BAT,unshieldedMsgBattery},
	{SYS_EVENT_1S,unshieldedMsg1S},
	//{SYS_EVENT_MD,unshieldevideoSysMsgMD},
	{EVENT_MAX,NULL},
};




