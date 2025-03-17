#include "application.h"
#include "taskPlayBackRes.c"
#define  VIDEO_PLAY_WAIT_KEYSOUND_END           while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}


extern uint8 g_clock_flag;
extern INT32U g_clock_times;

extern int video_playback_start(int index);
static int playBackKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	//int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			if(videoPlaybackGetStatus()== MEDIA_STAT_START)
			{
				videoPlaybackPuase();
				AUDIO_DIS();
				while(videoPlaybackGetStatus()==MEDIA_STAT_START);
				deamon_keysound_play();
			}
			else if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
			{
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				
				if( configGet(CONFIG_ID_KEYSOUND) == R_ID_STR_COM_ON )
				AUDIO_EN();
				videoPlaybackResume();
			}
			else
			{
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				video_playback_start(SysCtrl.file_index);
			}
		}
	}
	return 0;
}

enum
{
    PB_VOLUME_LEVEL1 = 0,
    PB_VOLUME_LEVEL2,
    PB_VOLUME_LEVEL3,
    PB_VOLUME_LEVEL4,
    PB_VOLUME_LEVEL5,
    PB_VOLUME_LEVELMAX
};
static uint8 PlayBack_Volume_Level = PB_VOLUME_LEVELMAX;

static int playBackKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

#if 0
		if(videoPlaybackGetStatus() == MEDIA_STAT_START)
        {
            if(PlayBack_Volume_Level < PB_VOLUME_LEVELMAX)
            {
                PlayBack_Volume_Level++;
            }
            videoPlaybackSetVolume(20*PlayBack_Volume_Level);
			
            return 0;
        }
#endif
		   
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
		#if 0
			if(state == MEDIA_STAT_PLAY)
				videoPlaybackFastForward();
		#else						
			if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
			{
                  videoPlaybackStop();
			}
			deamon_keysound_play();
			SysCtrl.file_index--;
			if(SysCtrl.file_index<1)
				SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
			 video_playback_start(SysCtrl.file_index);
			 playbackResolutionShow(handle);
			playbackFileNameShow(handle);
			playbackLockShow(handle);
		#endif
		}
	}
	return 0;
}

static int playBackKeyMsgUpLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	static char Str[2];
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
	    {
	        if(PlayBack_Volume_Level < PB_VOLUME_LEVELMAX)
	        {
	            PlayBack_Volume_Level++;
	        }
	        videoPlaybackSetVolume(20*PlayBack_Volume_Level);
			
			winSetStrInfor(winItem(handle,PLAYBACK_VOLUME_SIZE_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
			
			Str[0] = PlayBack_Volume_Level +'0';
			Str[1] = 0;
			winSetResid(winItem(handle,PLAYBACK_VOLUME_SIZE_ID),strToResId(Str));
			
	        return 0;
	    }
		}
	return 0;
}

static int playBackKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

#if 0
		 if(videoPlaybackGetStatus() == MEDIA_STAT_START)
        {
            if(PlayBack_Volume_Level > PB_VOLUME_LEVEL1)
            {
                PlayBack_Volume_Level--;
            }
            videoPlaybackSetVolume(20*PlayBack_Volume_Level);

            return 0;
        }
#endif
		 
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
		#if 0
			if(state == MEDIA_STAT_PLAY)
				videoPlaybackFastBackward();

		#else				
			if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
			{
                  videoPlaybackStop();
			}
			deamon_keysound_play();
			SysCtrl.file_index++;

		#if 0
			if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
				SysCtrl.file_index = 1;
		#else
			if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
			{
				if(managerFileCount(SysCtrl.wav_list) > 0)
				{
					taskStart(TASK_AUDIO_PLAY, 0);
					return 0;
				}
				else
				{
					SysCtrl.file_index=1;
				}
			}
			
		#endif
		
			video_playback_start(SysCtrl.file_index);
			playbackResolutionShow(handle);
			playbackFileNameShow(handle);
			playbackLockShow(handle);
		#endif
		}
	}
	return 0;
}

static int playBackKeyMsgDownLong(winHandle handle,uint32 parameNum,uint32* parame)
{

	static char Str[2];
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			if(PlayBack_Volume_Level > PB_VOLUME_LEVEL1)
			{
				PlayBack_Volume_Level--;
			}
			videoPlaybackSetVolume(20*PlayBack_Volume_Level);

			winSetStrInfor(winItem(handle,PLAYBACK_VOLUME_SIZE_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
			
			Str[0] = PlayBack_Volume_Level +'0';
			Str[1] = 0;
			winSetResid(winItem(handle,PLAYBACK_VOLUME_SIZE_ID),strToResId(Str));
			
		}
	else
		{
			if(windowIsOpen(&selfclockWindow) == 0)
			{
				uiOpenWindow(&selfclockWindow,0);
				g_clock_flag = 1;
				g_clock_times = XOSTimeGet();
			}
		}
	}
	return 0;
}

