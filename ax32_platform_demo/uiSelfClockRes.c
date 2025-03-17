#include"uiWin.h"

enum
{
	SELFCLOCK_RECT_ID=0, //SELFCLOCK_TIME_ID=0,
    SELFCLOCK_YMD_ID,
    SELFCLOCK_HMS_ID,
    SELFCLOCK_WEEK_ID,
    SELFCLOCK_CLOCK_ID,
    SELFCLOCK_REC_STATE_ID ,
    SELFCLOCK_REC_ASTATE_ID ,//=6,
    //MM-DD
    SELFCLOCK_YMD_MOI_ID,
    SELFCLOCK_YMD_MOII_ID,
    SELFCLOCK_YMD_DI_ID,
    SELFCLOCK_YMD_DII_ID,
    //HH-MM-SS
    SELFCLOCK_HMS_HI_ID,//=11,
    SELFCLOCK_HMS_HII_ID,
    SELFCLOCK_HMS_MII_ID,
    SELFCLOCK_HMS_MIII_ID,
    SELFCLOCK_HMS_SI_ID,
    SELFCLOCK_HMS_SII_ID,// =16,
    //DEVIDE '-' ':' ':'
    SELFCLOCK_YMD_DV_ID,
    SELFCLOCK_HMS_HD1_ID,
    SELFCLOCK_HMS_HD2_ID,
	SELFCLOCK_BATERRY_ID,
	SELFCLOCK_POWEROFF_ID,
    SELFCLOCK_MAX_ID
};
static widgetCreateInfor SelfClockWin[] =
{
    #if (SMALL_PANEL_SUPPORT==0)
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(142),R_COLOR_GRAY2,WIN_ABS_POS),
    createImageIcon(PHOTO_BATERRY_ID,       Rx(250), Ry(0), Rw(20), Rh(20), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
    widgetEnd(),
	#else
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
    //createRect(SELFCLOCK_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK),
	createImageIcon(SELFCLOCK_CLOCK_ID,       Rx(0), Ry(0), Rw(320), Rh(240), R_ID_ICON_CLOCKRECT1,ALIGNMENT_CENTER),
    //createImageIcon(SELFCLOCK_CLOCK_ID,       Rx(0), Ry(0), Rw(240), Rh(240), R_ID_ICON_CLOCKRECT,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_BATERRY_ID,       Rx(300), Ry(0), Rw(20), Rh(30), R_ID_ICON_MTBATTERY4,ALIGNMENT_CENTER),

    //createStringIcon(SELFCLOCK_YMD_ID, Rx(80), Ry(30), Rw(200), Rh(50),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,2),
    //createStringIcon(SELFCLOCK_HMS_ID, Rx(20), Ry(100), Rw(280), Rh(50),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,3),
    //createStringIcon(SELFCLOCK_WEEK_ID, Rx(65), Ry(175), Rw(100), Rh(65),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,5),
    createImageIcon(SELFCLOCK_YMD_MOI_ID,  Rx(90), Ry(40), Rw(20), Rh(20), R_ID_ICON_DATE_L0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_YMD_MOII_ID, Rx(120), Ry(40), Rw(20), Rh(20), R_ID_ICON_DATE_L0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_YMD_DV_ID,   Rx(150), Ry(40), Rw(12), Rh(16), R_ID_ICON_DATE_LD,ALIGNMENT_CENTER),
    
    createImageIcon(SELFCLOCK_YMD_DI_ID,   Rx(180), Ry(40), Rw(20), Rh(20), R_ID_ICON_DATE_L0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_YMD_DII_ID,  Rx(210), Ry(40), Rw(20), Rh(20), R_ID_ICON_DATE_L0,ALIGNMENT_CENTER),

    createImageIcon(SELFCLOCK_HMS_HI_ID,   Rx(30),  Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_B0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_HII_ID,  Rx(70),  Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_B0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_HD1_ID,  Rx(100), Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_BD,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_MII_ID,  Rx(130), Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_B0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_MIII_ID, Rx(170), Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_B0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_HD2_ID,  Rx(200), Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_BD,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_SI_ID,   Rx(230), Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_B0,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_HMS_SII_ID,  Rx(270), Ry(100), Rw(28), Rh(42), R_ID_ICON_DATE_B0,ALIGNMENT_CENTER),


    createImageIcon(SELFCLOCK_WEEK_ID,Rx(110), Ry(/*175*/187), Rw(100), Rh(30), R_ID_ICON_WEEK_SC_SAT,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_REC_STATE_ID,Rx(150), Ry(3), Rw(20), Rh(24), R_ID_ICON_RECSTATE,ALIGNMENT_CENTER),
    createImageIcon(SELFCLOCK_REC_ASTATE_ID,Rx(150), Ry(4), Rw(20), Rh(24), R_ID_ICON_RECASTATE,ALIGNMENT_CENTER),

	createImageIcon(SELFCLOCK_POWEROFF_ID,Rx(65), Ry(170), Rw(200), Rh(60), R_ID_ICON_CLOCKPOWEROFFC,ALIGNMENT_CENTER),


    widgetEnd(),
    #endif
};



