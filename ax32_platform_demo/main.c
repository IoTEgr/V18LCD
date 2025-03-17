/****************************************************************************
**
 **                              MAIN 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MAIN ENTER  OF SYSTEM
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : main.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h" 

System_Ctrl_T SysCtrl;
static MSG_T sysDeviceQStack[SYSDEVICE_Q_SIZE];
extern sysTask taskPowerOff;
extern sysTask taskVideoRecorde;
extern sysTask taskPhotoEncode;
extern sysTask taskPlayBack;
extern sysTask taskUsbDevice;
extern sysTask taskAudioRecorde;
extern sysTask taskAudioPlay;
extern msgDealInfor sysUnshieldedMsgDeal[];
void registerTask(void)
{
	taskRegister(TASK_POWER_OFF,&taskPowerOff);
	taskRegister(TASK_VIDEO_RECORD,&taskVideoRecorde);
	taskRegister(TASK_PHOTO_ENCODE,&taskPhotoEncode);
	taskRegister(TASK_PLAY_BACK,&taskPlayBack);
	taskRegister(TASK_USB_DEVICE,&taskUsbDevice);
	taskRegister(TASK_AUDIO_RECORD,&taskAudioRecorde);
	taskRegister(TASK_AUDIO_PLAY,&taskAudioPlay);
}

bool sysPowerOn=false;
/*******************************************************************************
* Function Name  : init
* Description    : initial system hardware & task
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
extern int uninit(void); //taskPowerOff.c
extern void recordTimeCount1S(void);
XWork_T *  recordIncrease1S=NULL;
int init(void)
{
   // debgchar('0');
	debg("****************CLKTUN:%x, OUT %x, IN %x***************\n",CLKTUN,(CLKTUN>>7)&0x1f,CLKTUN&0x1f);
	debg("****************DLLCON:%x, CH1 %x, CH2 %x***************\n",DLLCON,(DLLCON>>25)&0xf,(DLLCON>>21)&0xf);
	debg("CHIP_ID:%x\n",CHIP_ID);

	hal_vddWKOEnable(1);  // hold power
    hal_wkiWakeup(1);
    
	hal_sysInit();	//initial system for free run
	
	hal_uartInit();
	
//----------initial system work queue.work queues are isr callback function	
	taskInit();
	registerTask();
	XOSInit();	
//----------start system work queue for system check & deamon service	
	hal_timerStart(HAL_CFG_OS_TIMER,X_CFG_TICK,XOSTickService);  // 1000 hz 
	XWorkCreate(15*X_TICK,isp_process_check);  // isp process
	recordIncrease1S=XWorkCreate(1000*X_TICK,recordTimeCount1S);
	deg_Printf("system power on\n");
	taskInfor(1);
//----------board initial,LCD,LED,SPI,ADC,DAC....	
	boardInit(NULL);   // initial main board	
//----------get board device ioctrl handler	
    memset(&SysCtrl,0,sizeof(System_Ctrl_T));
	SysCtrl.bfd_battery = boardOpen(DEV_NAME_BATTERY);
    //SysCtrl.bfd_gsensor = boardOpen(DEV_NAME_GSENSOR);	
	SysCtrl.bfd_usb 	= boardOpen(DEV_NAME_USB);
	SysCtrl.bfd_key 	= boardOpen(DEV_NAME_ADKEY);   
    SysCtrl.bfd_sdcard  = boardOpen(DEV_NAME_SDCARD);		
	SysCtrl.bfd_led     = boardOpen(DEV_NAME_LED);
	SysCtrl.bfd_lcd     = boardOpen(DEV_NAME_LCD);
	//SysCtrl.bfd_ir 		= boardOpen(DEV_NAME_IR);
	//SysCtrl.bfd_usensor= boardOpen(DEV_NAME_USENSOR);
	SysCtrl.powerflag = POWERON_FLAG_MAX;
	
	if(SysCtrl.bfd_battery<0)
	{
		uninit();
	}
//-----------welcome
	deg_Printf("\n");
    deg_Printf(">>---------------------------------------------<<\n");
	deg_Printf(">>- Welcome.BuildWin SZ LTD.CO-----------------<<\n");
    deg_Printf(">>- VIDEO PROJECT TEAM.VISION------------------<<\n");
	deg_Printf(">>- %s --------------------<<\n",SYSTEM_VERSION);
    deg_Printf(">>---------------------------------------------<<\n");
//----------nv fs for read only resource
	nv_init();	 

//----------user menu configure set
	userConfigInitial();

//----------power on flag check
	int ret,temp;

	ret = boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_PARK,(INT32U)&temp);
    if((ret>=0) && temp && configValue2Int(CONFIG_ID_PARKMODE))
    {
		SysCtrl.powerflag = POWERON_FLAG_GSENSOR;  // gsensor wakeup by parking mode active
		SysCtrl.g_lock = 1;  // set lock flag
		deg_Printf("gsen wakeup parking mode active\n");
		//SysCtrl.f_update|=FLAG_LOK;
    }
    else
	{
		if(hal_rtcAlarmStatusGet(1))
		{
			SysCtrl.powerflag = POWERON_FLAG_RTC;
		}
		else
		{
			boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			if(temp)
				SysCtrl.powerflag = POWERON_FLAG_KEY; 
			else
			{
			    boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
				if(temp)
				   SysCtrl.powerflag = POWERON_FLAG_DCIN;     // dcin or usb in power on
				else
				   SysCtrl.powerflag = POWERON_FLAG_KEY;     // dcin or usb in power on  
			}
		}
    }
#if CMOS_USART_ONLINE_DBG>0
    XWorkCreate(10*X_TICK,sensorDBGTimeout);
#endif
    if(SysCtrl.powerflag==POWERON_FLAG_KEY) // power key is not hold
	{
		temp = 0;
		boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
		if(temp==0)
		{
			//uninit();
		}
	}
//	hal_dacInit();
	layout_logo_show(0,0,1);//layout_logo_show(1,0,1);  // power on.music en,do not wait music end	//---->show logo here,can speed start logo show.make user feeling system starting faster
//	XOSTimeDly(50);  // delay 80ms,wait lcd frame end	
//	boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
//--------initial resource manager for fs
	managerInit();
//--------initial font  & ui & configure
	fontInit();
    iconInit();
//---------update time RTC time
    //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
//	deg_Printf("main : backlight on.%d ms\n",XOSTimeGet()-tick);
//	int fd = boardOpen(DEV_NAME_BATTERY);
	//boardNameIoctrl("cmos-sensor",IOCTRL_CMOS_INIT,0);
//    deg_Printf("main : csi end.%d ms\n",XOSTimeGet()-tick);
	SysCtrl.t_wait = XOSTimeGet();
    DATE_TIME_T *rtcTime = hal_rtcTimeGet();	
	if(/*userConfigInitial() || */(rtcTime->year<2017)) // user configure reset  
	{
    #if DATETIME_LOAD_AUTO >0	
        rtcTime->year = configGet(CONFIG_ID_YEAR);
		rtcTime->month = configGet(CONFIG_ID_MONTH);
		rtcTime->day = configGet(CONFIG_ID_MDAY);
		rtcTime->hour = configGet(CONFIG_ID_HOUR);
		rtcTime->min = configGet(CONFIG_ID_MIN);
		rtcTime->sec = configGet(CONFIG_ID_SEC);
    #else
		rtcTime->year = 2017;
		rtcTime->month =1;
		rtcTime->day = 1;
		rtcTime->hour = 0;
		rtcTime->min = 0;
		rtcTime->sec = 0;
    #endif		
		hal_rtcTimeSet(rtcTime); // default time ->2017/01/01 00:00:00
	}	
    SysCtrl.pip_mode = 0;
	SysCtrl.f_update = 0;
    SysCtrl.avi_list = -1;
	SysCtrl.jpg_list = -1;
	SysCtrl.wav_list = -1;
	SysCtrl.powerOnTime=0;;
	SysCtrl.f_keysound = R_ID_STR_COM_OFF;//configValue2Int(CONFIG_ID_KEYSOUND);
	SysCtrl.sysQ = XMsgQCreate(sysDeviceQStack,SYSDEVICE_Q_SIZE);
	boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETLOCK,(INT32U)configValue2Int(CONFIG_ID_GSENSOR));	//set gsensor default level
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,configValue2Int(CONFIG_ID_IR_LED));
	SysCtrl.sdcard = SDC_STAT_NULL;//SDC_STAT_UNSTABLE;
	SysCtrl.usb =USB_STAT_NULL;
	SysCtrl.battery = BATTERY_STAT_4;  // DEFAULT VALUE
