/****************************************************************************
**
 **                              TASK DEMO
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                      TASK DEAMON
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : taskSysScanDevice.c
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

#define SDC_WAIT_TIME   50 // 500ms       
static int keyTime=0;
extern bool sysPowerOn;


INT32U g_md_delay_times=0;

void deamon_sdc_check(void);
void deamon_usb_check(void);
void deamon_gsensor_check(void);
void deamon_key_check(void);
void deamon_astern_check(void);
void deamon_night_effect(void);


void deamon_night_effect(void)
{
	if( !hal_gpioRead(GPIO_PB,GPIO_PIN6) )// need ir
	//if(IR_CHARGE_CHECK() == 0) //uart port, logic LOW ->LED ON
		cmos_spec_color(4);
	else
		cmos_spec_color(0);	

}


static void deamon_ir_auto_check(void)
{
#if (1 == IR_MENU_EN)
	uint32 irState=0;
	static uint8 ir_close_cnt=100;//100X10ms=1 s
	if(configGet(CONFIG_ID_IR_LED)!=R_ID_STR_IR_AUTO)
		return;
	if(isp_get_scen_light() < 0xa)		// need ir
	{
		boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&irState);
		if(irState==0)
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);

	}
	
	if(isp_get_scen_light() > 0x30)
	{
		ir_close_cnt--;
		if(ir_close_cnt==0)// wait for stable
		{
			boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&irState);
			if(irState)
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
			ir_close_cnt=100;
		}
	}
#endif
}
extern void resBuffConut(void);
extern void taskVideoRecTimeIncreace(void);
void timerMsgSend(void)
{
	static u8 lastSec=255;
	static uint32 lastTime = 0;
	HAL_RTC_T* rtcTime;
	rtcTime=hal_rtcTimeGet();
	if(lastSec!=rtcTime->sec)
	{
		lastSec=rtcTime->sec;
		SysCtrl.powerOnTime++;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_1S,0));
		taskVideoRecTimeIncreace();
		resBuffConut();
	}
	
	if((XOSTimeGet()-lastTime)<=500)
		return;
	lastTime = XOSTimeGet();
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_500MS,0));
}
/*******************************************************************************
* Function Name  : taskSysScanDev
* Description    : system service in deamon
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void taskSysScanDev(uint32 scanKey)
{
	static uint32 lastTime = 0;
	timerMsgSend();
	hal_ispService();
	#if CFG_MCU_TFDBG_EN
		hal_check_tfdebg();
	#endif

	
//--------key check -------------------
	if(scanKey)
		deamon_key_check();      // system key read
		
	if((XOSTimeGet()-lastTime)<=10*X_TICK)
		return ;
	lastTime = XOSTimeGet();
    if(taskCurrent() != TASK_POWER_OFF)
    {
	   // boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_CHECK,1);  // SYSTEM CHECK LED STATE
//-------sdc card check-----------------    
	    deamon_sdc_check();  // sdc state check
//--------gsensor check-----------------
	   // deamon_gsensor_check(); // gsensor state check

//--------clock check-----------------
        deamon_clock_check(0);

//--------sereen save check ------------	
	    deamon_screen_save(0);  // system check,no event
//--------auto power off check----------
	    deamon_auto_poweroff(0); //auto power off check 
//--------astern mode check-----------
        //deamon_astern_check(); // astern stat check
//--------ir auto check-------------
		//deamon_ir_auto_check();
    }
//--------usb check---------------------
	deamon_usb_check();  // usb state check

	if (HAL_CFG_EN_DBG ==0)
	{
		deamon_night_effect();
	}
//--------motion dection check----------
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if( hal_mdCheck() &&( windowIsOpen(&menuItemWindow) == 0 )&&( ( XOSTimeGet()-g_md_delay_times ) >=5*1000) )
		{
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_MD,1));
		}
	}
#if CMOS_USART_ONLINE_DBG >0
    sesnorDBGService();
#endif

    //LogShowCheck();
//	logFlush();
}
static bool keyEn=true;
void setKeyEn(bool en)
{
	keyEn=en;
}
/*******************************************************************************
* Function Name  : deamon_key_check
* Description    : key check value
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
uint g_switchValue_temp=0xff;

uint8 g_adc_flag=0;
void deamon_key_check(void)
{
//	static uint32 count = 0;
	static INT32U TimeInterval=10000;
	static uint32 lastTime = 0;
	static uint32 LastTime = 0;
//	INT32U CurTime;
	uint32 lcdOn=0;
	if((XOSTimeGet()-lastTime)<=10)
		return ;
	lastTime = XOSTimeGet();
	if(SysCtrl.bfd_key<0)
		return ;
	
	uint32 switchValue;
    if(boardIoctrl(SysCtrl.bfd_key,IOCTL_KEY_SWITCH,(INT32U )&switchValue)>=0)
    {
        //deg_Printf("11 switchValue:%x\n", switchValue);

        if(switchValue != g_switchValue_temp )
        {
        	g_switchValue_temp = switchValue;

			if( (!g_adc_flag) &&( switchValue == 0x5f || switchValue == 0x62 || switchValue == 0x6a ) )
			{
				g_adc_flag =1;
			}
			
            if(switchValue == 0x5f)//0X9f
            {
                deg_Printf("---------------------record\r\n");//1f
		
                switchValue = makeEvent(KEY_EVENT_RECORD,KEY_PRESSED);
            }
            else if(switchValue == 0x62)//0xa1
            {
                deg_Printf("---------------------mic\r\n");//0x21
                
                switchValue = makeEvent(KEY_EVENT_AUDIO,KEY_PRESSED);
            }
            else if(switchValue == 0x6a)//0xa8
            {
                deg_Printf("------------------save\r\n");//0x28
				
                switchValue = makeEvent(KEY_EVENT_SAVE,KEY_PRESSED);
            }

			deamon_clock_check(1);
			deamon_auto_poweroff(1);
			deamon_screen_save(1);


			
            boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
            //if(keyEn&&lcdOn)
            if(1)
        	{
        		if(windowIsOpen(&selfclockWindow))
    			{
    				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_RECORD,KEY_PRESSED));
    			}
				else
				{
        			XMsgQPost(SysCtrl.sysQ,(void*)switchValue);
				}
        	}
            else
            {
                //CHKPNT;
                //deamon_screen_save(1);
                if(getValue(switchValue)==KEY_RELEASE)
                    keyEn=true;
            }

           // keyTime =LastTime= XOSTimeGet();
           // TimeInterval=10000;
        }
    }

	int adcValue;
	if((boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_READ,(INT32U )&adcValue)>=0) && (g_adc_flag ))
	{
		if(adcValue ) // astern mode do not check key 
		{
			//debg("adcValue:%x\n",adcValue);

			//deamon_clock_check(1);
			//deamon_auto_poweroff(1);
			//deamon_screen_save(1);
			
			if(getType(adcValue)==KEY_EVENT_POWEROFF||getType(adcValue)==KEY_EVENT_POWER)
				XMsgQPost(SysCtrl.sysQ,(void*)adcValue);
			else
			{
				boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
			#if SYSTEM_BOARD == BOARD_TYPE_FPGA
				lcdOn = 1; //for key msg deal
			#endif 
				if(keyEn&&lcdOn)
					XMsgQPost(SysCtrl.sysQ,(void*)adcValue);
				else
				{
					//deamon_screen_save(1);
					if(getValue(adcValue)==KEY_RELEASE)
						keyEn=true;
				}
			}
			keyTime =LastTime= XOSTimeGet();
			TimeInterval=10000;
		}
	}
	

#if 0
	else
	{
	CurTime=XOSTimeGet();
	if(CurTime-LastTime>TimeInterval)
	{
		LastTime=CurTime;
		TimeInterval=500;
		if(CurTime&2)
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_MODE,KEY_PRESSED));
		if(CurTime&1)
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_MENU,KEY_PRESSED));
		else
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK,KEY_PRESSED));
	}
}
#endif
}


/*******************************************************************************
* Function Name  : updata_rtc_from_txt()
* Description	 :  updata rtc time with time.txt file
* Input 		 : 
* Output		 :
* Return		 : 
*******************************************************************************/
static void updata_rtc_from_txt()
{
	u32 ret;
	FIL rtc_file;
	DATE_TIME_T rtc_time,default_txt_time,txt_time;
	u8 txt_timebuf[]        = "2022-04-13,23:59:59"; //"2020-05-23,14:59:59";
	u8 rtc_defaulttimebuf[] = "2022-04-13,23:59:59";//"2020-05-23,14:59:59";
	ret =  f_open(&rtc_file,(char const *)UPDATE_RTC_TIME_FILE, FA_WRITE|FA_READ);
	if(ret == FR_OK)		//file on
	{
		u32 read;
		deg_Printf("fopen rtc file	ok\n");
		f_read(&rtc_file,&txt_timebuf, sizeof(txt_timebuf),&read);
		f_close(&rtc_file);
		deg_Printf("fread rtc file size=%d,string=%s\n",read,txt_timebuf);

		//convert txt time
		txt_time.year = 1000*(txt_timebuf[0]-'0')+100*(txt_timebuf[1]-'0')+10*(txt_timebuf[2]-'0')+(txt_timebuf[3]-'0');
		txt_time.month = 10*(txt_timebuf[5]-'0')+(txt_timebuf[6]-'0');
		txt_time.day =10*(txt_timebuf[8]-'0')+(txt_timebuf[9]-'0');
		txt_time.hour=10*(txt_timebuf[11]-'0')+(txt_timebuf[12]-'0');
		txt_time.min =10*(txt_timebuf[14]-'0')+(txt_timebuf[15]-'0');
		txt_time.sec =10*(txt_timebuf[17]-'0')+(txt_timebuf[18]-'0');

		//convert default time
		default_txt_time.year = 1000*(rtc_defaulttimebuf[0]-'0')+100*(rtc_defaulttimebuf[1]-'0')+10*(rtc_defaulttimebuf[2]-'0')+(rtc_defaulttimebuf[3]-'0');
		default_txt_time.month = 10*(rtc_defaulttimebuf[5]-'0')+(rtc_defaulttimebuf[6]-'0');
		default_txt_time.day =10*(rtc_defaulttimebuf[8]-'0')+(rtc_defaulttimebuf[9]-'0');
		default_txt_time.hour=10*(rtc_defaulttimebuf[11]-'0')+(rtc_defaulttimebuf[12]-'0');
		default_txt_time.min =10*(rtc_defaulttimebuf[14]-'0')+(rtc_defaulttimebuf[15]-'0');
		default_txt_time.sec =10*(rtc_defaulttimebuf[17]-'0')+(rtc_defaulttimebuf[18]-'0');

	    //compare txt_time default time
	    // if txt_time != default_txt_time   change time by txt
	    if((default_txt_time.year ==txt_time.year) && 
			(default_txt_time.month==txt_time.month) &&
			(default_txt_time.day ==txt_time.day)&&
			(default_txt_time.hour == txt_time.hour)&&
			(default_txt_time.min == txt_time.min)&&
			(default_txt_time.sec == txt_time.sec))
	    {
	    	//hal_rtcTimeSet(SysRTCTime); // no change ;
	    	deg_Printf("[Manual]system time don't need update ...\n" );

	}	
		else // use txt time as system time
		{ 
			hal_rtcTimeSet(&txt_time);	//update rtc time
			deg_Printf("[Manual]system time update from txt %s\n",txt_timebuf);

			//txt time write default time
			u32 writed; 
			ret =  f_open(&rtc_file,(char const *)UPDATE_RTC_TIME_FILE, FA_WRITE | FA_READ);
			f_write(&rtc_file,&rtc_defaulttimebuf, sizeof(rtc_defaulttimebuf),&writed);
			f_close(&rtc_file);    
			deg_Printf("[Manual]update default txt...\n" );

		}

		
	}	
	else			//no TIME.txt file ,creat new one
	{
		u32 writed;
		ret =  f_open(&rtc_file,(char const *)UPDATE_RTC_TIME_FILE, FA_CREATE_NEW| FA_WRITE | FA_READ);
		deg_Printf("create rtc file ret = %d\n",ret);
		if(FR_OK != ret)
		{
			return FALSE;
		}
		f_write(&rtc_file,&rtc_defaulttimebuf, sizeof(rtc_defaulttimebuf),&writed);
		f_close(&rtc_file);
		deg_Printf("fwrite rtc file size %d\n",writed);

		DATE_TIME_T *SysRTCTime = hal_rtcTimeGet();	
				deg_Printf("Get System RTC Time: %d-%d-%d %d:%d:%d\n",SysRTCTime->year,SysRTCTime->month,SysRTCTime->day,
					      SysRTCTime->hour,SysRTCTime->min,SysRTCTime->sec);
				
		//convert default time
		default_txt_time.year = 1000*(rtc_defaulttimebuf[0]-'0')+100*(rtc_defaulttimebuf[1]-'0')+10*(rtc_defaulttimebuf[2]-'0')+(rtc_defaulttimebuf[3]-'0');
		default_txt_time.month = 10*(rtc_defaulttimebuf[5]-'0')+(rtc_defaulttimebuf[6]-'0');
		default_txt_time.day =10*(rtc_defaulttimebuf[8]-'0')+(rtc_defaulttimebuf[9]-'0');
		default_txt_time.hour=10*(rtc_defaulttimebuf[11]-'0')+(rtc_defaulttimebuf[12]-'0');
		default_txt_time.min =10*(rtc_defaulttimebuf[14]-'0')+(rtc_defaulttimebuf[15]-'0');
		default_txt_time.sec =10*(rtc_defaulttimebuf[17]-'0')+(rtc_defaulttimebuf[18]-'0');
			
	    //compare rtc_time default time
	    if((SysRTCTime->year >default_txt_time.year)||
			((SysRTCTime->year ==default_txt_time.year) && (SysRTCTime->month > default_txt_time.month))||
			((SysRTCTime->year ==default_txt_time.year) && (SysRTCTime->month==default_txt_time.month) &&(SysRTCTime->day > default_txt_time.day))||
			((SysRTCTime->year ==default_txt_time.year) && (SysRTCTime->month==default_txt_time.month) &&(SysRTCTime->day ==default_txt_time.day)&&(SysRTCTime->hour > default_txt_time.hour))||
			((SysRTCTime->year ==default_txt_time.year) && (SysRTCTime->month==default_txt_time.month) &&(SysRTCTime->day ==default_txt_time.day)&&(SysRTCTime->hour == default_txt_time.hour)&&(SysRTCTime->min> default_txt_time.min))||
			((SysRTCTime->year ==default_txt_time.year) && (SysRTCTime->month==default_txt_time.month) &&(SysRTCTime->day ==default_txt_time.day)&&(SysRTCTime->hour == default_txt_time.hour)&&(SysRTCTime->min == default_txt_time.min)&&(SysRTCTime->sec > default_txt_time.sec))) // use rtc time
	{
	    	hal_rtcTimeSet(SysRTCTime); // use rtc new time;
	    	deg_Printf("[Auto]update rtc time from system ...\n" );

	}
		else // use txt default time 
	{
			hal_rtcTimeSet(&default_txt_time);	//write rtc time
			deg_Printf("[Auto]update rtc time from txt %s\n",rtc_defaulttimebuf);

		}
	}

	return TRUE;
}


