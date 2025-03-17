#include"uiWin.h"

enum
{
	FORMAT_TIPS_ID=0,
	FORMAT_SELECT_ID,
#if BAT_FLOW_CHARGE_EN
	FORMAT_BATERRY_ID
#endif
};
static widgetCreateInfor formatWin[] =
{
	#if SMALL_PANEL_SUPPORT==0
#if BAT_FLOW_CHARGE_EN
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(FORMAT_TIPS_ID,Rx(70),Ry(0), Rw(180),Rh(100),R_ID_STR_FMT_FORMAT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(FORMAT_SELECT_ID,Rx(70),Ry(100), Rw(180),Rh(40),INVALID_COLOR),
#else
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(142),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(FORMAT_TIPS_ID,Rx(0),Ry(0), Rw(180),Rh(100),R_ID_STR_FMT_FORMAT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(FORMAT_SELECT_ID,Rx(0),Ry(100), Rw(180),Rh(40),INVALID_COLOR),
#endif
	#else
	createFrameWin(Rx(30),Ry(50), Rw(260),Rh(140),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(FORMAT_TIPS_ID,Rx(0),Ry(0), Rw(260),Rh(100),R_ID_STR_FMT_FORMAT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(FORMAT_SELECT_ID,Rx(0),Ry(100), Rw(260),Rh(40),INVALID_COLOR),
	#endif
	widgetEnd(),
};

#if BAT_FLOW_CHARGE_EN
static void formatBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,FORMAT_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,FORMAT_BATERRY_ID),true);
}

static void formatBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		formatBaterryShow_charge(handle);
		//winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,FORMAT_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,FORMAT_BATERRY_ID),true);
}
#endif