//--------initial fs	
	fs_init();
//--------board check ,the first time.
	taskSysScanDev(0); // check board state
//--------wait power on music end.	
    ret = XOSTimeGet();
	volatile int i = 0x08fffff;
	i = 0x08fffff;
    while((audioPlaybackGetStatus() == MEDIA_STAT_PLAY)) // wait music end
	{
		i--;
		if(i==0)
			break;
		if((i&0xffff)==0)
			deg_Printf(".");
	}
    //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
    
    if( hal_gpioRead(SW_MIC_KEY_CH, SW_MIC_KEY_PIN) )
	{
		taskStart(TASK_AUDIO_RECORD,0);
	}
	else
	{
		taskStart(TASK_VIDEO_RECORD,0);
	}
	while(XOSTimeGet()-ret <= 1500)	// for check usb 
	{
		//XOSTimeDly(10);
		taskSysScanDev(0); // check board state
		hal_wdtClear();
	}
    INT16U width,height;
    dispLayerInit(DISP_LAYER_OSD0);                                    // enable ui display layer
    dispLayerGetResolution(DISP_LAYER_OSD0,&width,&height); // get osd size
	initDrawBuffWH(width,height);   // intitial ui manager
    R_loadResource((void *)User_Icon_Table,R_ICON_MAX);       // load icon table
    layout_keysound_load(NULL);
	winInit();
	//if all device is working , baterry may low, must off sysytem
	return 0;
}