typedef struct INFO_TXT{
		u8 value;
		u8 name[10];
}info_txt;

info_txt info_buf[5]; //info_txt info_buf[4];

#define INFO_LEN  sizeof(info_buf)/sizeof(info_txt)
void info_value_init(u8 *name, u8 val, u8 index)
{
	u8 i;
	for(i=0; i<INFO_LEN; i++)
	{
		if(i == index)
		{
			//deg_Printf("name=%s \n",name); NULL 0
			strcpy(info_buf[i].name, name);
			info_buf[i].value = val;
			break;
		}
	}
	
}

void set_info_value(u8 *name, u8 val)
{
	u8 i;
	for(i=0; i<INFO_LEN; i++)
	{
		if(strcmp(info_buf[i].name, name) == 0)
		{
			//strcpy(info_buf[i].name, name);
			info_buf[i].value = val;
			break;
		}
	}
}

u8 get_info_value(u8 *name)
{
	u8 i;
	for(i=0; i<INFO_LEN; i++)
	{
		if(strcmp(info_buf[i].name, name) == 0)
		{
			//deg_Printf("i=%d name=%s \n", i, name);
			break;
		}
	}
	return info_buf[i].value;
}

u8 resolution, recycle, motion,time, filetime;//u8 resolution, recycle, motion,time;

