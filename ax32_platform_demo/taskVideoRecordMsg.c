#include "application.h"
#include "taskVideoRecordRes.c"

static uint32 mdRecTime=0;
static uint8 powerkeypresflag=0;
static uiRect recordTimeRect;
static bool curWinIsVideo=false;
static bool firstPowerOnRec=false; //true;

extern INT32U g_md_delay_times;

extern uint8 g_clock_flag;
extern INT32U g_clock_times;

static int videoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
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

		if(videoRecordGetStatus() == MEDIA_STAT_START)
		{
			//task_record_stop();//20230107
		}
		else
		{
			if(SysCtrl.battery > BATTERY_STAT_0)
			{
				video_record_start();

				if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
				{
					mdRecTime=XOSTimeGet();
				}
				else
				{
					mdRecTime=0;
				}
				
			}
			
		}
	}
	return 0;
}
static int videoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

#if _DEBG_CUT_SDR_DATA_
		void sdram_data_capture_task(u8 *fname);
		sdram_data_capture_task(NULL);
		debg("cut raw finish\n");
#endif
		if(SysCtrl.usensorshow == USENSOR_SHOW_ON)
		{
            if(SysCtrl.pip_mode == 0)
                SysCtrl.pip_mode = DISP_PIP_MAX;
            SysCtrl.pip_mode--;
            dispLayerSetPIPMode(SysCtrl.pip_mode);
		}
		else
		{
			#if (1 == LCD_CROP_SCALE)
			/*if(SysCtrl.pip_mode == 0)
			{
				if(SysCtrl.crop_level<6)
					SysCtrl.crop_level++;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
			}*/
			#endif
		}
	}
	return 0;
}
static int videoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_ON)
		{
			configSet(CONFIG_ID_AUDIOREC,R_ID_STR_COM_OFF);
			videoRecordCmdSet(CMD_COM_AUDIOEN,0);
			hal_adc_volume_set(0);
			hal_adc_volume_setB(0);
			//add: lwq  2018.3.24
			mediaVideoCtrl.arg.audio = 0 ;
			mediaVideoCtrlB.arg.audio = 0 ; 
		}
        else
        {
			configSet(CONFIG_ID_AUDIOREC,R_ID_STR_COM_ON);
			videoRecordCmdSet(CMD_COM_AUDIOEN,1);
			hal_adc_volume_set(100);
			#if USENSOR_AUDIO_ON
				hal_adc_volume_setB(100);
			#endif
			//add: lwq  2018.3.24
			mediaVideoCtrl.arg.audio = 1 ;
			mediaVideoCtrlB.arg.audio = 1 ;
        }
		videoMicShow(handle);

	#if (1 == LCD_CROP_SCALE)
		//== lcd scale==
	/*	if(SysCtrl.pip_mode == DISP_PIP_FRONT)
		{
			if(SysCtrl.crop_level>0)
				SysCtrl.crop_level--;
			dispLayerSetFrontCrop(SysCtrl.crop_level);
		} */
	#endif
	}
	return 0;
}

static int videoKeyMsgUpLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(videoRecordGetStatus() == MEDIA_STAT_STOP )
		{
			INT32U value = configValue2Int(CONFIG_ID_SENSOR);
			if( value == 0)
			{
				configSet(CONFIG_ID_SENSOR,R_ID_STR_COM_ON);
				//sensor_rotate_adapt(3);
				 if( devSensorInit->id == 0x48 )  //H63P
			 	{
					sensor_rotate_adapt(3);
					//isp_init();
					isp_LSC_adj_cfg((u16*)lsc_tab,1);
			 	}
				else  if( devSensorInit->id == 0x63 )  //H63
				{
					sensor_rotate_adapt(3);
				}
			}
			else
			{
				configSet(CONFIG_ID_SENSOR,R_ID_STR_COM_OFF);
				//sensor_rotate_adapt(2);
				if( devSensorInit->id == 0x48 )  //H63P
			 	{
					sensor_rotate_adapt(2);
					//isp_init();
					isp_LSC_adj_cfg((u16*)lsc_tab,1);
			 	}
				else  if( devSensorInit->id == 0x63 )  //H63
				{
					sensor_rotate_adapt(2);
				}
			}
		}
	}
	return 0;
}

static int videoKeyMsgDownLong(winHandle handle,uint32 parameNum,uint32* parame)
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