#if KEY_PRESSED_POWER_TAP

int tap_exec(void)
{
	int ret;

//--------initial resource manager for fs
    managerInit();
//--------initial font  & ui & configure
    fontInit();
    iconInit();
//---------update time RTC time
    //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
//  deg_Printf("main : backlight on.%d ms\n",XOSTimeGet()-tick);
//  int fd = boardOpen(DEV_NAME_BATTERY);
    boardNameIoctrl("cmos-sensor",IOCTRL_CMOS_INIT,0);
//    deg_Printf("main : csi end.%d ms\n",XOSTimeGet()-tick);
    SysCtrl.t_wait = XOSTimeGet();
    DATE_TIME_T *rtcTime = hal_rtcTimeGet();    
    if(/*userConfigInitial() || */(rtcTime->year<2017)) // user configure reset  
    {
#if DATETIME_LOAD_AUTO >0	
        rtcTime->year = configGet(CONFIG_ID_YEAR);
        rtcTime->month = configGet(CONFIG_ID_MONTH);
        rtcTime->day = configGet(CONFIG_ID_MDAY);
        rtcTime->hour = configGet(CONFIG_ID_HOUR);
        rtcTime->min = configGet(CONFIG_ID_MIN);
        rtcTime->sec = configGet(CONFIG_ID_SEC);
#else
        rtcTime->year = 2017;
        rtcTime->month =1;
        rtcTime->day = 1;
        rtcTime->hour = 0;
        rtcTime->min = 0;
        rtcTime->sec = 0;
#endif		
        hal_rtcTimeSet(rtcTime); // default time ->2017/01/01 00:00:00
    }   
    SysCtrl.pip_mode = 0;
    SysCtrl.f_update = 0;
    SysCtrl.avi_list = -1;
    SysCtrl.jpg_list = -1;
    SysCtrl.wav_list = -1;
    SysCtrl.powerOnTime=0;;
    //SysCtrl.f_keysound = configValue2Int(CONFIG_ID_KEYSOUND);
    SysCtrl.sysQ = XMsgQCreate(sysDeviceQStack,SYSDEVICE_Q_SIZE);

    SysCtrl.sdcard = SDC_STAT_NULL;//SDC_STAT_UNSTABLE;
    SysCtrl.usb =USB_STAT_NULL;
    SysCtrl.battery = BATTERY_STAT_0;// BATTERY_STAT_4;  // DEFAULT VALUE
//--------initial fs    
    fs_init();
//--------board check ,the first time.
    //taskSysScanDev(0); // check board state
//--------wait power on music end.  
    ret = XOSTimeGet();
    
    //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
#if 0
    while(XOSTimeGet()-ret <= 1500) // for check usb 
    {
        //XOSTimeDly(10);
        taskSysScanDev(0); // check board state
        hal_wdtClear();
    }
#endif
    INT16U width,height;
    dispLayerInit(DISP_LAYER_OSD0);                                    // enable ui display layer
    dispLayerGetResolution(DISP_LAYER_OSD0,&width,&height); // get osd size
    initDrawBuffWH(width,height);   // intitial ui manager
    R_loadResource((void *)User_Icon_Table,R_ICON_MAX);       // load icon table
    //layout_keysound_load(NULL);
    winInit();
    //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);

    //if all device is working , baterry may low, must off sysytem
    return 0;

}

