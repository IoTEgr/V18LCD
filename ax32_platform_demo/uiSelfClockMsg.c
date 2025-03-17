#include "uiSelfClockRes.c"




uint8 g_clock_flag=0;
INT32U g_clock_times=0;


static uint32 g_mdRecTime=0;

static uint8 g_clock_power_off_flag=0;
static uint8 g_clock_power_off_cnt=0;



static int getWeekdayByYearday(int iY, int iM, int iD, int *week) 
{
    int iWeekDay = -1;

    if (1 == iM || 2 == iM) 
    {   
        iM += 12; 
        iY--;
    }

    //deg_Printf("iY= %d, im=%d, iD=%d\r\n", iY, iM, iD);
    //W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
    iWeekDay = (iD + 1 + 2 * iM + 3 * (iM + 1) / 5 + iY + iY / 4 - iY / 100 + iY / 400) % 7;
    //deg_Printf("999999= %d\r\n", iWeekDay);

    *week = iWeekDay;

    return 0;
}


//static char YmdString[12] = {0};
//static char HmsString[12] = {0};
//static char weekday[10] = {0};
//static char RtcString[30] = {0};

static int SelfClockShow_rtc(winHandle handle,uint32 parameNum,uint32* parame)
{
    static char RtcString[30] = {0};
    //int num = 0;
    static char weekday[10] = {0};

    strcpy(RtcString, hal_rtcTime2String(hal_rtcTimeGet()));
    //debug_msg("RtcString= %s, rtc= %s\r\n", RtcString, hal_rtcTime2String(hal_rtcTimeGet()));

#if 0
    for(num = 0; num < 20; num++)// ymd, 11bit
    {
         if(num < 5)
         {

         }
         else if(num < 11)
         {
             YmdString[num-5] = RtcString[num];
         }
         else
         {
             HmsString[num-11] = RtcString[num];
         }

         YmdString[2] = '-';

    }
#endif
    
    //debug_msg("YmdString= %s, hms= %s\r\n", YmdString, HmsString);
    //winSetResid(winItem(handle,SELFCLOCK_YMD_ID),strToResId(YmdString));
    //winSetResid(winItem(handle,SELFCLOCK_HMS_ID),strToResId(HmsString));
    SelfClock_Date_Show(handle, RtcString);


    HAL_RTC_T *rtcTime;

    rtcTime = hal_rtcTimeGet();
    int WeekDay = 0;

    getWeekdayByYearday(rtcTime->year, rtcTime->month, rtcTime->day, &WeekDay);
    SelfClockWeedShow(handle, WeekDay);

    SelfClockShow_RecState(handle);

    return 0;
}


static int SelfClockKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        //deg_Printf("MENU SelfClock Close Win!!!\n");        
		winDestroy(&handle);
	}
	return 0;
}
static int SelfClockKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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

static int SelfClockPower(winHandle handle,uint32 parameNum,uint32* parame)
{
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
    {
        return 0;
    }
#endif
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        if(get_suspended_lcd_value())
        {
             return 0;
        }

        //taskStart(TASK_VIDEO_RECORD,0);
        //XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
	}

	return 0;
}

static int SelfClockPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        if(get_suspended_lcd_value())
        {
             return 0;
        }

        //taskStart(TASK_PLAY_BACK,0);
	}

	return 0;
}

static int SelfClockRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
    {
        return 0;
    }
#endif

	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        if(get_suspended_lcd_value())
        {
             return 0;
        }

        //taskStart(TASK_AUDIO_RECORD,0);
        //XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
	}

	return 0;
}

static int SelfClockPhotoLong(winHandle handle,uint32 parameNum,uint32* parame)
{
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
    {
        return 0;
    }
#endif

	//return SelfClockKeyMsgMenu(handle, parameNum, parame);
}

static int SelfClockRecordLong(winHandle handle,uint32 parameNum,uint32* parame)
{
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock())
    {
        return 0;
    }
#endif

	//return SelfClockKeyMsgMenu(handle, parameNum, parame);
}

static int SelfClockSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		SelfClockBaterryShow(handle);
	if(SysCtrl.battery <= BATTERY_STAT_2)
	{

	}
	return 0;
}

static int SelfClockSysMsgUsb(winHandle handle,uint32 parameNum,uint32* parame)
{
    SelfClockBaterryShow(handle);

	return 0;
}