extern MENU(movie);
static int videoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	#if 0
        if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
        {
            if(videoRecordGetStatus() == MEDIA_STAT_START)
            {
                task_record_stop();
            }
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
        }
		//if(videoRecordGetStatus() != MEDIA_STAT_START)
        else if(videoRecordGetStatus() != MEDIA_STAT_START)
		{
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
		else
		{
			 if(windowIsOpen(&selfclockWindow) == 0)
	        {
	            uiOpenWindow(&selfclockWindow,0);
	        }
		}

	#else
		if(videoRecordGetStatus() != MEDIA_STAT_START)
		{
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
		else
		{
		
		}

	#endif
	}
	
	return 0;
}

static int videoKeyMsgMode_Audio(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoRecordGetStatus() != MEDIA_STAT_START)
		{
			task_record_stop();
		}

		if(windowIsOpen(&selfclockWindow))
		{
			winDestroy(&handle);
		}

		taskStart(TASK_AUDIO_RECORD,0);
		XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_AUDIO, KEY_PRESSED));
	}
	return 0;
}

static int videoKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

    return videoKeyMsgMode_Audio(handle, parameNum, parame);

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoRecordGetStatus() != MEDIA_STAT_START)
			taskChange();
		else  // lock this file
		{
			SysCtrl.g_lock = 1;
			videoLockShow(handle);
		}
	}
	return 0;
}

static int videoKeyMsgMode_L40Play(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoRecordGetStatus() != MEDIA_STAT_START)
		{
			taskStart(TASK_PLAY_BACK,0);
		}
	}
	return 0;
}

static int videoKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{

	deamon_clock_check(1);
	deamon_auto_poweroff(1);
	//deamon_screen_save(1);

	 if(get_suspended_lcd_value())
    {
        return 0;
    }

	if( powerkeypresflag )
	{
		powerkeypresflag = 0;
		return 0;
	}

	if(SysCtrl.sdcard!=SDC_STAT_NORMAL)
	{
		XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
		return 0;
	}

	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if(mdRecTime && ( videoRecordGetStatus() == MEDIA_STAT_START ) )
		{
			task_record_stop();
			mdRecTime=0;
			powerkeypresflag=1;
			g_md_delay_times=XOSTimeGet();
			return 0;
		}
	}

	if((SysCtrl.usb == USB_STAT_PC)) 
	{
		SysCtrl.usb = USB_STAT_NULL;
		taskStart(TASK_VIDEO_RECORD,0);
		return 0;
	}
	
	 if(videoRecordGetStatus() != MEDIA_STAT_START)
    {
        taskStart(TASK_PLAY_BACK,0);
    }

    return 0;
	
}

static int videoKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	//return videoKeyMsgMode_Audio(handle, parameNum, parame);
	return videoKeyMsgOk(handle, parameNum, parame);
}

static int videoKeyMsgSave(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoRecordGetStatus() == MEDIA_STAT_START)
			task_record_stop();
	#if 1
		if(windowIsOpen(&selfclockWindow))
		{
			winDestroy(&handle);
		}
	#endif
	}

	return 0;
}


extern int Photo_flag;
static int videoKeyMsgPhotoLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if((Photo_flag == 0) && (videoRecordGetStatus() != MEDIA_STAT_START)) 
		{
			taskStart(TASK_PHOTO_ENCODE,0);
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_ESC, KEY_PRESSED));
		}
	}

	return 0;
}

static int videoKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        if(videoRecordGetStatus() != MEDIA_STAT_START)
        {
            task_record_stop();
        }
        taskStart(TASK_PLAY_BACK,0);
	}

	return 0;
}

static int videoKeyMsgRecordLong(winHandle handle,uint32 parameNum,uint32* parame)
{
	return videoKeyMsgPhotoLong(handle, parameNum, parame);


	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		 if(get_suspended_lcd_value())
        {
            return 0;
        }
		 
		if(videoRecordGetStatus() != MEDIA_STAT_STOP)
		{
			//task_record_stop();
		}
       // deamon_screen_save(1);
        //uiOpenWindow(&selfclockWindow,0);
	}

	return 0;
}

static int videoKeyMsgRecordPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	return videoKeyMsgPhotoLong(handle, parameNum, parame);
}

static int videoKeyMsgAudio(winHandle handle,uint32 parameNum,uint32* parame)
{
	return videoKeyMsgMode_Audio(handle, parameNum, parame);
}

static int videoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard != SDC_STAT_NORMAL && videoRecordGetStatus() == MEDIA_STAT_START) // sdc out when recording
	{
		task_record_stop();
	}
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	videoSDShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}
static int videoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoBaterryShow(handle);
	return 0;
}