int init_tap(void) //cp from init(void)
{
   // debgchar('0');
	debg("****************CLKTUN:%x, OUT %x, IN %x***************\n",CLKTUN,(CLKTUN>>7)&0x1f,CLKTUN&0x1f);
	debg("****************DLLCON:%x, CH1 %x, CH2 %x***************\n",DLLCON,(DLLCON>>25)&0xf,(DLLCON>>21)&0xf);
	debg("CHIP_ID:%x\n",CHIP_ID);

	hal_vddWKOEnable(1);  // hold power
    hal_wkiWakeup(1);
    
	hal_sysInit();	//initial system for free run
	
	hal_uartInit();

//----------initial system work queue.work queues are isr callback function	
	taskInit();
	registerTask();
	XOSInit();	
//----------start system work queue for system check & deamon service	
	hal_timerStart(HAL_CFG_OS_TIMER,X_CFG_TICK,XOSTickService);  // 1000 hz 
	XWorkCreate(15*X_TICK,isp_process_check);  // isp process
	recordIncrease1S=XWorkCreate(1000*X_TICK,recordTimeCount1S);
	deg_Printf("system power on\n");
	taskInfor(1);
//----------board initial,LCD,LED,SPI,ADC,DAC....	
	boardInit(NULL);   // initial main board	
//----------get board device ioctrl handler	
    memset(&SysCtrl,0,sizeof(System_Ctrl_T));
	SysCtrl.bfd_battery = boardOpen(DEV_NAME_BATTERY);
    //SysCtrl.bfd_gsensor = boardOpen(DEV_NAME_GSENSOR);	
	SysCtrl.bfd_usb 	= boardOpen(DEV_NAME_USB);
	SysCtrl.bfd_key 	= boardOpen(DEV_NAME_ADKEY);   
    SysCtrl.bfd_sdcard  = boardOpen(DEV_NAME_SDCARD);		
	SysCtrl.bfd_led     = boardOpen(DEV_NAME_LED);
	SysCtrl.bfd_lcd     = boardOpen(DEV_NAME_LCD);
	//SysCtrl.bfd_ir 		= boardOpen(DEV_NAME_IR);
	//SysCtrl.bfd_usensor= boardOpen(DEV_NAME_USENSOR);
	SysCtrl.powerflag = POWERON_FLAG_MAX;
	if(SysCtrl.bfd_battery<0)
	{
		uninit();
	}
//-----------welcome
	deg_Printf("\n");
    deg_Printf(">>---------------------------------------------<<\n");
	deg_Printf(">>- Welcome.BuildWin SZ LTD.CO-----------------<<\n");
    deg_Printf(">>- VIDEO PROJECT TEAM.VISION------------------<<\n");
	deg_Printf(">>- %s --------------------<<\n",SYSTEM_VERSION);
    deg_Printf(">>---------------------------------------------<<\n");
//----------nv fs for read only resource
	nv_init();	 
//----------user menu configure set
	userConfigInitial();
//----------power on flag check
	int ret,temp;

	//ret = boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_PARK,(INT32U)&temp);
    if(0) //if((ret>=0) && temp && configValue2Int(CONFIG_ID_PARKMODE))
    {
		SysCtrl.powerflag = POWERON_FLAG_GSENSOR;  // gsensor wakeup by parking mode active
		SysCtrl.g_lock = 1;  // set lock flag
		deg_Printf("gsen wakeup parking mode active\n");
		//SysCtrl.f_update|=FLAG_LOK;
    }
    else
	{
		if(0) //if(hal_rtcAlarmStatusGet(1))
		{
			SysCtrl.powerflag = POWERON_FLAG_RTC;
		}
		else
		{
			boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			if(temp)
				SysCtrl.powerflag = POWERON_FLAG_KEY; 
			else
			{
			    boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
				if(temp)
				   SysCtrl.powerflag = POWERON_FLAG_DCIN;     // dcin or usb in power on
				else
				   SysCtrl.powerflag = POWERON_FLAG_KEY;     // dcin or usb in power on  
			}
		}
    }			  
#if CMOS_USART_ONLINE_DBG>0
    XWorkCreate(10*X_TICK,sensorDBGTimeout);
#endif
    if(SysCtrl.powerflag==POWERON_FLAG_KEY) // power key is not hold
	{
		temp = 0;
		boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
		//if(temp==0)
		 if( !(hal_gpioRead(SW_MIC_KEY_CH, SW_MIC_KEY_PIN) ) && !(hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN) ) && temp==0) 
		{
			//uninit();
            set_key_press_power_clock(1);
		}
	}