static void SelfClockBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,SELFCLOCK_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY4);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,SELFCLOCK_BATERRY_ID),true);
}

static void SelfClockBaterryShow(winHandle handle)
{
	//deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
	if(SysCtrl.usb != USB_STAT_NULL)
		SelfClockBaterryShow_charge(handle);
		//winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery >= BATTERY_STAT_4)
		winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,SELFCLOCK_BATERRY_ID),R_ID_ICON_CLOCK_BATTERY0);
	winSetVisible(winItem(handle,SELFCLOCK_BATERRY_ID),true);

}


static void SelfClock_YMD_HMS_L_Config(winHandle handle, int datetype, int dateval)
{
	winSetVisible(winItem(handle,datetype),false);

    switch(dateval)
    {
        case 0:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L0);
            break;
        case 1:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L1);
            break;
        case 2:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L2);
            break;
        case 3:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L3);
            break;
        case 4:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L4);
            break;
        case 5:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L5);
            break;
        case 6:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L6);
            break;
        case 7:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L7);
            break;
        case 8:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L8);
            break;
        case 9:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_L9);
            break;

    }

	winSetVisible(winItem(handle,datetype),true);
}


static void SelfClock_YMD_HMS_B_Config(winHandle handle, int datetype, int dateval)
{
	winSetVisible(winItem(handle,datetype),false);

    switch(dateval)
    {
        case 0:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B0);
            break;
        case 1:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B1);
            break;
        case 2:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B2);
            break;
        case 3:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B3);
            break;
        case 4:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B4);
            break;
        case 5:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B5);
            break;
        case 6:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B6);
            break;
        case 7:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B7);
            break;
        case 8:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B8);
            break;
        case 9:
            winSetResid(winItem(handle, datetype), R_ID_ICON_DATE_B9);
            break;

    }

	winSetVisible(winItem(handle,datetype),true);
}


typedef struct s_Change_Flag
{
    int month_save;
    int day_save;
    int hour_save;
    int minute_save;
    int change_flag;
}Date_Change_Flag;

static Date_Change_Flag datechange;

static void SelfClock_Date_Show(winHandle handle, u8 *datestr)
{
    if(datechange.change_flag == 0)
    {
      	datechange.month_save = (datestr[5] -  '0')*10  + (datestr[6]  - '0');
        datechange.day_save = (datestr[8] -  '0')*10  + (datestr[9]  - '0');
        datechange.hour_save = (datestr[11] - '0')*10 + (datestr[12] - '0');
        datechange.minute_save = (datestr[14] - '0')*10 + (datestr[15] - '0');
        datechange.change_flag = 1;

        SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_MOI_ID,  datestr[5]  - '0');
        SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_MOII_ID, datestr[6] -  '0');
        SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_DI_ID,   datestr[8] -  '0');
        SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_DII_ID,  datestr[9] -  '0');
        SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_HI_ID,   datestr[11] - '0');
        SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_HII_ID,  datestr[12] - '0');
        SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_MII_ID,  datestr[14] - '0');
        SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_MIII_ID, datestr[15] - '0');
    }

    if(datechange.change_flag == 1)
    {
        if(datechange.month_save != (datestr[5] - '0')*10 + (datestr[6] - '0'))
        {
            SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_MOI_ID,  datestr[5]  - '0');
            SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_MOII_ID, datestr[6] -  '0');
        }
		
        if(datechange.day_save != (datestr[8] - '0')*10 + (datestr[9] - '0'))
        {
            SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_DI_ID,   datestr[8] -  '0');
            SelfClock_YMD_HMS_L_Config(handle, SELFCLOCK_YMD_DII_ID,  datestr[9] -  '0');
        }

	
        if(datechange.hour_save != (datestr[11] - '0')*10 + (datestr[12] - '0'))
        {            
            SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_HI_ID,   datestr[11] - '0');
            SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_HII_ID,  datestr[12] - '0');
        }
    	
        if(datechange.minute_save != (datestr[14] - '0')*10 + (datestr[15] - '0'))
        {
            SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_MII_ID,  datestr[14] - '0');
            SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_MIII_ID, datestr[15] - '0');
        }
    }

    SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_SI_ID,   datestr[17] - '0');
    SelfClock_YMD_HMS_B_Config(handle, SELFCLOCK_HMS_SII_ID,  datestr[18] - '0');
}