static int videoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		videoBaterryShow(handle);

#if 0
	if(SysCtrl.battery <= BATTERY_STAT_2)
	{
		if(backrec_adpt())
		{
			task_record_stop();
			videoRemainTimeShow(handle, SysCtrl.t_remain);
		}
	}
#else

	/*if(SysCtrl.battery <= BATTERY_STAT_0)
	{
		if(videoRecordGetStatus() == MEDIA_STAT_START)
		{
			task_record_stop();
			videoRemainTimeShow(handle, SysCtrl.t_remain);
		}
	}*/
#endif
	return 0;
}
static int videoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	 if(!hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN))
 	{
		return 0;
 	}
	if(videoRecordGetStatus() != MEDIA_STAT_START)
	{
		video_record_start();
	}
	mdRecTime=XOSTimeGet();
	return 0;
}
static int videoSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("!!!\n");
	if(videoRecordGetStatus() != MEDIA_STAT_START)
		video_record_start();
	return 0;
}
extern uint32 videoRecordGetTimeSec(void);
extern void drawUIService1(void);
extern XWork_T *  recordIncrease1S;
void recordTimeCount1S(void)
{
	static char recTimeStr[]="00:00";
	uint32 sec,frameCntTime;
	if(videoRecordGetStatus() == MEDIA_STAT_START)
	{
		frameCntTime=videoRecordGetTimeSec();
		if(SysCtrl.t_rec<frameCntTime)
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=900*X_TICK;
		}
		else if(SysCtrl.t_rec>frameCntTime)
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=1100*X_TICK;
		}
		else
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=1000*X_TICK;
		}
		sec=SysCtrl.t_rec;
		SysCtrl.t_rec++;
		if(SysCtrl.t_rec>SysCtrl.t_mrec)
			SysCtrl.t_rec=0;
		if(curWinIsVideo)
		{
			sec=sec%3600;
			recTimeStr[0] = sec/600+'0';  // m
			recTimeStr[1] = ((sec/60)%10)+'0';	 // m
			recTimeStr[2] = ':';
			sec=sec%60;
			recTimeStr[3] = sec/10+'0';  // s
			recTimeStr[4] = sec%10+'0';   // s
			recTimeStr[5] = 0;
			//deg_Printf("rec:%s\n",recTimeStr);
			drawString(&recordTimeRect,&recordTimeRect,strToResId(recTimeStr),R_FONT_DEFAULT,ALIGNMENT_LEFT,R_COLOR_RED,R_COLOR_BLACK);
			drawUIService1();
		}
	}
}

static int videoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video Open Win!!!%d %d\n",SysCtrl.sdcard,SysCtrl.usb);
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
    {
        boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);
        uiOpenWindow(&selfclockWindow,0);
        return 0;
    }
#endif
	layout_version_get();
	winSetResid(winItem(handle,VIDEO_MODE_ID),R_ID_ICON_MTRECORD);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	#if SMALL_PANEL_SUPPORT==0
	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
	if(firstPowerOnRec)
	{
		if(0)//if(SysCtrl.sdcard == SDC_STAT_NORMAL&&SysCtrl.battery > BATTERY_STAT_2)
		{
			deg_Printf("power on,start record\n");
			if(SysCtrl.usensor==USENSOR_STAT_NULL||(SysCtrl.usensor!=USENSOR_STAT_NULL&&api_backrec_status()))
			{
				firstPowerOnRec=false;
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_ON,0));
			}
		}
		else
		{
			firstPowerOnRec=false;
		}
	}
	winGetPos(winItem(handle,VIDEO_REC_TIME_ID),&recordTimeRect);
	curWinIsVideo=true;

	winSetVisible(winItem(handle,VIDEO_SD_ID),false);
	return 0;
}
static int videoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video Close Win!!!\n");
	curWinIsVideo=false;
	return 0;
}
static int videoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	curWinIsVideo=true;
	deg_Printf("video WinChild Close!!!\n");
	if(videoRecordGetStatus() != MEDIA_STAT_START)
		videoRemainTimeShow(handle, SysCtrl.t_remain);
	#if SMALL_PANEL_SUPPORT==0
	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
	return 0;
}
static int videoWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video WinChild Open!!!\n");
	curWinIsVideo=false;
	return 0;
}
static int videoRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	return 0;
}
static int videoRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	//SysCtrl.t_rec = 0;
	videoLockShow(handle);
	videoRecTimeShow(handle, 0);
	drawUIService(true);
	return 0;
}
static int videoRecordResume1(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	return 0;
}