//	hal_dacInit();

    if(get_key_press_power_clock())
    {
        tap_exec();
        return 0;
    }

	layout_logo_show(0,0,1);//layout_logo_show(1,0,1);  // power on.music en,do not wait music end	//---->show logo here,can speed start logo show.make user feeling system starting faster
//	XOSTimeDly(50);  // delay 80ms,wait lcd frame end	
//	boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on

//--------initial resource manager for fs
	managerInit();
//--------initial font  & ui & configure
	fontInit();
    iconInit();
//---------update time RTC time
    //boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
//	deg_Printf("main : backlight on.%d ms\n",XOSTimeGet()-tick);
//	int fd = boardOpen(DEV_NAME_BATTERY);
	boardNameIoctrl("cmos-sensor",IOCTRL_CMOS_INIT,0);

//    deg_Printf("main : csi end.%d ms\n",XOSTimeGet()-tick);
	SysCtrl.t_wait = XOSTimeGet();
    DATE_TIME_T *rtcTime = hal_rtcTimeGet();	
	if(/*userConfigInitial() || */(rtcTime->year<2017)) // user configure reset  
	{
    #if DATETIME_LOAD_AUTO >0	
        rtcTime->year = configGet(CONFIG_ID_YEAR);
		rtcTime->month = configGet(CONFIG_ID_MONTH);
		rtcTime->day = configGet(CONFIG_ID_MDAY);
		rtcTime->hour = configGet(CONFIG_ID_HOUR);
		rtcTime->min = configGet(CONFIG_ID_MIN);
		rtcTime->sec = configGet(CONFIG_ID_SEC);
    #else
		rtcTime->year = 2017;
		rtcTime->month =1;
		rtcTime->day = 1;
		rtcTime->hour = 0;
		rtcTime->min = 0;
		rtcTime->sec = 0;
    #endif		
		hal_rtcTimeSet(rtcTime); // default time ->2017/01/01 00:00:00
	}	
    SysCtrl.pip_mode = 0;
	SysCtrl.f_update = 0;
    SysCtrl.avi_list = -1;
	SysCtrl.jpg_list = -1;
	SysCtrl.wav_list = -1;
	SysCtrl.powerOnTime=0;
	SysCtrl.f_keysound = R_ID_STR_COM_OFF ;//configValue2Int(CONFIG_ID_KEYSOUND);
	SysCtrl.sysQ = XMsgQCreate(sysDeviceQStack,SYSDEVICE_Q_SIZE);
	
	boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETLOCK,(INT32U)configValue2Int(CONFIG_ID_GSENSOR));	//set gsensor default level
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,configValue2Int(CONFIG_ID_IR_LED));

	SysCtrl.sdcard = SDC_STAT_NULL;//SDC_STAT_UNSTABLE;
	SysCtrl.usb = USB_STAT_NULL;
	SysCtrl.battery = BATTERY_STAT_4;  // DEFAULT VALUE
