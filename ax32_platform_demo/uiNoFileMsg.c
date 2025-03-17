#include "uiNoFileRes.c"

static int noFileKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_keysound_play();
		taskChange();
	}
	return 0;
}

static int noFileKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_keysound_play();
		taskStart(TASK_VIDEO_RECORD,0);
		//XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_RECORD, KEY_PRESSED));
	}
	return 0;
}

static int noFileKeyMsgAudio(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_keysound_play();
		taskStart(TASK_AUDIO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_AUDIO, KEY_PRESSED));
	}
	return 0;
}

static int noFileKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	return noFileKeyMsgMode(handle, parameNum, parame);
}

static int noFileSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{

#if 0
    if(get_key_clock_value() == 0)
    {
        deamon_screen_save(1);
        uiOpenWindow(&selfclockWindow,0);
    }
#endif

	return 0;
}

static int noFileKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
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

        //taskStart(TASK_VIDEO_RECORD,0);
        //XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
	}

	return 0;
}

static int noFileKeyMsRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        taskStart(TASK_AUDIO_RECORD,0);
        XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
	}

	return 0;
}

static int noFileKeyMsgRecordLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        deamon_screen_save(1);
       // uiOpenWindow(&selfclockWindow,0);
	}

	return 0;
}

static int noFileSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		//if(managerFileCount(SysCtrl.avi_list)>0)
			winDestroy(&handle);
	}
	return 0;
}
static int noFileOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile open Win!!!\n");
	if(parameNum==1)
		winSetResid(winItem(handle,NO_FILE_TIP),parame[0]);
	return 0;
}
static int noFileCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile Close Win!!!\n");
	return 0;
}
static int noFileWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile WinChild Close!!!\n");
	return 0;
}

msgDealInfor noFileMsgDeal[]=
{
	{KEY_EVENT_MODE,noFileKeyMsgMode},
	{KEY_EVENT_RECORD,noFileKeyMsgRecord},
	{KEY_EVENT_MENU,noFileKeyMsgRecord},
	{KEY_EVENT_AUDIO,noFileKeyMsgAudio},
	{KEY_EVENT_POWER,noFileKeyMsgPower},
    {KEY_EVENT_RECORD,noFileKeyMsRecord},
    {KEY_EVENT_RECORDLONG,noFileKeyMsgRecordLong},
	{KEY_EVENT_ESC,noFileKeyMsgEsc},
    {SYS_EVENT_1S,noFileSysMsg1S},
	{SYS_OPEN_WINDOW,noFileOpenWin},
	{SYS_CLOSE_WINDOW,noFileCloseWin},
	{SYS_CHILE_COLSE,noFileWinChildClose},
	{SYS_EVENT_SDC,noFileSysMsgSD},
	{EVENT_MAX,NULL},
};

WINDOW(noFileWindow,noFileMsgDeal,noFileWin)



