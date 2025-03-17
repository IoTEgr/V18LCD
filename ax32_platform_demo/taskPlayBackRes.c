#include"uiWin.h"

enum
{
	PLAYBACK_MODE_ID=0,
	PLAYBACK_HIDE_RECT_ID,
	PLAYBACK_PLAY_TIME_ID,
	PLAYBACK_RESOLUTION_ID,
	//PLAYBACK_MD_ID,// motion detect
	PLAYBACK_MONITOR_ID, // parking monitoring
	PLAYBACK_IRLED_ID,
	PLAYBACK_LOCK_ID,
	PLAYBACK_SD_ID,
	//PLAYBACK_MIC_ID,
	PLAYBACK_BATERRY_ID,
	PLAYBACK_POWERON_TIME_ID,
	PLAYBACK_FILE_NAME_ID,
	PLAYBACK_FILE_INDEX_ID,
	PLAYBACK_VOLUME_SIZE_ID,
	PLAYBACK_MAX_ID
};

#if SMALL_PANEL_SUPPORT>0
static widgetCreateInfor playBackWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createRect(PLAYBACK_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(PLAYBACK_MODE_ID,           Rx(0), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTRECORD,ALIGNMENT_LEFT),
	createStringIcon(PLAYBACK_PLAY_TIME_ID,      Rx(24), Ry(0), Rw(108), Rh(25),strToResId("00:00:00"),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createStringIcon(PLAYBACK_RESOLUTION_ID,Rx(132), Ry(0), Rw(40), Rh(25),strToResId("1M"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PLAYBACK_MD_ID,                Rx(172), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_MONITOR_ID,     Rx(192), Ry(0), Rw(22), Rh(25), /*R_ID_ICON_MTPARKOFF*/R_ID_ICON_MENUVOLUME2,ALIGNMENT_CENTER),
	createStringIcon(PLAYBACK_VOLUME_SIZE_ID,Rx(215), Ry(0), Rw(12), Rh(25),strToResId("5"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PLAYBACK_IRLED_ID,             Rx(214),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_LOCK_ID,              Rx(236),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),

	//createImageIcon(PLAYBACK_SD_ID,                  Rx(258), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_MIC_ID,                Rx(280), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_BATERRY_ID,       Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(PLAYBACK_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_NAME_ID,    Rx(5), Ry(25), Rw(160), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_INDEX_ID,   Rx(170), Ry(25), Rw(120), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	widgetEnd(),
};
#else
static widgetCreateInfor playBackWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createRect(PLAYBACK_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(PLAYBACK_MODE_ID,           Rx(5), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTRECORD,ALIGNMENT_LEFT),
	createStringIcon(PLAYBACK_PLAY_TIME_ID,      Rx(25), Ry(0), Rw(75), Rh(25),strToResId("00:00:00"),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createStringIcon(PLAYBACK_RESOLUTION_ID,Rx(100), Ry(0), Rw(30), Rh(25),strToResId("1M"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PLAYBACK_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_LOCK_ID,              Rx(190),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),

	createImageIcon(PLAYBACK_SD_ID,                  Rx(210), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_BATERRY_ID,       Rx(250), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringIcon(PLAYBACK_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_NAME_ID,    Rx(5), Ry(25), Rw(120), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_INDEX_ID,   Rx(120), Ry(25), Rw(80), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	widgetEnd(),
};
#endif

static void playbackPlayTimeShow(winHandle handle,INT32U playSeconde,INT32U totalSecode)
{
	static char TimeStr[9];
	//static INT32U LastSeconde=0;
	INT32U Minutes,secodes;
	if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
	{
		secodes = playSeconde;
		winSetStrInfor(winItem(handle,PLAYBACK_PLAY_TIME_ID),0,ALIGNMENT_CENTER,R_COLOR_RED);
	}
	else
	{
		winSetStrInfor(winItem(handle,PLAYBACK_PLAY_TIME_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
		secodes = totalSecode;
	}
	//if(LastSeconde==secodes)
	//	return;
	//LastSeconde=secodes;
	Minutes=secodes/60;
	secodes=secodes%60;
	TimeStr[0]='0';
	TimeStr[1]='0';
	TimeStr[2]=':';
	TimeStr[3]=(Minutes/10)+'0';
	TimeStr[4]=(Minutes%10)+'0';
	TimeStr[5]=':';
	TimeStr[6]=(secodes/10)+'0';
	TimeStr[7]=(secodes%10)+'0';
	TimeStr[8]=0;
	winSetResid(winItem(handle,PLAYBACK_PLAY_TIME_ID),strToResId(TimeStr));
	if(SysCtrl.file_type == FILE_TYPE_JPG)
	{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
	}
	else{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),true);
	}
}

static void playbackResolutionShow(winHandle handle)
{
	INT16U width=0,height=0;
	if(SysCtrl.file_type==FILE_TYPE_JPG)
	{
		imageDecodeGetResolution(&width,&height);

		if(height==480)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
		else if(height==720||height==960)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("1M"));
		else if(height==1080)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("2M"));
		else if(height==1440)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("3M"));
		else if(height==1800)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("5M"));
		else if(height==2160)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("8M"));
		else if(height==2520)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("10M"));
		else if(height==2880)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("12M"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
	}
	else if(SysCtrl.file_type==FILE_TYPE_AVI)
	{
		height=videoPlaybackGetResolution()->height;
		if(height==480)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
		else if(height==720)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("HD"));
		else if(height==1080)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("FHD"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
	}
}
static void playbackMDShow(winHandle handle)
{
	/*if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,PLAYBACK_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),false);*/
}
static void playbackMonitorShow(winHandle handle)
{
	/*if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKOFF);*/
}
static void playbackIrLedShow(winHandle handle)
{
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PLAYBACK_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,PLAYBACK_IRLED_ID),false);
#endif
}
static void playbackLockShow(winHandle handle)
{
#if 0
	if(managerCheckLockExt(SysCtrl.avi_list,SysCtrl.file_index)>0)
	{
		winSetResid(winItem(handle,PLAYBACK_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),false);
#endif
}
static void playbackSDShow(winHandle handle)
{
    return 0;
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void playbackMicShow(winHandle handle)
{
	/*if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICON);*/
}


static void playbackBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),true);
}

static void playbackBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		playbackBaterryShow_charge(handle);
		//winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),true);
}

static void playbackFileNameShow(winHandle handle)
{
	int idx;
	static char indexStr[10];
	char* name=manangerGetFileName(SysCtrl.avi_list,SysCtrl.file_index);
	if(name)
		winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(name));
	else
		winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(" "));
	idx = SysCtrl.file_index;
	indexStr[0] = ((idx/1000)%10)+'0'; 
	indexStr[1] = ((idx/100)%10)+'0';
	indexStr[2] = ((idx/10)%10)+'0';
	indexStr[3] = ((idx)%10)+'0';
	indexStr[4] = '/';
	idx = managerFileCount(SysCtrl.avi_list);
	indexStr[5] = ((idx/1000)%10)+'0'; 
	indexStr[6] = ((idx/100)%10)+'0';
	indexStr[7] = ((idx/10)%10)+'0';
	indexStr[8] = ((idx)%10)+'0';
	indexStr[9]= 0;
	if(idx>0)
		winSetResid(winItem(handle,PLAYBACK_FILE_INDEX_ID),strToResId(indexStr));
}
#if SMALL_PANEL_SUPPORT==0
static void playbackPoweOnTimeShow(winHandle handle,uint32 sec)
{
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,PLAYBACK_POWERON_TIME_ID),strToResId(powerOnTimeStr));
}
#endif