extern menu MENU(playBack);
static int playBackKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if((videoPlaybackGetStatus() != MEDIA_STAT_START)/*&&(managerFileCount(SysCtrl.avi_list)>0)*/)
		{
			videoPlaybackStop();
			deamon_keysound_play();

		#if 0
            uiOpenWindow(&menuItemWindow,1,&MENU(playBack));	
		#else
			taskStart(TASK_VIDEO_RECORD, 0);
		#endif
			//layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,NULL));
		}
	}
	return 0;
}
static int playBackKeyLongMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if((videoPlaybackGetStatus() != MEDIA_STAT_START)&&(managerFileCount(SysCtrl.avi_list)>0))
		{
			videoPlaybackStop();
			//deamon_keysound_play();

            uiOpenWindow(&menuItemWindow,1,&MENU(playBack));	
	
			//layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,NULL));
		}
	}
	return 0;
}

static int playBackKeyMsgMode_Default(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() != MEDIA_STAT_PLAY)
		{
			 taskChange();
			 deamon_keysound_play();
			 VIDEO_PLAY_WAIT_KEYSOUND_END;
		}
	}
	return 0;
}

static int playBackKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

    return playBackKeyMsgUp(handle,parameNum,parame);

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() != MEDIA_STAT_PLAY)
		{
			 taskChange();
			 deamon_keysound_play();
			 VIDEO_PLAY_WAIT_KEYSOUND_END;
		}
	}
	return 0;
}

static int playBackKeyMsgModeLong(winHandle handle,uint32 parameNum,uint32* parame)
{
    return playBackKeyMsgMode_Default(handle,parameNum,parame);
}

static int playBackKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
    return playBackKeyMsgDown(handle,parameNum,parame);
}

static int playBackKeyMsgPhotoLong(winHandle handle,uint32 parameNum,uint32* parame)
{
    return playBackKeyMsgOk(handle,parameNum,parame);
}

extern int Photo_flag;
static int playBackKeyMsgRecordPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
    uint32 keyState=KEY_STATE_INVALID;
    if(parameNum==1)
        keyState=parame[0];
    if(keyState==KEY_PRESSED)
    {
        if((Photo_flag == 0) && (videoPlaybackGetStatus() != MEDIA_STAT_START))
        {
            taskStart(TASK_PHOTO_ENCODE,0);
            XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_ESC, KEY_PRESSED));
        }
    }
}

static int playBackKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			videoPlaybackStop();
		}

		taskStart(TASK_VIDEO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
	}
	return 0;
}

static int playBackKeyMsgRecordLong(winHandle handle,uint32 parameNum,uint32* parame)
{

    return playBackKeyMsgRecordPhoto(handle, parameNum, parame);


	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			videoPlaybackStop();
		}

        //deamon_screen_save(1);
       // uiOpenWindow(&selfclockWindow,0);
	}
	return 0;
}

static int playBackKeyMsgAudio(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			videoPlaybackStop();
		}

		taskStart(TASK_AUDIO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_AUDIO, KEY_PRESSED));

	}
	return 0;
}

static int playBackKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
		{
			  videoPlaybackStop();
		}

		taskStart(TASK_VIDEO_RECORD, 0);
	}

	return 0;
}

static int playBackKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{

#if 0
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        if(get_suspended_lcd_value())
        {
             return 0;
        }

		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
            videoPlaybackStop();
		}
	}
    
    taskStart(TASK_VIDEO_RECORD,0);
    XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));

	return 0;
#else

	deamon_clock_check(1);
	deamon_auto_poweroff(1);
	//deamon_screen_save(1);

	 if(get_suspended_lcd_value())
    {
        return 0;
    }

	return playBackKeyMsgOk(handle, parameNum, parame);

#endif
}

#if 0

static int playBackKeyMsgUpLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			videoPlaybackFastForward();
		}
	}

	return 0;
}

static int playBackKeyMsgDownLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			videoPlaybackFastBackward();
		}
	}

	return 0;
}
#endif

