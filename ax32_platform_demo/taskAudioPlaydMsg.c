#include"taskAudioPlayRes.c"

extern int audio_play_start(int idx);
static uint32 getAudioPlayResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=INVALID_RES_ID;
	if(str)
		*str=WAV_ID(item+1);
}

static int audioPlayOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	int sum;

#if (1 == MP3_DECODE_SURPORT)
	if(mp3_name_list_init()<0)
		return -1;
#endif
	
	sum=managerFileCount(SysCtrl.wav_list);
	if(sum<0)
		sum=0;
	itemManageSetHeightAvgGap(winItem(handle,AUDIO_REC_SELECT_ID),R1h(33));
	itemManageCreateItem(winItem(handle,AUDIO_REC_SELECT_ID),itemCreatemenuOption,getAudioPlayResInfor,sum);
	itemManageSetCharInfor(winItem(handle,AUDIO_REC_SELECT_ID),0,ALIGNMENT_LEFT,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,AUDIO_REC_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,AUDIO_REC_SELECT_ID),R_COLOR_GRAY);
	itemManageSetCurItem(winItem(handle,AUDIO_REC_SELECT_ID),SysCtrl.file_index-1);
	if(sum==0)
		uiOpenWindow(&noFileWindow,1,"no audio file");
	return 0;
}
static int audioPlayCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int audioPlayWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	int sum;
	sum=managerFileCount(SysCtrl.wav_list);
	if(sum<=0)
	{
		uiOpenWindow(&noFileWindow,1,"no audio file");
		return 0;
	}
	if(SysCtrl.file_index>sum)
			SysCtrl.file_index = 1;
	itemManageUpdateRes(winItem(handle,AUDIO_REC_SELECT_ID),sum,SysCtrl.file_index-1);
	return 0;
}
static int audioPlayKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
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
		
		item=itemManageGetCurrentItem(winItem(handle,AUDIO_REC_SELECT_ID));
		if(SysCtrl.file_index!=item+1)
		{
			if(audioPlaybackGetStatus() != MEDIA_STAT_STOP)
				audioPlaybackStop();
		}
		SysCtrl.file_index=item+1;
		deg_Printf("audio play state ->%d\n",audioPlaybackGetStatus());
		if(audioPlaybackGetStatus() == MEDIA_STAT_STOP)
		{
			audio_play_start(SysCtrl.file_index);
		}
		else if(audioPlaybackGetStatus() == MEDIA_STAT_START)
		{
			AUDIO_DIS();
			audioPlaybackPuase();
		}
		else if(audioPlaybackGetStatus() == MEDIA_STAT_PAUSE)
		{
			if( configGet(CONFIG_ID_KEYSOUND) == R_ID_STR_COM_ON )
			AUDIO_EN();
			audioPlaybackResume();
		}
	}
	return 0;
}
static int audioPlayKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item=0;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		item=itemManageGetCurrentItem(winItem(handle,AUDIO_REC_SELECT_ID));
		
		if( ( item < 1 ) && ( managerFileCount(SysCtrl.avi_list) > 0 ) )
		{
		#if (1 == MP3_DECODE_SURPORT)	
			mp3_name_list_unit();
		#endif
			taskStart(TASK_PLAY_BACK, 0);
		}
		else
		{
			itemManagePreItem(winItem(handle,AUDIO_REC_SELECT_ID));
		}
	}
	return 0;
}
static int audioPlayKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,AUDIO_REC_SELECT_ID));
	}
	return 0;
}
static int audioPlayKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audioPlaybackGetStatus() != MEDIA_STAT_START)
		{
			//taskChange();
		#if (1 == MP3_DECODE_SURPORT)	
			mp3_name_list_unit();
		#endif
			taskStart(TASK_VIDEO_RECORD, 0);
		}
	}
	return 0;
}

static int audioPlayKeyMsgAudio(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audioPlaybackGetStatus() == MEDIA_STAT_START)
		{
			audioPlaybackStop();
		}

		taskStart(TASK_AUDIO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_AUDIO, KEY_PRESSED));
	}
	return 0;
}

static int audioPlayKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		
		if(audioPlaybackGetStatus() == MEDIA_STAT_START)
		{
			audioPlaybackStop();
		}

		taskStart(TASK_VIDEO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_RECORD, KEY_PRESSED));
	}
	return 0;
}

static int audioPlayKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audioPlaybackGetStatus() == MEDIA_STAT_START)
		{
			audioPlaybackStop();
		}

		if(managerFileCount(SysCtrl.avi_list) < 1)
		{
			uiOpenWindow(&noFileWindow,0);
			return 0;
		}

		taskStart(TASK_PLAY_BACK,0);
	}
	return 0;
}

static int audioPlayKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	return audioPlayKeyMsgOk(handle, parameNum, parame);
}		

static int audioPlaySysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	int sum;
	deg_Printf("player : sdc stat ->%d\n",SysCtrl.sdcard);
	if((SysCtrl.sdcard != SDC_STAT_NORMAL)) // sdc out when recording
	{
		if(audioPlaybackGetStatus() != MEDIA_STAT_STOP)
			audioPlaybackStop();
		uiOpenWindow(&noFileWindow,1,"no audio file");
	}
	else
	{
		
		sum=managerFileCount(SysCtrl.wav_list);
		if(sum<=0)
		{
			uiOpenWindow(&noFileWindow,1,"no audio file");
			return 0;
		}
		if(SysCtrl.file_index>sum)
				SysCtrl.file_index = 1;
		itemManageUpdateRes(winItem(handle,AUDIO_REC_SELECT_ID),sum,SysCtrl.file_index-1);
	}
	return 0;
}

static int audioPlaySysMsgTimeUpdate(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

extern menu MENU(playBack2);

static int AudioplayBackKeyLongMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if((audioPlaybackGetStatus() == MEDIA_STAT_STOP)&&(managerFileCount(SysCtrl.wav_list)>0))
		{
            uiOpenWindow(&menuItemWindow,1,&MENU(playBack2));	
		}
	}
	return 0;
}


msgDealInfor audioPlayMsgDeal[]=
{
	{SYS_OPEN_WINDOW,audioPlayOpenWin},
	{SYS_CLOSE_WINDOW,audioPlayCloseWin},
	{SYS_CHILE_COLSE,audioPlayWinChildClose},
	{KEY_EVENT_OK,audioPlayKeyMsgOk},
	{KEY_EVENT_UP,audioPlayKeyMsgUp},
	{KEY_EVENT_DOWN,audioPlayKeyMsgDown},
	{KEY_EVENT_MENU,audioPlayKeyMsgMode},
	{KEY_EVENT_AUDIO,audioPlayKeyMsgAudio},
	{KEY_EVENT_RECORD,audioPlayKeyMsgRecord},
	{KEY_EVENT_ESC,audioPlayKeyMsgEsc},
	{KEY_EVENT_POWER,audioPlayKeyMsgPower},
	{SYS_EVENT_SDC,audioPlaySysMsgSD},
	{SYS_EVENT_TIME_UPDATE,audioPlaySysMsgTimeUpdate},
	{KEY_EVENT_MENULONG,AudioplayBackKeyLongMsgMenu},
	{EVENT_MAX,NULL},
};

WINDOW(audioPlayWindow,audioPlayMsgDeal,audioPlayWin)