static int videoRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	return 0;
}
static int videoRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
	SysCtrl.g_lock = 1;
	videoLockShow(handle);
	return 0;
}

static int videoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static uint32 flag=0;

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
		videoBaterryShow_charge(handle); 
	}
	else
	{
		videoBaterryShow(handle);
	}

	if(firstPowerOnRec)
	{
		if(SysCtrl.usensor==USENSOR_STAT_NULL||(SysCtrl.usensor!=USENSOR_STAT_NULL&&api_backrec_status()))
		{
			firstPowerOnRec=false;
			XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_ON,0));
		}
	}
	winSetResid(winItem(handle,VIDEO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	#if SMALL_PANEL_SUPPORT==0
	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	videoIrLedShow(handle);
	/*if(SysCtrl.usb == USB_STAT_DCIN)
	{
		if(winIsVisible(winItem(handle,VIDEO_BATERRY_ID)))
			winSetVisible(winItem(handle,VIDEO_BATERRY_ID),false);
		else
		{
			winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
			winSetVisible(winItem(handle,VIDEO_BATERRY_ID),true);
		}	
	}*/
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if(mdRecTime&&XOSTimeGet()-mdRecTime>=MOTION_DEC_TIME*1000)
		{
			task_record_stop();
			mdRecTime=0;
		}
	}
	if(videoRecordGetStatus() == MEDIA_STAT_START)
	{
		if(flag&1)
			boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
		else
			boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
		flag++;
	}
	if(winIsVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID)))
		winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);

#if 0
    if(get_key_clock_value() == 0)
    {
		//if(videoRecordGetStatus() == MEDIA_STAT_START)
		//	task_record_stop();

        deamon_screen_save(1);
        uiOpenWindow(&selfclockWindow,0);
    }
#endif

	if(videoRecordGetStatus() == MEDIA_STAT_START)
	{
		//deg_Printf("video : fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
		if((!winIsVisible(winItem(handle,VIDEO_SD_ID))) && ( (SysCtrl.sd_freesize>>20) < 1 ))
		{
			winSetVisible(winItem(handle,VIDEO_SD_ID),true);
		}
	}

	return 0;
}

