#include"uiWin.h"
#if SMALL_PANEL_SUPPORT>0
enum
{
	VERSION_TIPS_ID=0,
	VERSION_LOG1_ID,
	VERSION_LOG2_ID,
#if BAT_FLOW_CHARGE_EN
	VERSION_BATERRY_ID,
#endif
};
static widgetCreateInfor versionWin[] =
{
#if BAT_FLOW_CHARGE_EN
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(VERSION_TIPS_ID,Rx(70),Ry(10), Rw(180),Rh(40),R_ID_STR_SET_VERSION,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(VERSION_LOG1_ID,Rx(70),Ry(50), Rw(180),Rh(40),"AX3292_CAR",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(VERSION_LOG2_ID,Rx(70),Ry(90), Rw(180),Rh(40),"20220310",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createImageIcon(VERSION_BATERRY_ID,	   Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
#else
	createFrameWin(Rx(70),Ry(50), Rw(180),Rh(140),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(VERSION_TIPS_ID,Rx(0),Ry(10), Rw(180),Rh(40),R_ID_STR_SET_VERSION,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(VERSION_LOG1_ID,Rx(0),Ry(50), Rw(180),Rh(40),"AX3292_CAR",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(VERSION_LOG2_ID,Rx(0),Ry(50), Rw(180),Rh(40),"20220310",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
#endif
	widgetEnd(),
};
#endif


#if BAT_FLOW_CHARGE_EN

static void VersionBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,VERSION_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,VERSION_BATERRY_ID),true);
}

static void VersionBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		VersionBaterryShow_charge(handle);
		//winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,VERSION_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,VERSION_BATERRY_ID),true);
}
#endif