int updata_settings_menu(void)//(resolution, recycle, motion,time, filetime);
{
    CHKPNT;
    deg_Printf("p1=%d, p2=%d, p3=%d, p4=%d\r\n", get_info_value("1080"),get_info_value("recycle"), get_info_value("time"), get_info_value("filetime"));
	if( get_info_value("1080") == 0)
        configSet(CONFIG_ID_RESOLUTION,R_ID_STR_RES_FHD);
	else if(get_info_value("1080") == 1)
        configSet(CONFIG_ID_RESOLUTION,R_ID_STR_RES_HD);
	else if(get_info_value("1080") == 2)
        configSet(CONFIG_ID_RESOLUTION,R_ID_STR_RES_VGA);
	else
        configSet(CONFIG_ID_RESOLUTION,R_ID_STR_RES_FHD);

	if(motion)
	{
        configSet(CONFIG_ID_MOTIONDECTION,R_ID_STR_COM_ON);
	}
    else
    {
        configSet(CONFIG_ID_MOTIONDECTION,R_ID_STR_COM_OFF);
    }

    if(get_info_value("time"))
    {
        configSet(CONFIG_ID_TIMESTAMP,R_ID_STR_COM_ON);
    }
    else
    {
        configSet(CONFIG_ID_TIMESTAMP,R_ID_STR_COM_OFF);
    }

    return 0;
}


#if UPDATE_SETTING_EN

u8 txt[]="1080P:0 RECYCLE:1 TIME:1 FILETIME:10 ";
u8 settings_buf[50] = {0};

//create new txt and write default buf
static void updata_settings_txt_default(void)
{
	u32 ret;
    u32 writed;
	FIL setting_fil;

    ret =  f_open(&setting_fil,(char const *)UPDATE_SETTING_FILE, FA_CREATE_NEW| FA_WRITE | FA_READ);
    deg_Printf("create settings file ret = %d\n",ret);
    if(FR_OK != ret)
    {
        return FALSE;
    }

    f_write(&setting_fil,&txt, sizeof(txt),&writed);
    f_close(&setting_fil);
    deg_Printf("fwrite settings file size %d\n",writed);

    ret = f_open(&setting_fil, (char const*)UPDATE_SETTING_FILE, FA_WRITE|FA_READ);
    if(ret == FR_OK)
    {
        u32 read;
        deg_Printf("open settings file success \n");
        f_read(&setting_fil, &settings_buf, sizeof(settings_buf), &read);
        f_close(&setting_fil);
        deg_Printf("read size=%d buf=%s \n", read, settings_buf);
    }
}

static void updata_settings_set_default_param(void)
{
    // u8 txt[]="1080P:0 RECYCLE:1 MOTION:0 TIME:1 FILETIME:5 ";
    resolution = 0; recycle = 1; time = 0; filetime = 10;
}

static void updata_settings_init_param(void)
{
    info_value_init("1080", resolution, 0);
    info_value_init("recycle", recycle, 1);
    //info_value_init("motion", motion, 2);
    info_value_init("time" , time, 3);
    info_value_init("filetime" , filetime, 4);
}