msgDealInfor videoRecordeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,videoOpenWin},
	{SYS_CLOSE_WINDOW,videoCloseWin},
	{SYS_CHILE_COLSE,videoWinChildClose},
	{SYS_CHILE_OPEN,videoWinChildOpen},
	{KEY_EVENT_OK,videoKeyMsgOk},
	{KEY_EVENT_UP,videoKeyMsgUp},
	//{KEY_EVENT_DOWN,videoKeyMsgDown},
	{KEY_EVENT_UPLONG,videoKeyMsgUpLong},
	//{KEY_EVENT_DOWNLONG,videoKeyMsgDownLong},
	{KEY_EVENT_RECORD,videoKeyMsgRecord},
	//{KEY_EVENT_RECORDLONG,videoKeyMsgRecordLong},
    //{KEY_EVENT_RECORD_PHOTO,videoKeyMsgRecordPhoto},
    {/*KEY_EVENT_PHOTO*/KEY_EVENT_MENU,videoKeyMsgRecordPhoto},
	//{KEY_EVENT_PHOTOLONG,videoKeyMsgPhotoLong},
	{KEY_EVENT_MENULONG,videoKeyMsgMenu},

	{KEY_EVENT_DOWNLONG,videoKeyMsgDownLong},

	{KEY_EVENT_SAVE,videoKeyMsgSave},
	
	{KEY_EVENT_MODE,videoKeyMsgMode},
	{KEY_EVENT_AUDIO,videoKeyMsgAudio},
	{KEY_EVENT_POWER,videoKeyMsgPower},
	{SYS_EVENT_SDC,videoSysMsgSD},
	{SYS_EVENT_USB,videoSysMsgUSB},
	{SYS_EVENT_BAT,videoSysMsgBattery},
	{SYS_EVENT_MD,videoSysMsgMD},
	{SYS_EVENT_ON,videoSysMsgRestart},
	{VID_EVENT_REC_STOP,videoRecordStopped},
	{VID_EVENT_REC_START,videoRecordStarted},
	{VID_EVENT_REC_RESUME,videoRecordResume1},
	{VID_EVENT_REC_ERROR,videoRecordErr},
	{VID_EVENT_LOCK_FILE,videoRecordLockFile},
	{SYS_EVENT_1S,videoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(videoRecordWindow,videoRecordeMsgDeal,videoRecordeWin)

bool cut_sdr_data_en = 0;
u32 cut_sdr_data_addr = 0;
#if _DEBG_CUT_SDR_DATA_
typedef struct 
{
	void * sdr_data_buff;
	u32 len;
}SDR_DATA_MSG;
SDR_DATA_MSG sdr_data;
/*******************************************************************************
* Function Name  : sdram_data_capture_cfg
* Description    : config for capture sdram_data
* Input          : void *sdr_data_buff
* Output         : None
* Return         : None
*******************************************************************************/
void sdram_data_capture_cfg(void *sdr_data_buff)
{
	//dvp
	void hal_Cut_SDR_SrcCSI(void *sdr_buf,bool csi_save);
#if (_DEBG_CUT_SDR_DATA_&0xf0) == _DEBG_CUT_BEFOR_CSI_
	hal_Cut_SDR_SrcCSI(sdr_data_buff,1);
#elif _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_POST_YUV_
	hal_Cut_SDR_SrcCSI(sdr_data_buff,0);
#endif
}
/*******************************************************************************
* Function Name  : sdram_data_capture_task
* Description    : capture sdram data 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void sdram_data_capture_task(u8 *fname)
{
	ax32xx_csiEnable(0);
	hal_mjpegEncodeUninit();
	//capture rawdata
	_Sensor_Adpt_ *senradpt = hal_csiAdptGet();
#if _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_RAW_
	debg("capture rawdata start\n");
	char filename[18] = "DEBG/RAWD0000.raw";
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 2;
	sdr_data.sdr_data_buff = hal_sysMemMalloc(senradpt->pixelh*senradpt->pixelw*2,64);
#elif _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_YUV_
	debg("capture csi yuv422 data start\n");
	char filename[18] = "DEBG/YUV80000.raw";
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 2;
#else
	debg("capture post yuv420 data start\n");
	char filename[18] = "DEBG/YUV60000.yuv";
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 3 / 2;
	#if _DEBG_CUT_SDR_DATA_  ==  _DEBG_CUT_POST_YUV_INAVI_    
		sdr_data.sdr_data_buff = (void * )cut_sdr_data_addr;
	#elif _DEBG_CUT_SDR_DATA_  ==  _DEBG_CUT_POST_YUV_LCD_
		sdr_data.sdr_data_buff = (void * )cut_sdr_data_addr;
		sdr_data.len = 480*272*3/2;
	#endif
#endif
	if(sdr_data.sdr_data_buff ==NULL){
		debg("sdr_data_buff  apply fail \n");
		return;
	}
	sdram_data_capture_cfg(sdr_data.sdr_data_buff);
	//msg deal
	FIL Fil;
	FRESULT ret = f_mkdir("DEBG\0");
	if(ret != FR_OK){	
		if(ret != FR_EXIST){
			debg("DEBG mkdir err\n");
			return;
		}
	}
	u32 filenum ,loop = 1000000;
	debg("CSIMODE:%x CSIPMODE2:%x CSI_DMA_MODE:%x ",CSIMODE,ISPMODE,CSI_DMA_MODE);
	while(loop--){
		hal_wdtClear();
		if(cut_sdr_data_en == 1){
			debg("capture sdrdata pending\n");
			cut_sdr_data_en = 0;
			uint write;
			for(filenum = 0;filenum < 1000;filenum++){
				filename[9]='0'+ ((filenum / 1000)%10);
				filename[10]='0'+ ((filenum / 100)%10);
				filename[11]='0'+ ((filenum / 10)%10);
				filename[12]='0'+ (filenum  % 10);
				if(FR_OK != f_open(&Fil,(char const *)filename, FA_READ)){
					if(FR_OK == f_open(&Fil,(char const *)filename, FA_CREATE_NEW | FA_WRITE)){
						f_write(&Fil, (const void*)sdr_data.sdr_data_buff, sdr_data.len, &write);
						debg("Raw len = %x\n",write);
						loop = 0;
						hal_sysMemFree(sdr_data.sdr_data_buff);
						memcpy(fname,filename,sizeof(filename));
						hal_mjpegMemUninit();//mjpegEncCtrl.ybuffer = 0;
					}
					f_close(&Fil);
					break;
				}
				f_close(&Fil);
			}		
		}
	}
	
}
#endif


