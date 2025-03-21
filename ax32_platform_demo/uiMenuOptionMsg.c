#include "uiMenuOptionRes.c"

static menuItem* currentItem=NULL;
static uint32 getOptionResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=currentItem->pOption[item].image;
	if(str)
		*str=currentItem->pOption[item].str;
}

static int menuOptionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(currentItem)
		{
			configSet(currentItem->configId,currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str);
			configSys(currentItem->configId);
			userConfigSave();
		}
		winDestroy(&handle);
	}
	return 0;
}

static int menuOptionKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,OPTION_SELECT_ID));
	}
	return 0;
}
static int menuOptionKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,OPTION_SELECT_ID));
	}
	return 0;
}
static int menuOptionKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int menuOptionKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskChange();
	}
	return 0;
}

static int menuOptionKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	deamon_clock_check(1);
	deamon_auto_poweroff(1);
	//deamon_screen_save(1);
	 if(get_suspended_lcd_value())
        {
            return 0;
        }
	
	return menuOptionKeyMsgOk(handle, parameNum, parame);
}

static int menuOptionKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	return menuOptionKeyMsgMenu(handle, parameNum, parame);
}

static int menuOptionSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int menuOptionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 itemNum=0,config,i;
	if(parameNum<1)
	{
		winDestroy(&handle);
		return 0;
	}
	currentItem=(menu*)parame[0];
	deg_Printf("menuOption Open Win!!!\n");
	itemNum=itemManageSetHeightAvgGap(winItem(handle,OPTION_SELECT_ID),R1h(32));
	//for(i=0;i<itemNum;i++)
	//	itemManageSetColumnMargin(winItem(handle,OPTION_SELECT_ID),i,1,5);
	itemManageCreateItem(winItem(handle,OPTION_SELECT_ID),itemCreatemenuOption,getOptionResInfor,currentItem->optionSum);
	itemManageSetCharInfor(winItem(handle,OPTION_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,OPTION_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,OPTION_SELECT_ID),R_COLOR_BLACK);
	config=configGet(currentItem->configId);
	itemNum=0;
	while(itemNum<currentItem->optionSum)
	{
		if(currentItem->pOption[itemNum].str==config)
			break;
		itemNum++;
	}
	if(itemNum>=currentItem->optionSum)
		itemNum=0;
	itemManageSetCurItem(winItem(handle,OPTION_SELECT_ID),itemNum);
	winSetResid(winItem(handle,OPTION_TITLE_ID),currentItem->str);
#if BAT_FLOW_CHARGE_EN
	OptionBaterryShow(handle);
#endif
	return 0;
}
static int menuOptionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuOption Close Win!!!\n");
	currentItem=NULL;
	return 0;
}
static int menuOptionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuOption WinChild Close!!!\n");
	return 0;
}
static int menuOptionSysMsg500Ms(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

#if BAT_FLOW_CHARGE_EN
static int menuOptionSysMsg1s(winHandle handle,uint32 parameNum,uint32* parame)
{
	/*if(SysCtrl.usb == USB_STAT_DCIN)
	{
		if(winIsVisible(winItem(handle,ITEM_BATERRY_ID)))
			winSetVisible(winItem(handle,ITEM_BATERRY_ID),false);
		else
		{
			winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
			winSetVisible(winItem(handle,ITEM_BATERRY_ID),true);
		}	
	}*/

	//deg_Printf("xxxxxxxxxxx\r\n");
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
		OptionBaterryShow_charge(handle); //videoBaterryShow(handle);
	}

	return 0;
}
#endif

msgDealInfor menuOptionMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuOptionOpenWin},
	{SYS_CLOSE_WINDOW,menuOptionCloseWin},
	{SYS_CHILE_COLSE,menuOptionWinChildClose},
	{KEY_EVENT_OK,menuOptionKeyMsgOk},
	{KEY_EVENT_UP,menuOptionKeyMsgUp},
	{KEY_EVENT_DOWN,menuOptionKeyMsgDown},
	//{KEY_EVENT_MENU,menuOptionKeyMsgMenu},
	{KEY_EVENT_MODE,menuOptionKeyMsgMode},
	{KEY_EVENT_POWER,menuOptionKeyMsgPower},
	{KEY_EVENT_ESC,menuOptionKeyMsgEsc},
	{SYS_EVENT_SDC,menuOptionSysMsgSD},
#if BAT_FLOW_CHARGE_EN
	{SYS_EVENT_1S,menuOptionSysMsg1s},
#endif
	{EVENT_MAX,NULL},
};

WINDOW(menuOptionWindow,menuOptionMsgDeal,menuOptionWin)


