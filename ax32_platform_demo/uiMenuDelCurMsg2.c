#include "uiMenuDelCurRes2.c"

static uint32 getdelCurResInfor(uint32 item,uint32* image,uint32* str)
{
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
	return 0;
}

static int delCurKeyMsgOk2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	char *name;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DELCUR_SELECT_ID));
		if(item==0)
		{
			if(managerFileCount(SysCtrl.wav_list)<=0)
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
				return;
			}
			if(managerCheckLockExt(SysCtrl.wav_list,SysCtrl.file_index)<=0)
			{
				name = manangerGetFileFullPathName(SysCtrl.wav_list,SysCtrl.file_index,NULL);
				deg_Printf("delete : %s.",name);
				if(f_unlink(name)==FR_OK)
				{
					deg_Printf("->ok\n");
					managerDelFileByIndex(SysCtrl.wav_list,SysCtrl.file_index);
					if(SysCtrl.file_index>managerFileCount(SysCtrl.wav_list))
						SysCtrl.file_index=managerFileCount(SysCtrl.wav_list);
					deamon_fsFreeSize();
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
				}
				else
				{
					deg_Printf("->fail\n");
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
				}
			}
			else
				uiOpenWindow(&tipsWindow,2,R_ID_STR_SET_LOCKED,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int delCurKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DELCUR_SELECT_ID));
	}
	return 0;
}
static int delCurKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DELCUR_SELECT_ID));
	}
	return 0;
}
static int delCurKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delCurKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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

static int delCurKeyMsgPower2(winHandle handle,uint32 parameNum,uint32* parame)
{
	deamon_clock_check(1);
	deamon_auto_poweroff(1);
	//deamon_screen_save(1);
	 if(get_suspended_lcd_value())
    {
        return 0;
    }
			
	return delCurKeyMsgOk2(handle, parameNum, parame);
}

static int delCurKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	return delCurKeyMsgMenu(handle, parameNum, parame);
}

static int delCurOpenWin2(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DELCUR_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,DELCUR_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,DELCUR_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,DELCUR_SELECT_ID),itemCreatemenuOption,getdelCurResInfor,2);
	itemManageSetCharInfor(winItem(handle,DELCUR_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,DELCUR_SELECT_ID),1);
	
	return 0;
}
static int delCurCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur Close Win!!!\n");
	return 0;
}
static int delCurWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor delCurMsgDeal2[]=
{
	{SYS_OPEN_WINDOW,delCurOpenWin2},
	{SYS_CLOSE_WINDOW,delCurCloseWin},
	{SYS_CHILE_COLSE,delCurWinChildClose},
	{KEY_EVENT_OK,delCurKeyMsgOk2},
	{KEY_EVENT_UP,delCurKeyMsgUp},
	{KEY_EVENT_DOWN,delCurKeyMsgDown},
	//{KEY_EVENT_MENU,delCurKeyMsgMenu},
	{KEY_EVENT_MODE,delCurKeyMsgMode},
	{KEY_EVENT_POWER,delCurKeyMsgPower2},
	{KEY_EVENT_ESC,delCurKeyMsgEsc},
	{EVENT_MAX,NULL},
};

WINDOW(delCurWindow2,delCurMsgDeal2,delCurWin2)