//write init fro txt
static void updata_settings_write_param(void)
{
    // u8 txt[]="1080P:0 RECYCLE:1 MOTION:0 TIME:1 FILETIME:5 ";
	u8 i, pos;
    u8 buf1[10] = {0}, buf2[10] = {0}, buf3[10] = {0}, buf4[10] = {0};

    // 1 format, no 5 param 
    // i=5/i=15/i=24/i=31/i=42 example: "1080P:0aRECYCLabcbMOTIOdefcTIME:1dFILETIME:5 "
    if((settings_buf[5] != ':') || (settings_buf[15] != ':') || (settings_buf[22] != ':') \
        || (settings_buf[33] != ':') )
    {
        updata_settings_set_default_param();
        f_unlink(UPDATE_SETTING_FILE);
        updata_settings_txt_default();
        CHKPNT;
        return;
    }

	for(i=0; i<50; i++)
    {
        if(i < 5)
        {
            buf1[i] = settings_buf[i];
        }
        else if(i < 15)
        {
            buf2[i-(5+3)] = settings_buf[i];
        }
        else if(i < 23)
        {
            buf3[i-(15+3)] = settings_buf[i];
        }
        else if(i < 33)
        {
            buf4[i-(23+3)] = settings_buf[i];
        }
       
    }

    deg_Printf("buf1=%s,buf2=%s,buf3=%s,buf4=%s\n", \
        buf1, buf2, buf3, buf4);

    // 2 param str err
    //compare str1--str5 example: "1aa0P:0 RECbbLE:1 MOccON:0 TddE:1 FILEeeME:5 "
    if((strncmp(buf1, "1080P", sizeof("1080P")) != 0) || (strncmp(buf2, "RECYCLE", sizeof("RECYCLE")) != 0) \
        || (strncmp(buf3, "TIME", sizeof("TIME")) != 0) || (strncmp(buf4, "FILETIME", sizeof("FILETIME")) != 0))
    {
        updata_settings_set_default_param();
        f_unlink(UPDATE_SETTING_FILE);
        updata_settings_txt_default();
        CHKPNT;
        return;
    }

#if 0 //blank space char need compare? open : #if 0
    // 3 lack blank space
    // example: "1080P:0aRECYCLE:1bMOTION:0cTIME:1dFILETIME:5 "
    if((settings_buf[5+1] != ' ') || (settings_buf[15+1] != ' ') || (settings_buf[24+1] != ' ') \
        || (settings_buf[31+1] != ' ') /*|| (settings_buf[24+1] != '')*/)
    {
        updata_settings_set_default_param();
        updata_settings_txt_default();
        CHKPNT;
        return;
    }
#endif

    // 4 str beyond, resolve 50 and delete other
    //f_size(fp)


	pos = 0;
	for(i=0; i<50; i++)
	{
		if(settings_buf[i] == ':')
		{
            //deg_Printf("xxx_i=%d\r\n", i);
			pos++;
			if(pos == 1)
			{
				resolution = settings_buf[i+1]-'0';
				//info_value_init("1080", resolution, 0);
			}
			else if(pos == 2)
			{
				recycle = settings_buf[i+1]-'0';
				//info_value_init("recycle", recycle, 1);
                if(recycle != 1)
                {
                    CHKPNT;
                    recycle = 0;
                }
			}
			else if(pos == 3)
			{
				time = settings_buf[i+1]-'0';
				//info_value_init("time" , time, 3);
                if(time != 1)
                {
                    CHKPNT;
                    time = 0;
                }
			}
			else if(pos == 4)
			{
				//filetime = settings_buf[i+1]-'0';
			#if 0
				if((settings_buf[i+1]-'0') ==0) //0xxx
				{
					filetime = 5;
				}
				else if((settings_buf[i+2] > '5') || (settings_buf[i+2] < '0')) //(1-9)xxx
				{
					filetime = settings_buf[i+1]-'0';
				}
				else
				{
					if((settings_buf[i+1]-'0') == 1)
						filetime = (settings_buf[i+1]-'0')*10 + (settings_buf[i+2]-'0');
					else 
						filetime = 5;
				}
			#else
				if( (settings_buf[i+1] >= '0') && (settings_buf[i+1] <= '9') && (settings_buf[i+2] >= '0') && (settings_buf[i+9] <= '9') )
				{
					filetime = (settings_buf[i+1]-'0')*10 + (settings_buf[i+2]-'0');
				}

			#endif

				//info_value_init("filetime" , filetime, 4);
			}
		}
	}
}

static void updata_settings_from_txt(void)
{		
	u32 ret;
	FIL setting_fil;

	memset(info_buf, sizeof(info_buf), 0);
	ret = f_open(&setting_fil, (char const*)UPDATE_SETTING_FILE, FA_WRITE|FA_READ);
	if(ret == FR_OK)
	{
		u32 read;
		deg_Printf("open settings file success \n");
		f_read(&setting_fil, &settings_buf, sizeof(settings_buf), &read);
		f_close(&setting_fil);
		deg_Printf("read size=%d buf=%s \n", read, settings_buf);
        updata_settings_write_param();
	}
	else //not find file ,creat new one
	{
        CHKPNT;
        //deg_Printf("11settings_buf=%s\r\n", settings_buf);
        updata_settings_txt_default();
        updata_settings_set_default_param();
        deg_Printf("22settings_buf=%s\r\n", settings_buf);
	}

    updata_settings_init_param();
	deg_Printf("resolution=%d recycle=%d time=%d, filetime=%d\n", resolution, recycle, time, filetime);
    updata_settings_menu();
}

#endif


#if 0
static void updata_settings_from_txt(void)
{		
	u8 i, pos;
	u32 ret;
	FIL setting_fil;
	u8 settings_buf[50];
	//u8 txt[]="1080P:0 RECYCLE:1 MOTION:0 TIME:1";
	u8 txt[]="1080P:0 RECYCLE:1 MOTION:0 TIME:1 FILETIME:5 ";

	memset(info_buf, sizeof(info_buf), 0);
	ret = f_open(&setting_fil, (char const*)UPDATE_SETTING_FILE, FA_WRITE|FA_READ);
	if(ret == FR_OK)
	{
		u32 read;
		deg_Printf("open settings file success \n");
		f_read(&setting_fil, &settings_buf, sizeof(settings_buf), &read);
		//比较数据格式 1080: RECYCLE: MOTION TIME
		f_close(&setting_fil);
		deg_Printf("read size=%d buf=%s \n", read, settings_buf);
	}
	else			//not find file ,creat new one
	{
		u32 writed;
		ret =  f_open(&setting_fil,(char const *)UPDATE_SETTING_FILE, FA_CREATE_NEW| FA_WRITE | FA_READ);
		deg_Printf("create rtc file ret = %d\n",ret);
		if(FR_OK != ret)
		{
			return FALSE;
		}
		f_write(&setting_fil,&txt, sizeof(txt),&writed);
		f_close(&setting_fil);
		deg_Printf("fwrite rtc file size %d\n",writed);

		ret = f_open(&setting_fil, (char const*)UPDATE_SETTING_FILE, FA_WRITE|FA_READ);
		if(ret == FR_OK)
		{
			u32 read;
			deg_Printf("open settings file success \n");
			f_read(&setting_fil, &settings_buf, sizeof(settings_buf), &read);
			//比较数据格式 1080: RECYCLE: MOTION TIME
			f_close(&setting_fil);
			deg_Printf("read size=%d buf=%s \n", read, settings_buf);
		}
	}
	
	pos = 0;
	//for(i=0; i<40; i++)
	for(i=0; i<50; i++)
	{
		if(settings_buf[i] == ':')
		{
			pos++;
			if(pos == 1)
			{
				resolution = settings_buf[i+1]-'0';
				info_value_init("1080", resolution, 0);
			}
			else if(pos == 2)
			{
				recycle = settings_buf[i+1]-'0';
				info_value_init("recycle", recycle, 1);
			}
			else if(pos == 3)
			{
				motion = settings_buf[i+1]-'0';
				info_value_init("motion", motion, 2);
			}
			else if(pos == 4)
			{
				time = settings_buf[i+1]-'0';
				info_value_init("time" , time, 3);
			}
			else if(pos == 5)
			{
				//filetime = settings_buf[i+1]-'0';
				if((settings_buf[i+1]-'0') ==0) //0xxx
				{
					filetime = 5;
				}
				else if((settings_buf[i+2] > '5') || (settings_buf[i+2] < '0')) //(1-9)xxx
				{
					filetime = settings_buf[i+1]-'0';
				}
				else
				{
					if((settings_buf[i+1]-'0') == 1)
						filetime = (settings_buf[i+1]-'0')*10 + (settings_buf[i+2]-'0');
					else 
						filetime = 5;
				}

				info_value_init("filetime" , filetime, 4);
			}
		}
	}

	deg_Printf("resolution=%d recycle=%d motion=%d time=%d, filetime=%d\n", resolution, recycle, motion, time, filetime);
    updata_settings_menu();
}