static int SelfClockKeyMsg1s(winHandle handle,uint32 parameNum,uint32* parame)
{

    if(windowIsOpen(&tips1Window))
    {
        winDestroy(&tips1Window);
        return 0;
    }

	if( (g_clock_power_off_flag == 1 )&&(get_key_press_power_clock() == 1))
	{
		g_clock_power_off_cnt++;
		if(g_clock_power_off_cnt >= 2 )
		{
			g_clock_power_off_flag=0;
			g_clock_power_off_cnt=0;
			ax32xx_wdtEnable2(1);
			while(1);
		}
	}

	
	if((XOSTimeGet()- g_clock_times)> 800)
	{
		if( g_clock_flag == 1)
		{
			g_clock_flag = 0;
		}
	}

    // rtc string show
    SelfClockShow_rtc(handle, parameNum, parame);

    // bat show
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
        SelfClockBaterryShow_charge(handle); 
    }
	else
	{
		SelfClockBaterryShow(handle);
	}
#if KEY_PRESSED_POWER_TAP
    if(get_key_clock_value() && (get_key_press_power_clock() == 0))
#else
    if(get_key_clock_value())
#endif
    {
        //add on lcd
        deamon_screen_save(1);
		winDestroy(&handle);
    }

#if KEY_PRESSED_POWER_TAP
    static int clock_cnt = 0;
    if(get_key_press_power_clock())
    {
        clock_cnt++;
        if(clock_cnt >= 7)
        {
            taskStart(TASK_POWER_OFF,0);
        }
    }
#endif

	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if(g_mdRecTime&&XOSTimeGet()-g_mdRecTime>=MOTION_DEC_TIME*1000)
		{
			task_record_stop();
			g_mdRecTime=0;
		}
	}


	return 0;
}

static int SelfClockOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{    

    datechange.change_flag  = 0;
    weed_flag = 0;

    if(windowIsOpen(&tips1Window))
    {
        winDestroy(&tips1Window);
        return 0;
    }

	winSetVisible(winItem(handle,SELFCLOCK_POWEROFF_ID),false);
    SelfClockBaterryShow(handle);
    SelfClockShow_rtc(handle, parameNum, parame);
	deg_Printf("SelfClock Open Win!!!\n");
	
	return 0;
}
static int SelfClockCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("SelfClock Close Win!!!\n");
	return 0;
}
static int SelfClockWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
    SelfClockBaterryShow(handle);
	deg_Printf("SelfClock WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}

static int SelfClockvideoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{

	 if(!hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN))
 	{
		return 0;
 	}
	 
	if(videoRecordGetStatus() != MEDIA_STAT_START)
	{
		video_record_start();
	}

	g_mdRecTime=XOSTimeGet();
	return 0;
}

static int SelfClockPowerOFF(winHandle handle,uint32 parameNum,uint32* parame)
{

	if(configGet(CONFIG_ID_LANGUAGE) == R_ID_STR_LAN_SCHINESE)
    {
         winSetResid(winItem(handle,SELFCLOCK_POWEROFF_ID), R_ID_ICON_CLOCKPOWEROFFC);
    }
    else
    {
         winSetResid(winItem(handle,SELFCLOCK_POWEROFF_ID), R_ID_ICON_CLOCKPOWEROFFE);
    }

    winSetVisible(winItem(handle,SELFCLOCK_POWEROFF_ID),true);

	g_clock_power_off_flag=1;
	return 0;
}

msgDealInfor SelfClockMsgDeal[]=
{
	{SYS_OPEN_WINDOW,SelfClockOpenWin},
	{SYS_CLOSE_WINDOW,SelfClockCloseWin},
	{SYS_CHILE_COLSE,SelfClockWinChildClose},
    {KEY_EVENT_POWER,SelfClockPower},
    {KEY_EVENT_RECORD,SelfClockRecord},
    {KEY_EVENT_PHOTOLONG,SelfClockPhotoLong},
    {KEY_EVENT_RECORDLONG,SelfClockRecordLong},
    {KEY_EVENT_PHOTO,SelfClockPhoto},
    {SYS_EVENT_BAT,SelfClockSysMsgBattery},
    {SYS_EVENT_USB,SelfClockSysMsgUsb},
	{SYS_EVENT_1S,SelfClockKeyMsg1s},
	{SYS_EVENT_MD, SelfClockvideoSysMsgMD},
	{SELFCLOCK_EVENT_POWROFF, SelfClockPowerOFF},
	{EVENT_MAX,NULL},
};

WINDOW(selfclockWindow,SelfClockMsgDeal,SelfClockWin)


