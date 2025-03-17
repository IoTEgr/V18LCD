#include"uiWin.h"

enum
{
	OPTION_TITLE_ID=0,
	OPTION_RECT_ID,
	OPTION_SELECT_ID,
	OPTION_BATERRY_ID,
};
static widgetCreateInfor menuOptionWin[] =
{

#if BAT_FLOW_CHARGE_EN
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(OPTION_TITLE_ID,Rx(70), Ry(30), Rw(180), Rh(32)," ",ALIGNMENT_CENTER, R_COLOR_BLUE,0),
	createRect(OPTION_RECT_ID,       Rx(70),Ry(62), Rw(180),Rh(2),R_COLOR_DBLUE),
	createItemManage(OPTION_SELECT_ID,Rx(70),Ry(74), Rw(180),Rh(142),INVALID_COLOR),
	createImageIcon(OPTION_BATERRY_ID,	   Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
#else
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(176),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(OPTION_TITLE_ID,Rx(0), Ry(0), Rw(180), Rh(32)," ",ALIGNMENT_CENTER, R_COLOR_BLUE,0),
	createRect(OPTION_RECT_ID,       Rx(0),Ry(32), Rw(180),Rh(2),R_COLOR_DBLUE),
	createItemManage(OPTION_SELECT_ID,Rx(0),Ry(34), Rw(180),Rh(142),INVALID_COLOR),
	createImageIcon(OPTION_BATERRY_ID,	   Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
#endif
	widgetEnd(),
};

#if BAT_FLOW_CHARGE_EN
static void OptionBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,OPTION_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,OPTION_BATERRY_ID),true);
}

static void OptionBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		OptionBaterryShow_charge(handle);
		//winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,OPTION_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,OPTION_BATERRY_ID),true);
}
#endif