#endif

void deamon_fs_scan(void)
{
	deg_Printf("deamon : fs mount start.%d\n",XOSTimeGet());
#if (1 == UPDATE_RTC_TIME_EN)
	updata_rtc_from_txt();
#endif
#if (1 == UPDATE_SETTING_EN)
        updata_settings_from_txt();
#endif	
	
	char string[16];
#if(AUDIO_REC_EN==1)
	strcpy(string,FILEDIR_AUDIO);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);//FILEDIR_AUDIO);
#endif
	strcpy(string,FILEDIR_VIDEOA);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*) string);//FILEDIR_VIDEO);

#if (1 == AVI_LOCK_FOLDER)
	strcpy(string,FILEDIR_LOCKA);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*) string);
	strcpy(string,FILEDIR_LOCKB);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*) string);
#endif

#if RECORD_MODE == RECORD_MODE_DEFAULT	
	strcpy(string,FILEDIR_VIDEOB);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);//FILEDIR_VIDEO);
	SysCtrl.bfolder = 1;
#elif RECORD_MODE == RECORD_MODE_AUTO	
    if(0) //if(SysCtrl.usensor!=USENSOR_STAT_NULL)
    {
		strcpy(string,FILEDIR_VIDEOB);
		string[strlen(string)-1] = 0;
		f_mkdir((const TCHAR*)string);//FILEDIR_VIDEO);
		SysCtrl.bfolder = 1;
    }
	else
		SysCtrl.bfolder = 0;
#else
    // no B floder
    SysCtrl.bfolder = 0;
#endif

	strcpy(string,FILEDIR_PHOTO);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);//FILEDIR_PHOTO);
	hal_sdStop();					
    
	SysCtrl.fs_clustsize = fs_getclustersize();
	deg_Printf("deamon : fs cluster size.%d B\n",SysCtrl.fs_clustsize);
	SysCtrl.sdcard = SDC_STAT_NORMAL; 
    XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
	if(SysCtrl.avi_list<0) // scan file list
	{
		SysCtrl.avi_list = managerCreate(FILEDIR_VIDEO,MA_FILE_AVI,-1);
		SysCtrl.jpg_list = managerCreate(FILEDIR_VIDEO,MA_FILE_JPG,SysCtrl.avi_list); // using the same list with avi_list
		SysCtrl.avia_list = managerCreate(FILEDIR_VIDEO,MA_FILE_AVIA,SysCtrl.avi_list); // using the same list with avi_list
		SysCtrl.avib_list = managerCreate(FILEDIR_VIDEO,MA_FILE_AVIB,SysCtrl.avi_list); // using the same list with avi_list
		managerScanStart(SysCtrl.avi_list);
		managerFileScan(SysCtrl.avi_list,FILEDIR_VIDEOA);
		managerFileScan(SysCtrl.avi_list,FILEDIR_VIDEOB);
		#if (1 == AVI_LOCK_FOLDER)
		managerFileScan(SysCtrl.avi_list,FILEDIR_LOCKA);
		managerFileScan(SysCtrl.avi_list,FILEDIR_LOCKB);
		#endif

		if(strcmp(FILEDIR_PHOTO,FILEDIR_VIDEO)!=0)
			managerFileScan(SysCtrl.jpg_list,FILEDIR_PHOTO);
		managerScanEnd(SysCtrl.avi_list);
	}
#if(AUDIO_REC_EN==1)
	if(SysCtrl.wav_list<0)
    {
	#if (1 == MP3_DECODE_SURPORT)
		mp3_name_list_init();
	#endif
		
		SysCtrl.wav_list = managerCreate(FILEDIR_AUDIO,MA_FILE_WAV,-1);
		deg_Printf("SysCtrl.wav_list %d\n",SysCtrl.wav_list);
		managerScanStart(SysCtrl.wav_list);
		managerFileScan(SysCtrl.wav_list ,FILEDIR_AUDIO);
		managerScanEnd(SysCtrl.wav_list);
    }
#endif
	//if(menuActiveGet())
	{
		deamon_fsFreeSize();
		task_video_record_caltime();
	}/**/
	hal_sdUnlock();
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
	deg_Printf("deamon : sdc normal\n",XOSTimeGet());
}
/*******************************************************************************
* Function Name  : deamon_sdc_check
* Description    : sd card stat check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_sdc_check(void)
{
	static int sdccnt=SDC_WAIT_TIME/*the initiled value is used when power on*/,scanFlag=0/*,scantime=0*/;
	int temp,res;

	if(scanFlag /*&& (task_get_curstate()!=MAIN_STAT_POWER_ON)*/)
	{
	//	if(scanFlag)
	//		scantime++;
	//	if(scantime>=10)
		{
			deamon_fs_scan();
			scanFlag = 0;
	//		scantime= 0;
		}
	}
//-------------mark modify,2017/11/30.when sdc pulled in,the power of sdc may not stable,wait about 500ms	
    if(SysCtrl.bfd_sdcard<0)
		return ;
	boardIoctrl(SysCtrl.bfd_sdcard,IOCTRL_SDC_CHECK,(INT32U)&temp);
	if(SysCtrl.sdcard <= SDC_STAT_UNSTABLE)
	{
		if(hal_sdInit(SD_BUS_WIDTH4)>=0)
		{
			if(SysCtrl.sdcard == SDC_STAT_NULL)
			{
			    SysCtrl.sdcard = SDC_STAT_UNSTABLE;
				deg_Printf("deamon : sdc pulled in.\n");
			}
			else if(SysCtrl.sdcard == SDC_STAT_UNSTABLE)
			{
			    sdccnt++;
				if(sdccnt<SDC_WAIT_TIME)  // 500 ms,wait sdc stable
				    return ;
				sdccnt = 0;
				deg_Printf("deamon : sdc stable.\n");
				res = fs_mount(0);
				if(res!=FR_OK)
				{
					hal_sdSetCardState(SDC_STATE_NULL);
					if(hal_sdInit(SD_BUS_WIDTH4)>=0) // sdc intial 1line
					{
						res = fs_mount(0);
					}
				}

				if(res<0)
				{
					SysCtrl.sdcard = SDC_STAT_ERROR; 
					XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
					deg_Printf("deamon : sdc error.%d\n",res);
				}
				else
				{
                    scanFlag = 1;
					SysCtrl.sdcard = SDC_STAT_UNSTABLE; 
					deg_Printf("deamon : sdc mounted ok\n");
				}
		    }
			temp = 1;
		}
	}
	if(temp==0) // no sdc dectcted
	{
		if(SysCtrl.sdcard == SDC_STAT_NULL)
		{
			if(sdccnt>=SDC_WAIT_TIME)
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
		}
		else if(SysCtrl.sdcard == SDC_STAT_UNSTABLE)
		{
			deg_Printf("deamon : sdc pulled out\n");
			SysCtrl.sdcard = SDC_STAT_NULL;
		}
		else// if(SysCtrl.sdcard!=SDC_STAT_NULL)
		{
			fs_nodeinit();  // initial fs node
			managerDestory(SysCtrl.avi_list);
			managerDestory(SysCtrl.jpg_list);
			managerDestory(SysCtrl.avia_list);
			managerDestory(SysCtrl.avib_list);
			#if(AUDIO_REC_EN==1)
			managerDestory(SysCtrl.wav_list);
			#endif
			SysCtrl.avi_list = -1;
			SysCtrl.jpg_list = -1;
			SysCtrl.avia_list = -1;
			SysCtrl.avib_list = -1;
			SysCtrl.wav_list = -1;
		    SysCtrl.sdcard = SDC_STAT_NULL;  // sdc out
		    XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
			deamon_fsFreeSize();
		    task_video_record_caltime();
		    deg_Printf("deamon : sdc unmounted.\n");
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
		}
		sdccnt = 0;
	}
	
}