static int playBackSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("player : sdc stat ->%d\n",SysCtrl.sdcard);
	if((SysCtrl.sdcard != SDC_STAT_NORMAL)/* && (state == MEDIA_STAT_PLAY)*/) // sdc out when recording
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		    videoPlaybackStop();
		uiOpenWindow(&noFileWindow,0);
	}
	else
	{
		SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
		if(SysCtrl.file_index>0)
		{
			video_playback_start(SysCtrl.file_index);
			playbackResolutionShow(handle);
			playbackFileNameShow(handle);
			playbackLockShow(handle);
		}
		 else if(managerFileCount(SysCtrl.avi_list))
        {
			taskStart(TASK_AUDIO_PLAY, 0);
        }
		else
		{
			uiOpenWindow(&noFileWindow,0);
		}
	}
	return 0;
}
static int playBackSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{	
	playbackBaterryShow(handle);
	return 0;
}
static int playBackSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playBackSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playBackOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack Open Win!!!\n");
	winSetName(handle,"play back");
	winSetResid(winItem(handle,PLAYBACK_MODE_ID),R_ID_ICON_MTPLAY2);
	#if SMALL_PANEL_SUPPORT==0
	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif

	playbackResolutionShow(handle);
	playbackFileNameShow(handle);
	playbackLockShow(handle);
	playbackMDShow(handle);
	//playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);

	//playbackPlayTimeShow(handle,0,0);
	if(managerFileCount(SysCtrl.avi_list)<1)
	{

#if 0
		if(managerFileCount(SysCtrl.wav_list) > 0)
		{
			taskStart(TASK_AUDIO_PLAY, 0);
			return 0;
		}
#endif

		uiOpenWindow(&noFileWindow,0);
	}
	return 0;
}
static int playBackCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack Close Win!!!\n");
	return 0;
}
static int playBackWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack WinChild Close!!!\n");
	if(managerFileCount(SysCtrl.avi_list)<1)
	{
		 if(managerFileCount(SysCtrl.avi_list))
        {
			taskStart(TASK_AUDIO_PLAY, 0);
        }
        else
    	{
			uiOpenWindow(&noFileWindow,0);
    	}
		return 0;
	}
	else
	{
		video_playback_start(SysCtrl.file_index); 
	}
	#if SMALL_PANEL_SUPPORT==0
	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	playbackResolutionShow(handle);
	playbackFileNameShow(handle);
	playbackLockShow(handle);
	playbackMDShow(handle);
	//playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);	
	return 0;
}
static int playBackSysMsgTimeUpdate(winHandle handle,uint32 parameNum,uint32* parame)
{
	INT32 playtime=0,totaltime = 0;
	videoPlaybackGetTime((u32*)&totaltime,(u32*)&playtime);
	if(playtime%1000 >500)
		playtime=playtime/1000;
	else
		playtime=playtime/1000;
	if(totaltime%1000 >500)
		totaltime = totaltime/1000 + 1;
	else 
		totaltime = totaltime/1000;
	playbackPlayTimeShow(handle,playtime,totaltime);
	return 0;
}

static int playBackSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
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
		playbackBaterryShow_charge(handle); 
	}
	else
	{
		playbackBaterryShow(handle);
	}

	#if SMALL_PANEL_SUPPORT==0
	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif

#if 0
    if(get_key_clock_value() == 0)
    {
        if(videoPlaybackGetStatus()== MEDIA_STAT_START)
		    videoPlaybackStop();

        deamon_screen_save(1);
        uiOpenWindow(&selfclockWindow,0);
    }
#endif
	return 0;
}

msgDealInfor playBackMsgDeal[]=
{
	{SYS_OPEN_WINDOW,playBackOpenWin},
	{SYS_CLOSE_WINDOW,playBackCloseWin},
	{SYS_CHILE_COLSE,playBackWinChildClose},
	{KEY_EVENT_OK,playBackKeyMsgOk},
	{KEY_EVENT_UP,playBackKeyMsgUp},
	{KEY_EVENT_DOWN,playBackKeyMsgDown},
	{KEY_EVENT_MENU,playBackKeyMsgMenu},
	{KEY_EVENT_MENULONG,playBackKeyLongMsgMenu},
	{KEY_EVENT_MODE,playBackKeyMsgMode},
    {KEY_EVENT_PHOTO,playBackKeyMsgPhoto},
    {KEY_EVENT_PHOTOLONG,playBackKeyMsgPhotoLong},    
    {KEY_EVENT_RECORD_PHOTO,playBackKeyMsgRecordPhoto},
	{KEY_EVENT_POWER,playBackKeyMsgPower},
	{KEY_EVENT_RECORD,playBackKeyMsgRecord},
    {KEY_EVENT_RECORDLONG,playBackKeyMsgRecordLong},
	{KEY_EVENT_AUDIO,playBackKeyMsgAudio},
	{KEY_EVENT_ESC,playBackKeyMsgEsc},
	{KEY_EVENT_UPLONG,playBackKeyMsgUpLong},
	{KEY_EVENT_DOWNLONG,playBackKeyMsgDownLong},
	{KEY_EVENT_AUDIO,playBackKeyMsgAudio},
	{SYS_EVENT_SDC,playBackSysMsgSD},
	{SYS_EVENT_USB,playBackSysMsgUSB},
	{SYS_EVENT_BAT,playBackSysMsgBattery},
	{SYS_EVENT_MD,playBackSysMsgMD},
	{SYS_EVENT_1S,playBackSysMsg1S},
	{SYS_EVENT_TIME_UPDATE,playBackSysMsgTimeUpdate},
	{EVENT_MAX,NULL},
};

WINDOW(playBackWindow,playBackMsgDeal,playBackWin)


