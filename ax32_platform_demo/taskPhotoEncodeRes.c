#include"uiWin.h"

enum
{
	PHOTO_MODE_ID=0,
	PHOTO_HIDE_RECT_ID,
	PHOTO_PIC_REMAIN_ID,
	PHOTO_RESOLUTION_ID,
	PHOTO_MD_ID,// motion detect
	PHOTO_MONITOR_ID, // parking monitoring
	PHOTO_IRLED_ID,
	PHOTO_LOCK_ID,
	PHOTO_SD_ID,
	PHOTO_MIC_ID,
	PHOTO_BATERRY_ID,
	PHOTO_POWERON_TIME_ID,
	PHOTO_SYSTIME_ID,
    PHOTO_SHOW_ID,
	PHOTO_MAX_ID
};

#if SMALL_PANEL_SUPPORT>0
static widgetCreateInfor photoEncodeWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createRect(PHOTO_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(PHOTO_MODE_ID,           Rx(10), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTRECORD,ALIGNMENT_LEFT),
	createStringIcon(PHOTO_PIC_REMAIN_ID,      Rx(24), Ry(0), Rw(108), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_RESOLUTION_ID,Rx(50), Ry(0), Rw(40), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PHOTO_MD_ID,                Rx(172), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_MONITOR_ID,     Rx(192), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_IRLED_ID,             Rx(214),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_LOCK_ID,              Rx(236),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_SD_ID,                  Rx(258), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_MIC_ID,                Rx(280), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_BATERRY_ID,       Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(PHOTO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_SYSTIME_ID,        Rx(5), Ry(215), Rw(300), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
    createStringIcon(PHOTO_SHOW_ID,        Rx(160), Ry(120), Rw(80), Rh(80),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
    createImageIcon(PHOTO_SHOW_ID,       Rx(120), Ry(80), Rw(100), Rh(100), R_ID_ICON_PICTURE,ALIGNMENT_CENTER),

	widgetEnd(),
};
#else
static widgetCreateInfor photoEncodeWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createRect(PHOTO_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(PHOTO_MODE_ID,           Rx(5), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTRECORD,ALIGNMENT_LEFT),
	createStringIcon(PHOTO_PIC_REMAIN_ID,      Rx(25), Ry(0), Rw(75), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_RESOLUTION_ID,Rx(100), Ry(0), Rw(30), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createImageIcon(PHOTO_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_LOCK_ID,              Rx(190),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),

	createImageIcon(PHOTO_SD_ID,                  Rx(210), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_BATERRY_ID,       Rx(250), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringIcon(PHOTO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_SYSTIME_ID,        Rx(5), Ry(215), Rw(220), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	widgetEnd(),
};
#endif

static void photoResolutionShow(winHandle handle)
{
	if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_1M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("1M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_2M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("2M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_3M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("3M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_5M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("5M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_8M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("8M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_10M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("10M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_12M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("12M"));
	else 
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("???"));
}
static void photoMDShow(winHandle handle)
{
    return 0;
}
static void photoMonitorShow(winHandle handle)
{
    return 0;
	/*if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PHOTO_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PHOTO_MONITOR_ID),R_ID_ICON_MTPARKOFF);*/
}
static void photoIrLedShow(winHandle handle)
{
    return 0;
#if (1 == IR_MENU_EN)
	/*if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIRON);*/
#else
	winSetVisible(winItem(handle,PHOTO_IRLED_ID),false);
#endif
}
static void photoLockShow(winHandle handle,bool show)
{
	/*if(show)
	{
		winSetResid(winItem(handle,PHOTO_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PHOTO_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PHOTO_LOCK_ID),false);*/
}
static void photoSDShow(winHandle handle)
{
    return 0;
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void photoMicShow(winHandle handle)
{
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PHOTO_MIC_ID),R_ID_ICON_MTMICON);
}

static void photoPictureShow(winHandle handle)
{
	winSetResid(winItem(handle,PHOTO_SHOW_ID), R_ID_ICON_PICTURE);
}

static void photoBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,PHOTO_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,PHOTO_BATERRY_ID),true);
}

static void photoBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		photoBaterryShow_charge(handle);
		//winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,PHOTO_BATERRY_ID),true);
}

static void photoRemainNumShow(winHandle handle)
{
    winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId(" "));
}

#if SMALL_PANEL_SUPPORT==0
static void photoPoweOnTimeShow(winHandle handle,uint32 sec)
{
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,PHOTO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
}
#endif