/*******************************************************************************
* Function Name  : deamon_sdc_check
* Description    : sd card stat check when uvc+msdc remove to video
                   remove sd and insert again
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static bool Usb_To_Video_Flag = FALSE;
BOOL get_usb_to_video_flag(void)
{
    return Usb_To_Video_Flag;
}

void set_usb_to_video_flag(bool StateFlag)
{
    Usb_To_Video_Flag = StateFlag;
}

void deamon_usb_to_video_check_card(void)
{
    if(get_usb_to_video_flag())
    {
        set_usb_to_video_flag(FALSE);

        fs_nodeinit();  // initial fs node
        managerDestory(SysCtrl.avi_list);
        managerDestory(SysCtrl.jpg_list);
        managerDestory(SysCtrl.avia_list);
        managerDestory(SysCtrl.avib_list);
    #if(AUDIO_REC_EN==1)
        managerDestory(SysCtrl.wav_list);
    #endif
        SysCtrl.avi_list = -1;
        SysCtrl.jpg_list = -1;
        SysCtrl.avia_list = -1;
        SysCtrl.avib_list = -1;
        SysCtrl.wav_list = -1;
        SysCtrl.sdcard = SDC_STAT_NULL;  // sdc out
        deamon_sdc_check();
    }
}


/*******************************************************************************
* Function Name  : deamon_astern_check
* Description    : astern mode & backrec status 
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_astern_check(void)
{
	bool ret;

	api_backrec_linking();	

	ret = api_astern_status();
	if((SysCtrl.astern==ASTERN_OFF) && (ret == true))
	{
		//deg_Printf("deamon : astern on\n");
		//SysCtrl.astern = ASTERN_ON;
		//XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_ASTERN,ASTERN_ON));
	}
	else if((SysCtrl.astern==ASTERN_ON) && (ret == false))
	{
		//deg_Printf("deamon : astern off\n");
		//SysCtrl.astern = ASTERN_OFF;
		//XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_ASTERN,ASTERN_OFF));
	}

	ret = api_backrec_status();
	if((SysCtrl.usensorshow==USENSOR_SHOW_OFF) && (ret == true))
	{
		deg_Printf("deamon : uensor show on\n");
		SysCtrl.usensorshow = USENSOR_SHOW_ON;
		SysCtrl.t_rec = 0;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USENSOR,USENSOR_SHOW_ON));
	}
	else if((SysCtrl.usensorshow==USENSOR_SHOW_ON) && (ret == false))
	{
		deg_Printf("deamon : uensor show off\n");
		SysCtrl.usensorshow = USENSOR_SHOW_OFF;
		SysCtrl.t_rec = 0;
		api_astern_set_status(false);
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USENSOR,USENSOR_SHOW_OFF));
	}
}
void reflush_usensor_check(void)
{
	boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
	deg_Printf("{deamon : usensor out}\n");
	SysCtrl.usensor=USENSOR_STAT_NULL;	
	SysCtrl.usensorshow = USENSOR_SHOW_OFF;
	SysCtrl.t_rec = 0;
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_USENSOR,USENSOR_STAT_NULL));
}
/*******************************************************************************
* Function Name  : deamon_usb_check
* Description    : usb & battery stat check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
extern int uninit(void);
static BOOL softwareDetachUsensor=FALSE;
void setSoftwareDetachUsensor(BOOL en)
{
	softwareDetachUsensor=en;
}
extern s32 usb11HostCheckNoWait(void);
void deamon_usb_check(void)
{
	static INT32U usbdelay = 0;
	static INT32U usensordect_time = 0, set_power = 0;
	static INT8S checkTime = 1,checkStable=0,usbdev=0;
	static INT32U nopower=0;
	static INT8U lastBatLevel=BATTERY_STAT_5;
	int temp,ret;
	if(usensordect_time == 0)
		usensordect_time =  XOSTimeGet();
#if 0 //#if RECORD_MODE != RECORD_MODE_SIGNAL
//-------------------usensor detect----------------------
	ret = boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(ret>=0)
	{
		if(((temp ==0)&&(SysCtrl.usensor!=USENSOR_STAT_NULL))||softwareDetachUsensor)
		{
		//	sysLog("out");
            boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
			set_power = 0;
			usensordect_time = XOSTimeGet();
			if(softwareDetachUsensor==FALSE)
				deg_Printf("deamon : usensor out\n");
			SysCtrl.usensor=USENSOR_STAT_NULL;
			backrec_sensor(0);
			usbdelay = 0;
		}
		else if(temp)
		{
			if(SysCtrl.usensor==USENSOR_STAT_NULL)
			{
			#if (AX32_PLATFORM == AX3292)
				if(set_power == 0){
					if(XOSTimeGet()>usensordect_time+500)
					{
						boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,1);
						set_power =1;
						usensordect_time = XOSTimeGet();
					}
				}
				if((XOSTimeGet()>usensordect_time+100)&&(set_power==1)){
				#if IO1DO1_USE_SOFTSTART
					hal_io1d1_pd_enable(IO1_CH_PA7,1);
				#endif
				}
				if((XOSTimeGet()>usensordect_time+500)&&(set_power==1))
				{
					
					//usensordect_time = XOSTimeGet();
					s32 check_flag = usb11HostCheckNoWait();
					if(-1 != check_flag)
					{
						if(check_flag)
			#endif
						{
							usbdelay = XOSTimeGet();
							SysCtrl.usensor=USENSOR_STAT_UNSTABLE;
							deg_Printf("deamon : usensor in\n");
						}
			#if (AX32_PLATFORM == AX3292)
						else
						{
							boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
							set_power = 0;
							usensordect_time = XOSTimeGet();
						}
					}
				}
			#endif	
			}
			else if((SysCtrl.usensor==USENSOR_STAT_UNSTABLE) &&((usbdelay+100)<XOSTimeGet()))
			{
				usbdelay = 0;
			#if (AX32_PLATFORM != AX3292)
                boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,1);
			    deg_Printf("deamon : usensor power on\n");
			#endif
			    SysCtrl.usensor=USENSOR_STAT_IN;
			    backrec_sensor(1);
			#if (AX32_PLATFORM == FPGA)
				u8 usb_type = usb_host_detect();
			#elif (AX32_PLATFORM == AX3282S)
				u8 usb_type = 2; //USB2.0
			#elif ((AX32_PLATFORM == AX3292)||(AX32_PLATFORM == AX3291A))
				u8 usb_type = 1; //USB1.1
			#else
				#error "please check usensor usb type!"
			#endif
				usb_host_type_set(usb_type);
				if(usb_type ==1)
					hal_husb11_init_typeA();
				else if(usb_type ==2)
					hal_husb_init_typeA();
				else
					SysCtrl.usensor=USENSOR_STAT_NULL;
			}
		}
		/*else if((temp)&&(SysCtrl.usensor!=USENSOR_STAT_IN))
		{
			boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
			XOSTimeDly(200);
            boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,1);
			sysLog("in");
			deg_Printf("deamon : usensor in\n");
			SysCtrl.usensor=USENSOR_STAT_IN;
			backrec_sensor(1);
			hal_husb_init_typeA();
		}*/
	}
