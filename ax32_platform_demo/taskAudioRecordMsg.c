#include"taskAudioRecordRes.c"

extern void AudioShow_RecState(winHandle handle);
extern uint8 g_clock_flag;
extern INT32U g_clock_times;


//定义需要处理的消息
static int audioOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioRecBaterryShow(handle);
	return 0;
}
static int audioCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	//audioRecBaterryShow(handle);
	return 0;
}
static int audioWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioRecBaterryShow(handle);
	return 0;
}
static int audioKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	#if 1
		if(windowIsOpen(&selfclockWindow))
		{
			winDestroy(&handle);
		}
	#endif
		if(SysCtrl.sdcard!=SDC_STAT_NORMAL)
		{
			deg_Printf("error, SysCtrl.sdcard=%x\r\n", SysCtrl.sdcard);
			XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
			return -1;
		}

		if(audioRecordGetStatus() == MEDIA_STAT_START)
		{
			//audioRecordStop();
		}
		else
		{
			//if(SysCtrl.battery > BATTERY_STAT_0)
			audioRecordStart();
		}
            audioFileNameShow(handle);
	}
	return 0;
}
static int audioKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        ENTER();MARK();

		if(audioRecordGetStatus() != MEDIA_STAT_START)
			//taskChange();
            taskStart(TASK_PLAY_BACK, 0);
	}
	return 0;
}

static int audioKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	//return audioKeyMsgOk(handle, parameNum, parame);

	uint32 keyState=KEY_STATE_INVALID;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	#if 1
		if(windowIsOpen(&selfclockWindow))
		{
			winDestroy(&handle);
		}
	#endif
	
		if(audioRecordGetStatus() == MEDIA_STAT_START)
		{
			audioRecordStop();
		}

		taskStart(TASK_VIDEO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_RECORD, KEY_PRESSED));
	}

	return 0;
}

static int audioKeyMsgSave(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	#if 1
		if(windowIsOpen(&selfclockWindow))
		{
			winDestroy(&handle);
		}
	#endif
		if(audioRecordGetStatus() == MEDIA_STAT_START)
		{
			audioRecordStop();
		}

		taskStart(TASK_VIDEO_RECORD, 0);
	}

	return 0;
}

static int audioKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{

	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audioRecordGetStatus() == MEDIA_STAT_START)
		{
			audioRecordStop();
		}

        taskStart(TASK_PLAY_BACK, 0);
	}

	return 0;
}

static int audioKeyMsgMenuLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

         if(windowIsOpen(&selfclockWindow) == 0)
        {
            uiOpenWindow(&selfclockWindow,0);
			g_clock_flag = 1;
			g_clock_times = XOSTimeGet();
        }
	}
	return 0;
}

static int audioSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		return 0;


	if(SysCtrl.battery <= BATTERY_STAT_0)
	{
		if(audioRecordGetStatus() != MEDIA_STAT_STOP)
		{
			audioRecordStop();
		}
	}
	return 0;
}

static int audioKeyMsgRecordLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//if(audioRecordGetStatus() == MEDIA_STAT_START)
		//{
			//audioRecordStop();
		//}
		
        deamon_screen_save(1);
       // uiOpenWindow(&selfclockWindow,0);
	}

	return 0;
}

static int audioKeyMsgPower_L40Play(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audioRecordGetStatus() != MEDIA_STAT_START)
		{
			taskStart(TASK_PLAY_BACK, 0);
		}
	}

	return 0;
}

static int audioKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_clock_check(1);
		deamon_auto_poweroff(1);
			
        if(get_suspended_lcd_value())
        {
            return 0;
        }

	#if 0
		if(audioRecordGetStatus() == MEDIA_STAT_START)
		{
			audioRecordStop();
		}
		
		taskStart(TASK_VIDEO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
	#else

		return audioKeyMsgPower_L40Play(handle, parameNum, parame);

	#endif
	}

	return 0;
}

static int audioKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audioRecordGetStatus() != MEDIA_STAT_STOP)
		{
			audioRecordStop();
		}

		taskStart(TASK_VIDEO_RECORD, 0);
	}

	return 0;
}

static int audioKeyMsgAudio(winHandle handle,uint32 parameNum,uint32* parame)
{
	return audioKeyMsgOk(handle, parameNum, parame);
}


static int audioRecSysMsgTimeUpdate(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioRecTimeShow(handle,audioRecordGetTime());
	return 0;
}

int Test_Delay_Ms(int Delay_Ms)
{
    int time_start = 0;

    time_start = XOSTimeGet();
    while(XOSTimeGet() > (time_start + Delay_Ms))
    {
        break;
    }

    return 0;
}

static int audioKeyMsg1s(winHandle handle,uint32 parameNum,uint32* parame)
{
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
		audioRecBaterryShow_charge(handle); 
	}

#if 0
    if(get_key_clock_value() == 0)
    {
		//if(audioRecordGetStatus() == MEDIA_STAT_START)
        //{
		//	audioRecordStop();
        //}

        audioRecSysMsgTimeUpdate(handle, parameNum, parame);

        deamon_screen_save(1);
        uiOpenWindow(&selfclockWindow,0);
    }
#endif
	AudioShow_RecState(handle);

	return 0;
}


msgDealInfor audioRecordeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,audioOpenWin},
	{SYS_CLOSE_WINDOW,audioCloseWin},
	{SYS_CHILE_COLSE,audioWinChildClose},
	{KEY_EVENT_OK,audioKeyMsgOk},
	{KEY_EVENT_MODE,audioKeyMsgMode},
    //{KEY_EVENT_PHOTO,audioKeyMsgPhoto},
	{KEY_EVENT_RECORD,audioKeyMsgRecord},

	{/*KEY_EVENT_MENULONG*/KEY_EVENT_DOWNLONG,audioKeyMsgMenuLong},
	{SYS_EVENT_BAT,audioSysMsgBattery},
	
	{KEY_EVENT_SAVE,audioKeyMsgSave},
	
	{KEY_EVENT_RECORDLONG,audioKeyMsgRecordLong},
	{KEY_EVENT_AUDIO,audioKeyMsgAudio},
	{KEY_EVENT_POWER,audioKeyMsgPower},
	{KEY_EVENT_ESC,audioKeyMsgEsc},
	{SYS_EVENT_TIME_UPDATE,audioRecSysMsgTimeUpdate},
	{SYS_EVENT_1S,audioKeyMsg1s},
	{EVENT_MAX,NULL},
};

WINDOW(audioRecordWindow,audioRecordeMsgDeal,audioRecordeWin)