static void SelfClockWeedShow_EN(winHandle handle, int iWeekDay)
{
    switch(iWeekDay)
    {   
        case 0:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_SUN);
            break;
        case 1:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_MON);
            break;
        case 2:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_TUE);
            break;
        case 3:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_WED);
            break;
        case 4:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_THUR);
            break;
        case 5:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_FRI);
            break;
        case 6:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_EN_SAT);
            break;
 
        default :
            break;
    }

}


static void SelfClockWeedShow_SC(winHandle handle, int iWeekDay)
{
    switch(iWeekDay)
    {   
        case 0:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_SUN);
            break;
        case 1:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_MON);
            break;
        case 2:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_TUE);
            break;
        case 3:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_WED);
            break;
        case 4:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_THUR);
            break;
        case 5:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_FRI);
            break;
        case 6:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_SC_SAT);
            break;
 
        default :
            break;
    }

}

static void SelfClockWeedShow_JA(winHandle handle, int iWeekDay)
{
    switch(iWeekDay)
    {   
        case 0:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_SUN);
            break;
        case 1:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_MON);
            break;
        case 2:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_TUE);
            break;
        case 3:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_WED);
            break;
        case 4:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_THUR);
            break;
        case 5:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_FRI);
            break;
        case 6:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_JA_SAT);
            break;
 
        default :
            break;
    }

}

static void SelfClockWeedShow_KO(winHandle handle, int iWeekDay)
{
    switch(iWeekDay)
    {   
        case 0:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_SUN);
            break;
        case 1:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_MON);
            break;
        case 2:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_TUE);
            break;
        case 3:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_WED);
            break;
        case 4:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_THUR);
            break;
        case 5:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_FRI);
            break;
        case 6:
            winSetResid(winItem(handle,SELFCLOCK_WEEK_ID), R_ID_ICON_WEEK_KO_SAT);
            break;
 
        default :
            break;
    }

}



static int weed_flag = 0;
static int weed_save = 0;

static void SelfClockWeedShow(winHandle handle, int iWeekDay)
{
	//deg_Printf("[%d] %s, iWeekDay=%d \r\n", __LINE__, __FUNCTION__, iWeekDay);

    if(weed_flag == 0)
    {
        weed_flag = 1;
        weed_save = iWeekDay;
    }
    else
    {
        if(weed_save == iWeekDay)
        {
            return;
        }
    }

	winSetVisible(winItem(handle,SELFCLOCK_WEEK_ID),false);

    if(configGet(CONFIG_ID_LANGUAGE) == R_ID_STR_LAN_ENGLISH)
    {
        SelfClockWeedShow_EN(handle, iWeekDay);
    }
    else if(configGet(CONFIG_ID_LANGUAGE) == R_ID_STR_LAN_SCHINESE)
    {
        SelfClockWeedShow_SC(handle, iWeekDay);
    }
	 else if(configGet(CONFIG_ID_LANGUAGE) == R_ID_STR_LAN_JAPANESE)
    {
        SelfClockWeedShow_JA(handle, iWeekDay);
    }
	 else if(configGet(CONFIG_ID_LANGUAGE) == R_ID_STR_LAN_KOREA)
    {
        SelfClockWeedShow_KO(handle, iWeekDay);
    }

    winSetVisible(winItem(handle,SELFCLOCK_WEEK_ID),true);
}


static void SelfClockShow_RecState(winHandle handle)
{
	winSetVisible(winItem(handle,SELFCLOCK_REC_STATE_ID),false);
	winSetVisible(winItem(handle,SELFCLOCK_REC_ASTATE_ID),false);

    if(videoRecordGetStatus() == MEDIA_STAT_START )
    {
        //winSetResid(winItem(handle,SELFCLOCK_REC_STATE_ID), R_ID_ICON_WEEK_SC_SAT);
        winSetVisible(winItem(handle,SELFCLOCK_REC_STATE_ID),true);

    }
	else if ( audioRecordGetStatus() == MEDIA_STAT_START )
	{
		winSetVisible(winItem(handle,SELFCLOCK_REC_ASTATE_ID),true);
	}
}