#endif	
//--------------------usb detect------------------------
	ret = boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(ret>=0)
    {	
		if((temp == 0) && (SysCtrl.usb !=USB_STAT_NULL)) // dc out
		{			
		    SysCtrl.usb = USB_STAT_NULL;  // dc out
			checkStable = 5; // wait stable
			usbdev = 0;
			usbdev_reset();
			deg_Printf("usb out\n");
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
		}
		else if(temp&& (SysCtrl.usb ==USB_STAT_NULL)) // dc in
		{
			SysCtrl.t_usb = USB_STAT_DCIN;
		#if (AX32_PLATFORM == AX3292)
				ret = hal_usbdCheck();
		#else
			if(SysCtrl.usensor == USENSOR_STAT_NULL)
				ret = hal_usbdCheck();
			else
				ret = -1;
		#endif
			if(ret>=0)
			{
				hal_usbdInit(0);
			}

		 	SysCtrl.usb = USB_STAT_DCIN;
			SysCtrl.battery = lastBatLevel=BATTERY_STAT_5;
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
			nopower = 0;
		}
		else
		{
			if((SysCtrl.usb == USB_STAT_DCIN)&&usbdev_rqu()&&(usbdev==0))
			{
				SysCtrl.usb = USB_STAT_PC;
				usbdev = 1;
				lastBatLevel=BATTERY_STAT_5;
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
				//hal_usbdUninit();
			}

			//deg_Printf("-----BAT_CHARGE_CHECK()=%d-------.\n",BAT_CHARGE_CHECK());
            if(BAT_CHARGE_CHECK())      // bat full
            {
                SysCtrl.battery = BATTERY_STAT_MAX;
            }
		}
    }
//----------------------battery detect---------------------------------------	
	if(SysCtrl.usb == USB_STAT_NULL) // only dc out check battery
	{
		checkTime--;
		if(checkTime)
			return ;
		checkTime = 1;
	    ret = boardIoctrl(SysCtrl.bfd_battery,IOCTRL_BAT_CHECK,(INT32U)&temp);  
		if(ret>=0)
		{
			if(SysCtrl.battery != temp&&checkStable==0) // need battery stable
				checkStable = 5;//  3;
			else if(SysCtrl.battery == temp)
			{
				checkStable=0;
				return ; // no need update
			}
			if(checkStable>0)
				checkStable--;
            
			if(checkStable == 0)
			{
				if(temp>lastBatLevel)
					return;
				lastBatLevel=temp;
				SysCtrl.battery = temp;
				if(temp == 0)
				{
					taskStart(TASK_POWER_OFF,0);	
				}
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_BAT,SysCtrl.battery));
				deg_Printf("battery = %x\n",SysCtrl.battery);
			}
		}
	}
}
/*******************************************************************************
* Function Name  : deamon_gsensor_check
* Description    : gsensor stat check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_gsensor_check(void) // 10 ms
{
	static INT8U count=0;
	int temp,ret;
	count++;
	if(count<50)
		return ;
	count = 0;

	if((keyTime+3000)>XOSTimeGet())
		return ;
	if((taskCurrent() != TASK_VIDEO_RECORD)/* || menuActiveGet()*/)
		return ;
	if((SysCtrl.bfd_gsensor>0) && configValue2Int(CONFIG_ID_GSENSOR))
    {
    	if(videoRecordGetStatus()==MEDIA_STAT_START)	// recording will check Gsensor
    	{
			ret = boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_LOCK,(INT32U)&temp);
			//deg_Printf("deamon gsensor: ret=%d,temp=0x%x\n",ret,temp);
			if((ret>=0) && temp)
			{
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(VID_EVENT_LOCK_FILE,0));
				deg_Printf("deamon : gsensor active.\n");
				//sysLog("deamon : gsensor active.");
			}
    	}
    }
}