//--------initial fs	
	fs_init();
//--------board check ,the first time.
	taskSysScanDev(0); // check board state
//--------wait power on music end.	
    ret = XOSTimeGet();
	volatile int i = 0x08fffff;
	i = 0x08fffff;
    while((audioPlaybackGetStatus() == MEDIA_STAT_PLAY)) // wait music end
	{
		i--;
		if(i==0)
			break;
		if((i&0xffff)==0)
			deg_Printf(".");
	}
    boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1); // back light on
	while(XOSTimeGet()-ret <= 700)	// for check usb 
	{
		//XOSTimeDly(10);
		taskSysScanDev(0); // check board state
		hal_wdtClear();
	}
    INT16U width,height;
    dispLayerInit(DISP_LAYER_OSD0);                                    // enable ui display layer
    dispLayerGetResolution(DISP_LAYER_OSD0,&width,&height); // get osd size
	initDrawBuffWH(width,height);   // intitial ui manager
    R_loadResource((void *)User_Icon_Table,R_ICON_MAX);       // load icon table
    layout_keysound_load(NULL);
	winInit();
	//if all device is working , baterry may low, must off sysytem
	return 0;
}


#endif

/*******************************************************************************
* Function Name  : main
* Description    : 
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{	
	
//--------------power on--------	 
#if KEY_PRESSED_POWER_TAP
    init_tap();
    configSystem();
#else
    init();       // system power on configure
	configSystem();

//----fireware upgrade
	upgrade();
#endif

/*
TASK_POWER_OFF	has highest priority,if other place start TASK_POWER_OFF task,
the task started here will be ignored;
*/	
	SysCtrl.battery_charge = SysCtrl.battery;
	taskStart(TASK_VIDEO_RECORD,0);
	unshieldedMsgFuncRegister(sysUnshieldedMsgDeal);
	sysPowerOn=true;
	XMsgQFlush(SysCtrl.sysQ);
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
	XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_BAT,SysCtrl.battery));
	if(SysCtrl.usb!=USB_STAT_NULL)
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USENSOR,SysCtrl.usensorshow));
	//XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_ASTERN,SysCtrl.astern));

	
	if( HAL_CFG_EN_DBG == 0 ) 
	{
		hal_gpioInit(IR_CHECK_CH,IR_CHECK_PIN,GPIO_INPUT,GPIO_PULL_UP);
	}
	taskService();
	//self_test();
	return 2; // for usb upgrade
}
/*******************************************************************************
* Function Name  :  systemService
* Description    : system event service for event and key get
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void systemService(void)
{
	hal_wdtClear();
	taskSysScanDev(1);
	msgDeal();
	drawUIService(false);
}

void sendDrawUIMsg(void)
{
	msgDealByType(SYS_DRAW_UI,winGetCurrent(),0,NULL);
};


