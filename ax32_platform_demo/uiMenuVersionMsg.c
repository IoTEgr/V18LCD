#include "uiMenuVersionRes.c"
#if SMALL_PANEL_SUPPORT>0
static uint32 getVersionResInfor(uint32 item,uint32* image,uint32* str)
{
/*
	if(item==0)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_OK;
	}
	else if(item==1)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_CANCEL;
	}
*/
	return 0;
}

static int versionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}

static int versionKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int versionKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}

static int versionKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	deamon_clock_check(1);
	deamon_auto_poweroff(1);
	//deamon_screen_save(1);
	 if(get_suspended_lcd_value())
        {
            return 0;
        }
	
	return versionKeyMsgOk(handle, parameNum, parame);
}

static int versionKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	return versionKeyMsgMenu(handle, parameNum, parame);
}


#if BAT_FLOW_CHARGE_EN
static int versionKeyMsg1s(winHandle handle,uint32 parameNum,uint32* parame)
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
		VersionBaterryShow_charge(handle);
	}

	return 0;
}
#endif

static int versionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("version Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,VERSION_TIPS_ID),1,R1h(32));
	itemManageSetColumnSumWithGap(winItem(handle,VERSION_TIPS_ID),0,2,R1w(50),R1w(12));
#if BAT_FLOW_CHARGE_EN
	VersionBaterryShow(handle);
#endif
	return 0;
}
static int versionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("version Close Win!!!\n");
	return 0;
}
static int versionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("version WinChild Close!!!\n");
	return 0;
}


msgDealInfor verisonMsgDeal[]=
{
	{SYS_OPEN_WINDOW,versionOpenWin},
	{SYS_CLOSE_WINDOW,versionCloseWin},
	{SYS_CHILE_COLSE,versionWinChildClose},
	{KEY_EVENT_OK,versionKeyMsgOk},
	//{KEY_EVENT_MENU,versionKeyMsgMenu},
	{KEY_EVENT_MODE,versionKeyMsgMode},
	{KEY_EVENT_POWER,versionKeyMsgPower},
	{KEY_EVENT_ESC,versionKeyMsgEsc},
#if BAT_FLOW_CHARGE_EN
	{SYS_EVENT_1S,versionKeyMsg1s},
#endif
	{EVENT_MAX,NULL},
};

WINDOW(versionWindow,verisonMsgDeal,versionWin)

#endif