/*******************************************************************************
* Function Name  : deamon_clock_check
* Description    : check clock state
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/

static int default_clock_state = 0; 
int set_clock_state(int state) // 1-open, 0-off
{
    default_clock_state = state;
}

int get_clock_state(void) // 1-open, 0-off
{
    return default_clock_state;
}

int deamon_clock_check(int ClearClockTime)
{
	static INT32U LastTime=0;
	uint32 cfg_save = 60;
	static INT32U flag = 0;
	INT8U task;

	if(ClearClockTime)
		goto RESET_CLOCK_TIME;

	task = taskCurrent();

	if(taskCurrent() == TASK_POWER_OFF)
		return 0;


	  if(windowIsOpen(&selfclockWindow) == 1 )
	    {
			LastTime=XOSTimeGet();
	        return 0;
	    }

#if 0
	if((task == TASK_VIDEO_RECORD && videoRecordGetStatus() == MEDIA_STAT_START) || \
		(task == TASK_AUDIO_RECORD && audioRecordGetStatus() ==  MEDIA_STAT_START)||\
		(task == TASK_PLAY_BACK && videoPlaybackGetStatus() == MEDIA_STAT_PLAY)||\
		motion || (task == TASK_USB_DEVICE)||(task == TASK_POWER_ON))
#else
    if((task == TASK_USB_DEVICE)||(task == TASK_POWER_ON) || \
		(task == TASK_PLAY_BACK && videoPlaybackGetStatus() == MEDIA_STAT_PLAY))
#endif
	{
		LastTime=XOSTimeGet();
		return 0;
	}

    if(flag == 0)
    {
		LastTime=XOSTimeGet();
        flag = 1;
    }

	
#if 0
    //deg_Printf("wait time= %d\r\n", XOSTimeGet()-LastTime);
 	if((XOSTimeGet()-LastTime)>cfg_save*1000)
	{
        //CHKPNT;
		LastTime=XOSTimeGet();
        set_clock_state(1);
        if(windowIsOpen(&selfclockWindow) == 0)
        {
            uiOpenWindow(&selfclockWindow,0);
        }
	}
#endif


    return 0;

RESET_CLOCK_TIME:
    LastTime=XOSTimeGet();

	return 0;
}



/*******************************************************************************
* Function Name  : deamon_screen_save
* Description    : screen save check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_screen_save(int on)
{
	static INT32U LastTime=0;
	uint32 cfg_save;
	uint32 lcdOn=0;
	if(taskCurrent() == TASK_POWER_OFF)
		return 0;

    // if clock no open, return,
    //if(get_clock_state() == 0)
    //if((get_clock_state() == 0) || ((taskCurrent() == TASK_PLAY_BACK) && (videoPlaybackGetStatus() == MEDIA_STAT_PLAY)))
    if((taskCurrent() == TASK_PLAY_BACK) && (videoPlaybackGetStatus() == MEDIA_STAT_PLAY))
    {
        return 0;
    }

	cfg_save = configValue2Int(CONFIG_ID_SCREENSAVE);
   	if(SysCtrl.astern == ASTERN_ON||on)
	{
		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn==0)
	        boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);   // screen on			
		LastTime=XOSTimeGet();
		return 1;
	}
	if(cfg_save==0)
		return 0;

#if 0
	  if(windowIsOpen(&selfclockWindow) == 0)
	  	{
	  		LastTime=XOSTimeGet();
			return 0;
	  	}
#endif
	
	if((XOSTimeGet()-LastTime)>cfg_save*1000)
	{
		LastTime=XOSTimeGet();
		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn)
			boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0);   // screen off
	}
	return 0;
}
/*******************************************************************************
* Function Name  : deamon_auto_poweroff
* Description    : system auto power off check
* Input          : int event : system event
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_auto_poweroff(int ClearPowerOffTime)
{
#define TIME_INTERNAL        500
	INT8U task;
	INT32U AutoPowerOffTime;
	static INT32U LastTime=0,PowerOffTime=0;
	if(ClearPowerOffTime)
		goto RESET_POWER_OFF_TIME;
	if((INT32U)(XOSTimeGet()-LastTime)<TIME_INTERNAL)
		return 0;
	LastTime=XOSTimeGet();

	task = taskCurrent();
	if(task == TASK_POWER_OFF)
		return 0;

	AutoPowerOffTime=configValue2Int(CONFIG_ID_AUTOOFF)*1000;
	if(AutoPowerOffTime==0)
		goto RESET_POWER_OFF_TIME;

	 if(SysCtrl.usb != NULL)
	 	goto RESET_POWER_OFF_TIME;

	if((task == TASK_VIDEO_RECORD && videoRecordGetStatus() == MEDIA_STAT_START) || \
		(task == TASK_AUDIO_RECORD && audioRecordGetStatus() ==  MEDIA_STAT_START)||\
		(task == TASK_PLAY_BACK && videoPlaybackGetStatus() == MEDIA_STAT_PLAY)||\
		/*motion ||*/ (task == TASK_USB_DEVICE)||(task == TASK_POWER_ON)/*||(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON) */|| hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN) )
	{
		goto RESET_POWER_OFF_TIME;
	}
		
	PowerOffTime+=TIME_INTERNAL;
	if(PowerOffTime>=AutoPowerOffTime)
	{
		deg_Printf("Auto power off\n");
		taskStart(TASK_POWER_OFF,0);
	}
	return 0;
RESET_POWER_OFF_TIME:
	PowerOffTime=0;
	return 0;
}



/*******************************************************************************
* Function Name  : deamon_keysound_check
* Description    : deamon key sound condition check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_keysound_play(void)
{
	//if(SysCtrl.f_keysound==0) // key sound off
	{
		return 0;
	}
	if(audioPlaybackGetStatus()==MEDIA_STAT_PLAY) // video/audio playing
	{
		return 0;
	}
    if(taskCurrent() == TASK_POWER_OFF)
		return 0;
	
	Audio_ARG_T music;

//	music.size = 2160;
	music.media.src.buff = (INT32U)layout_keysound_load(&music.size);//buildwin_res_key;
	if(music.media.src.buff==0)
	{
		deg_Printf("load music fail\n");
		return -1;
	}
	music.channel =1 ;
	music.type = FILE_TYPE_RAW;
	music.samplerate = 16000;
	music.media.type = MEDIA_SRC_RAM;

	music.media.src.buff += 44;	// cut wav head data
	music.size -= 44;

    audioPlaybackSetVolume(KEYSOUND_VOLUME);
//	audioPlaybackSetVolume(100);
	audioPlaybackStart(&music);

	return 0;		
}
/*******************************************************************************
* Function Name  : deamon_fsFreeSize
* Description    : deamon get fs free size
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_fsFreeSize(void)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		SysCtrl.sd_freesize = fs_free_size()>>1;  // kb
		/*if(fs_check()<0)
		{
			SysCtrl.sd_freesize = 0;
			deg_Printf("deamon : fs system too more debris\n");
		}*/
		if(SysCtrl.sd_freesize == 0)
		{
			SysCtrl.sdcard = SDC_STAT_FULL; 
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
			deg_Printf("deamon : sdc normal but free space is 0.set sdc error\n");  // user need format sdc
		}
	}
	else
		SysCtrl.sd_freesize = 0;
	//deg_Printf("deamon : fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
	return SysCtrl.sd_freesize;
}
/*******************************************************************************
* Function Name  : deamon_fsSizeAdd
* Description    : deamon dec size from free size
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_fsSizeModify(INT8S dec,INT32U size)
{
	if(size&(SysCtrl.fs_clustsize-1))
	{
		size = (size&(~(SysCtrl.fs_clustsize-1)))+SysCtrl.fs_clustsize;
		//size+=1024;
	}
	size>>=10;
//    deg_Printf("deamon 0: fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
	if(dec<0)
	{
		if(SysCtrl.sd_freesize>size)
			SysCtrl.sd_freesize-=size;
		else
			SysCtrl.sd_freesize = 0;
	}
	else
	{
		SysCtrl.sd_freesize+=size;
	}
//	deg_Printf("deamon 1: fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
	return SysCtrl.sd_freesize;
}

int get_key_clock_value(void)
{
	uint32 key_clock_value = 0; 

    boardIoctrl(SysCtrl.bfd_lcd,IOGET_KEY_CLOCK,(u32)&key_clock_value);
    //deg_Printf("key_clock_value=%d\r\n", key_clock_value);

    return key_clock_value;
}

int get_suspended_lcd_value(void)
{
	uint32 lcdOn=0; 
    int suspend_lcd_value = 0;

    deamon_auto_poweroff(1);
    boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
    if(lcdOn&&SysCtrl.astern == ASTERN_OFF)
    {
        suspend_lcd_value = 0;
    }
    else
    {
        suspend_lcd_value = 1;
        deamon_screen_save(1);
        setKeyEn(true);
    }

	return suspend_lcd_value;
}



#if KEY_PRESSED_POWER_TAP

static int pwr_pressed_tap_status = 0; //if long press, set 0

void set_key_press_power_clock(int status)
{
    pwr_pressed_tap_status = status;
}

int get_key_press_power_clock(void)
{
    return pwr_pressed_tap_status;
}

#endif




