/****************************************************************************
**
 **                              BOARD
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  BOARD BWV1
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : board_bwv1.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is for user board
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
//#include "../device/gsensor/gsensor.h"
//#include "../device/tft_lcd/tft_lcd.h"
//#include "../device/cmos_sensor/cmos_sensor.h"

#if SYSTEM_BOARD == BOARD_TYPE_BWV1

#if AX32_PLATFORM == AX3292

	#define  BOARD_VERSION   0x11  // v12


	#define  LED_N0_CH       GPIO_PA//GPIO_PE
	#define  LED_N0_PIN      GPIO_PIN13 //GPIO_PIN1
	#define  LED_N0_ACTIVE   1

	#define  GSE_INT_CH      GPIO_PE
	#define  GSE_INT_PIN     GPIO_PIN13 //GPIO_PIN0

	#define  PWR_KEY_CH      GPIO_PB
	#define  PWR_KEY_PIN     GPIO_PIN0

    #define  CLOCK_KEY_CH     GPIO_PB //GPIO_PA
    #define  CLOCK_KEY_PIN    GPIO_PIN13 //

	#define  USB_DET_CH      GPIO_PA
	#define  USB_DET_PIN     GPIO_PIN13
#if IR_MENU_EN	
	#define  IR_LED_CH 			   GPIO_PA
	#define  IR_LED_PIN			   GPIO_PIN9
#endif 
	
	#define  USB2_PWR_CH     GPIO_PA
	#define  USB2_PWR_PIN    GPIO_PIN13 //GPIO_PIN7
	#define  USB2_DET_CH     GPIO_PA //GPIO_PE
	#define  USB2_DET_PIN    GPIO_PIN13 //GPIO_PIN0


	#define  ADC_BAT_CH      ADC_CH_MVOUT
	#define  ADC_BGP_CH      ADC_CH_BGOP
	#define  ADC_KEY_CH      ADC_CH_PB1 //FIXME:redefine @hal.h
	//#define  ADC_GSR_CH      ADC_CH_PE0

#else

#if 0
	#define  LED_N0_CH       GPIO_PA
	#define  LED_N0_PIN      GPIO_PIN7
	#define  LED_N0_ACTIVE   1
#endif
	#define  LCD_BK_CH       GPIO_PA
	#define  LCD_BK_PIN      GPIO_PIN9

	#define  GSE_INT_CH      GPIO_PA
	#define  GSE_INT_PIN     GPIO_PIN6

	#define  PWR_KEY_CH      GPIO_PB
	#define  PWR_KEY_PIN     GPIO_PIN0

	#define  USB_DET_CH      GPIO_PA
	#define  USB_DET_PIN     GPIO_PIN13

	#if USENSOR_PWRCTRL_EN
	#define  USB2_PWR_CH     GPIO_PE
	#define  USB2_PWR_PIN    GPIO_PIN1
	#define  USB2_DET_CH     GPIO_PE
	#define  USB2_DET_PIN    GPIO_PIN0
	#else
	#define  USB2_DET_CH     GPIO_PE
	#define  USB2_DET_PIN    GPIO_PIN13 //GPIO_PIN0
	#endif
#if IR_MENU_EN	
	#define  IR_LED_CH 		 GPIO_PB
	#define  IR_LED_PIN		 GPIO_PIN5 
#endif

	#define  ADC_BAT_CH      ADC_CH_MVOUT
	#define  ADC_BGP_CH      ADC_CH_BGOP
	#define  ADC_KEY_CH      ADC_CH_PB1 //FIXME:redefine @hal.h
	#define  ADC_GSR_CH      ADC_CH_PA6
#endif

static u8 adCh;

static int board_led_init(void)
{

	return 0;
}

static int board_led_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	static u8 led0=0;
	
    if(op == IOCTRL_LED_CHECK)
	{
		op = IOCTRL_LED_NO0;
		para = led0;
	}
	
    if(op == IOCTRL_LED_NO0)
    {
		if((hal_uartIOShare()==0)||(hal_iic1IOShare()==0))
		{
			hal_gpioInit(LED_N0_CH,LED_N0_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
		//	hal_gpioEPullSet(GPIO_PA,GPIO_PIN7,GPIO_PULLE_UP);
		}
		led0 = para;
	#if LED_N0_ACTIVE == 0
	     para = (para^1)&1;
	#endif

		if(para)
			hal_gpioWrite(LED_N0_CH,LED_N0_PIN,GPIO_HIGH);
		else
			hal_gpioWrite(LED_N0_CH,LED_N0_PIN,GPIO_LOW);
    }
	
	return 0;
}

static int board_lcd_init(void)
{
	//lcd_init();

	hal_gpioInit(LCD_BK_CH,LCD_BK_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
	hal_gpioEPullSet(LCD_BK_CH,LCD_BK_PIN,GPIO_PULLE_DOWN);
	hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_LOW);

//--------------LCD RESET------------------------------------
//	hal_dacHPSet(1,HP_VDD_2_8V);
//	hal_sysDelayMS(50);
//	
//	hal_dacHPSet(0,0);
//	hal_sysDelayMS(100);
//	
//	hal_dacHPSet(1,HP_VDD_2_8V);
//	hal_sysDelayMS(50);

	lcdInit();

	return 0;
}
static int board_cmos_init(void)
{
	
	sensorInit();
	hal_mjpegEncodeInit();
	ax32xx_csi_only_FrameSet();
	hal_csiEnable(1);
	return 0;
}
static int board_cmos_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_CMOS_INIT)
	{
		return board_cmos_init();
	}
	return -1;
}
extern void setKeyEn(bool en);
static int board_lcd_ioctrl(INT32U prit,INT32U op,INT32U para)
{
     static INT32U  lcdState=1;
    if(op == IOCTRL_LCD_BKLIGHT)
    {
		if(para)
		{
			hal_gpioEPullSet(LCD_BK_CH,LCD_BK_PIN,GPIO_PULLE_UP);
			hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_HIGH);
			lcdState=1;
		}
		else
		{
			hal_gpioEPullSet(LCD_BK_CH,LCD_BK_PIN,GPIO_PULLE_DOWN);
			hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_LOW);
			lcdState=0;
			setKeyEn(false);
		}
    }
	else if(op == IOGET_LCD_BKLIGHT){
   		*(INT32U*)para=lcdState;
	}
    else if(op == IOCTRL_LCD_LCMOFF)
    {
        hal_lcdLCMPowerOff();
    }

	return 0;
}

static int board_sdc_init(void)
{
//#if USENSOR_PWRCTRL_EN
    hal_sysLDOSet(SYS_LDO_SDC,0,0);  // disable SD_VDD
//#else
//    hal_sysLDOSet(SYS_LDO_SDC,0,1);  // enable SD_VDD
//#endif
    return 0;
}

static int board_sdc_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_SDC_CHECK)
	{
		if(hal_sdCheck())
			*(INT32U *)para = 1;
		else
			*(INT32U *)para = 0;

		return 0;
	}

	return -1;
}
static int board_usb_init(void)
{
    hal_wki1InputEnable(1);

	hal_gpioInit(DC_BAT_CHARGE_CH,DC_BAT_CHARGE_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	//hal_gpioEPullSet(DC_BAT_CHARGE_CH,DC_BAT_CHARGE_PIN,GPIO_PULL_FLOATING);
	
    return 0;
}

static int board_usb_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_USB_CHECK)
	{ 
#if 1
       *(INT32U *)para = hal_wki1Read();
#else
		hal_spiIOShare();		
		//hal_gpioInit(USB_DET_CH,USB_DET_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);  // spi clk using this io
		
		*(INT32U *)para = 1;//hal_gpioRead(USB_DET_CH,USB_DET_PIN);
#endif		
		return 0;
	}

	return -1;
}
static int board_usb2_init(void)
{
    return -1;
#if (AX32_PLATFORM == AX3292)
	
	hal_gpioInit_io1d1(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);  //配置为高压IO
	#if IO1DO1_USE_SOFTSTART
	if((USB2_PWR_CH == GPIO_PA)&&(USB2_PWR_PIN == GPIO_PIN7))
			 hal_io1d1_softstart(IO1_CH_PA7,1,IO1_SOFTTIME_14MS,0);
	else if((USB2_PWR_CH == GPIO_PD)&&(USB2_PWR_PIN == GPIO_PIN1))
			 hal_io1d1_softstart(IO1_CH_PD1,1,IO1_SOFTTIME_14MS,0);
	#endif
	// hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_DOWN);  //B_5V enable
#else
	#if USENSOR_PWRCTRL_EN
		//detect-pin config as output for ir_led
		hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		hal_gpioWrite(USB2_DET_CH,USB2_DET_PIN,GPIO_HIGH);
		
		hal_gpioInit(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);
	#else
		hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
		hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_DOWN);
	#endif

#endif
	return 0;
}
static int board_usb2_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	static INT32U lastUSBPower=0xffffffff;
    return -1;

#if (AX32_PLATFORM != AX3292)
    UNUSED u8 output_stat = hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);// store detect-pin output status
#endif
	if(op == IOCTRL_USB_CHECK)
	{
#if (AX32_PLATFORM == AX3292)
	
	*(INT32U *)para = 1;  // use soft detect
#else
	#if USENSOR_PWRCTRL_EN
        if(hal_gpioRead(USB2_PWR_CH,USB2_PWR_PIN))
        {
            //debg("set usb2 check : pull-down\n");
            hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_DOWN);
            *(INT32U *)para = !hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);
        }
        else
        {
            //debg("set usb2 check : pull-up\n");
            hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_UP);
            *(INT32U *)para = hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);
        }
        
        hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
        hal_gpioWrite(USB2_DET_CH,USB2_DET_PIN,output_stat);
	#else
        *(INT32U *)para = !hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);
	#endif
#endif
		return 0;
	}
    else if(op == IOCTRL_USB_POWER)
    {
		if(para==lastUSBPower)
			return -1;
		  // debg("[usb2]set power : %x\n",para);
#if (AX32_PLATFORM == AX3292)
	
     
	 if(para)
	 {
	#if IO1DO1_USE_SOFTSTART
		hal_io1d1_softstart_clr(IO1_CH_PA7,0);
	#else
		 hal_gpioInit(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_DOWN);	
		 hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);
	#endif
		 delay_5ms(10);
	 }
	 else
	 {
	#if IO1DO1_USE_SOFTSTART
		hal_io1d1_softstart_clr(IO1_CH_PA7,1);
	#else
		hal_gpioInit_io1d1(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);  //配置为高压IO
	#endif
		delay_5ms(10);
	 }
		lastUSBPower=para;
#else

	#if USENSOR_PWRCTRL_EN
        if(para)
        {
            hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_HIGH);
			delay_5ms(1000);
        }
        else
        {
            hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);
			delay_5ms(1000);
        }
		lastUSBPower=para;
	#endif
#endif
        return 0;
    }

	return -1;
}

//------------battery --------
#define  BATTERY_MAX     5
#define  BATTERY_AVG     3
#define  BATTERY_INV     12  //6
#define  BATTERY_OFS     (30) //电池到IC引脚之间的压差,unit:mV
const static u16 batteryValueTable[BATTERY_MAX] = {3300,3540,3630,3850, 4000};

static int board_battery_init(void)
{
	u16 batValue=0,bgpValue=0;
	int value;

	hal_batDetectEnable(1);

    int i = 3;
    while(i--)
    {
        batValue += hal_adcGetChannel(ADC_BAT_CH);
        bgpValue += hal_adcGetChannel(ADC_BGP_CH);
    }

	if(bgpValue==0)
		value = 0;
	else
		value = hal_adcVDDRTCCalculate(batValue,bgpValue) + BATTERY_OFS;

	if(value</*batteryValueTable[1]*/3480)
	{
		board_usb_ioctrl(0,IOCTRL_USB_CHECK,(INT32U)&value);
		if(value==0)
		{
			deg_Printf("board : battary low power.try to power off.\n");
			hal_vddWKOEnable(0);
			
			ax32xx_wkiCleanPending();
			XOSTimeDly(200);
			uninit();
		    return -1;
		}
	}
	
	return 0;
}
static int curBatteryADC=0;
int getCurBatteryADC(void)
{
	return curBatteryADC;
}
static int board_battery_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	static u8 step = 0,oldBat=0xff;
	static u16 batValue=0,bgpValue=0;
	int i,value;
	if(op == IOCTRL_BAT_CHECK)
	{
	//	if((step&1) == 0)
		{
			batValue += hal_adcGetChannel(ADC_BAT_CH);
			bgpValue += hal_adcGetChannel(ADC_BGP_CH);
			step++;
		}
		/*else if(step&1)
		{            
			step++;
		}*/
		
		if(step>=BATTERY_AVG)
		{
			if(bgpValue==0)
				value = 0;
            else
                value = hal_adcVDDRTCCalculate(batValue,bgpValue) + BATTERY_OFS;

			for(i=0;i<BATTERY_MAX;i++)
			{
				if(value<=batteryValueTable[i])
					break;
			}
			if(oldBat!=i)
			{
				if(oldBat==0xff)
					oldBat = i;
				else
				{
					if(oldBat>i)
					{
						if(value<batteryValueTable[i]-BATTERY_INV)
							oldBat=i;
					}
					else
					{
						if(value>batteryValueTable[oldBat]+BATTERY_INV)
							oldBat=i;
					}
				}
			}
			curBatteryADC=value;
			//deg_Printf("battery : ad value = %d,i = %d,oldBat=%d\n",value,i,oldBat);
			step = 0;
			bgpValue=0;
			batValue=0;
			*(INT32U *)para = oldBat;
		#if 0
			#define LIMIT_BATVAL	3480 //must < batteryValueTable[0]	
			if((LIMIT_BATVAL < batteryValueTable[1])&&(value < LIMIT_BATVAL))
			{
				*(INT32U *)para	= -1;
			}
		#endif
			return 0;
		}
	}

	return -1;
}

//----------------gsensor---------------------
static int board_gsensor_init(void)
{
	return 0;

	hal_vddGSENEnable(1);
	return gSensorInit();
}

static int board_gsensor_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	INT32S value=0;

	return value;

	switch(op)
	{
		case IOCTRL_GSE_SETLOCK : 
			   return gSensorActiveSet(para);
			   break;
		case IOCTRL_GSE_SETPARK:		   	
			   return gSensorWakeupSet(para);
			   break;
		case IOCTRL_GSE_LOCK:
			   value = gSensorActiveGet();
			   *(INT32U *)para = value;
			   break;
		case IOCTRL_GSE_PARK:
			   value = gSensorWakeUpGet();
			    *(INT32U *)para = value;
			   break;
		case IOCTRL_GSE_STABLE:
			   gSensorMircoMotionWait(para);
			   break;
	    case IOCTRL_GSE_GETNAME:
			   strcpy((char *)para,gSensorGetName());
			   break;
	}

	return value;
}

//----------------ad key-----------------------
#define  _POWR_OFF_LGC_		1
#define  KEY_NUM			   6

/*
if you want to implement the long press function of the key,
please refer to the getKey() function, when the key long pressed,
change the value of key
*/
typedef struct _keyInfor
{
	u8   keyType;      // key event
	u16  keyADCvalue;  // adc value of key
	u16  keyADCmin;    // no need initialzation,in board_adkey_init() function,this value calculated according to (keyADCvalue-ADC_KEY_DEVIATION)
	u16  keyADCmax;    // no need initialzation,in board_adkey_init() function,this value calculated according to (keyADCvalue+ADC_KEY_DEVIATION)
}keyInfor;
#define ADC_KEY_DEVIATION     40  // if ADC value of key has a large error,ADC_KEY_DEVIATION shud be large 
#define ADD_KEY(key,keyADCvalue)  {key,keyADCvalue,0,0}
#if (AX32_PLATFORM == AX3291A)  //AX3291 FP80 PIN
keyInfor adckeyInfor[]=
{
	ADD_KEY(KEY_EVENT_OK,  0x7c),
	ADD_KEY(KEY_EVENT_DOWN,0x174),
	ADD_KEY(KEY_EVENT_UP,  0x26c),
	ADD_KEY(KEY_EVENT_MENU,0x364),
	ADD_KEY(KEY_EVENT_MODE,0x3F0),
};
#elif ((AX32_PLATFORM == AX3282S)||(AX32_PLATFORM == AX3292)) //same to ax3282
#if KEY_NUM == 6
keyInfor adckeyInfor[]=
{
	ADD_KEY(KEY_EVENT_MENU,660),
	//ADD_KEY(KEY_EVENT_PHOTO,760),
	ADD_KEY(KEY_EVENT_UP,845),
	ADD_KEY(KEY_EVENT_DOWN,901),
};
#elif KEY_NUM == 5
keyInfor adckeyInfor[]=
{
	ADD_KEY(KEY_EVENT_OK,  307),
	ADD_KEY(KEY_EVENT_DOWN,465),
	ADD_KEY(KEY_EVENT_UP,  633),
	ADD_KEY(KEY_EVENT_MODE,822),
};
#endif
#endif
static int board_adkey_init(void)
{
	u32 i;
	for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
	{
		if(adckeyInfor[i].keyADCvalue<=ADC_KEY_DEVIATION)
			adckeyInfor[i].keyADCmin=0;
		else
			adckeyInfor[i].keyADCmin=adckeyInfor[i].keyADCvalue-ADC_KEY_DEVIATION;
		adckeyInfor[i].keyADCmax=adckeyInfor[i].keyADCvalue+ADC_KEY_DEVIATION;
		if(adckeyInfor[i].keyADCmax>0x3f0)
			adckeyInfor[i].keyADCmax=0x3f0;
	}
	adCh = ADC_KEY_CH;
    hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
#if (AX32_PLATFORM == AX3291A) //adkey use io inner 10k resistance
	hal_gpioInit(GPIO_PB,GPIO_PIN1,GPIO_INPUT,GPIO_PULL_UP); //ADKEY0 PULL UP
#endif
	
	//hal_gpioInit(SW_SAVE_KEY_CH, SW_SAVE_KEY_PIN,    GPIO_INPUT,  GPIO_PULLE_DOWN );
	hal_gpioInit(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN, GPIO_INPUT, GPIO_PULLE_DOWN );
	hal_gpioInit(SW_MIC_KEY_CH, SW_MIC_KEY_PIN,      GPIO_INPUT,  GPIO_PULLE_DOWN );

	hal_gpioInit(AUDIO_EN_CH, AUDIO_EN_PIN,      GPIO_OUTPUT,  GPIO_PULLE_UP );
	hal_gpioWrite(AUDIO_EN_CH,AUDIO_EN_PIN,GPIO_LOW);
	
	return 0;
}
uint32 keyADCvalue=0;
uint32 getKeyADCvalue(void)
{
	return keyADCvalue;
}

#define KEY_SCAN_UNFINISHED             0xffffffff
uint32 getKeyVal(void)
{
	static uint32 lastKey=KEY_EVENT_END;
	static uint32 keyScanTimes=0;
	uint32 value,i,key=0;

	uint32 value_save, value_vr, value_mic;

#if (_POWR_OFF_LGC_ == 1)
	value = hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
#else
	value = !hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
#endif
	   if(value)
	   	{
	   	   key = KEY_EVENT_POWER;   // power key ,key value
	   	}
	   else
	   {
		  value = hal_adcGetChannel(ADC_KEY_CH);
		  if(value<=1000)
		  {
		  	keyADCvalue=value;
		    //deg_Printf("adcKeyValue:%d\n",value);
		  }
		   for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
		   {
		   	   if(value>=adckeyInfor[i].keyADCmin&&value<=adckeyInfor[i].keyADCmax)
			   	  key = adckeyInfor[i].keyType;
		   }
	   }
	   if(lastKey!=key)
   	   {
   	  	lastKey=key;
		keyScanTimes=1;
   	   }
	   else
	   	keyScanTimes++;
	   if(keyScanTimes>=3)
   	  {
   	  	keyScanTimes=0;
		return key;
   	  }
	   return KEY_SCAN_UNFINISHED;
}

u8 key_clock_flag = 0;
static uint32 suspended_lcd_time=0;
uint32 getKey(void)
{
	static uint32 keyPowerPressCnt=0;
	uint32 keyVal;
	keyVal=getKeyVal();

	
   if((keyVal != KEY_SCAN_UNFINISHED)&&(keyVal != 0))
   	{
		if(get_suspended_lcd_value())
	    {
	    	suspended_lcd_time=XOSTimeGet();
	        return KEY_SCAN_UNFINISHED;
	    }
   	}

	 if((XOSTimeGet()- suspended_lcd_time )< 500)
 	{
		return KEY_SCAN_UNFINISHED;
 	}

	
	if(keyVal==KEY_SCAN_UNFINISHED)
		return KEY_SCAN_UNFINISHED;
	if(keyVal==KEY_EVENT_POWER)
	{
			
        if( !(hal_gpioRead(SW_MIC_KEY_CH, SW_MIC_KEY_PIN) ) && !(hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN) ) )  
        {
            //CHKPNT;
		    keyPowerPressCnt++;
        }
        else
        {
            //CHKPNT;
		    keyPowerPressCnt = 0;
        }

		if(get_key_press_power_clock())
		{
			if(keyPowerPressCnt>=1)
				keyVal=KEY_EVENT_POWEROFF;
			else
				keyVal=0;	
		}
		else
		{
			if(keyPowerPressCnt>=30)
				keyVal=KEY_EVENT_POWEROFF;
			else
				keyVal=0;	
		}
        
		
	}
	else
	{
		if(keyPowerPressCnt>0&&keyPowerPressCnt<30)
			keyVal = KEY_EVENT_POWER;
		keyPowerPressCnt=0;
	}
#if KEY_NUM == 5
	static uint32 keyModePressCnt=0;
	if(keyVal==KEY_EVENT_MODE)
	{
		keyModePressCnt++;
		if(keyModePressCnt>=10)
			keyVal=KEY_EVENT_MENU;
		else
			keyVal=0;	
	}
	else
	{
		if(keyModePressCnt>0&&keyModePressCnt<10)
			keyVal = KEY_EVENT_MODE;
		keyModePressCnt=0;
	}
#endif

	static uint32 keyMenuPressCnt=0;
	if(keyVal==KEY_EVENT_MENU)
	{
		keyMenuPressCnt++;
		if(keyMenuPressCnt>=10)
			keyVal= KEY_EVENT_MENULONG;
		else
			keyVal=0;	
	}
	else
	{
		if(keyMenuPressCnt>0&&keyMenuPressCnt<10)
			keyVal = KEY_EVENT_MENU;
		keyMenuPressCnt=0;
	}

	static uint32 keyDownPressCnt=0;
	if(keyVal==KEY_EVENT_DOWN)
	{
		keyDownPressCnt++;
		if(keyDownPressCnt>=10)
		{
			keyVal= KEY_EVENT_DOWNLONG;
		}
		else
			keyVal=0;	
	}
	else
	{
		if(keyDownPressCnt>0&&keyDownPressCnt<10)
			keyVal = KEY_EVENT_DOWN;
		keyDownPressCnt=0;
	}

	static uint32 keyUpPressCnt=0;
	if(keyVal==KEY_EVENT_UP)
	{
		keyUpPressCnt++;
		if(keyUpPressCnt>=10)
		{
			keyVal= KEY_EVENT_UPLONG;
		}
		else
			keyVal=0;	
	}
	else
	{
		if(keyUpPressCnt>0&&keyUpPressCnt<10)
			keyVal = KEY_EVENT_UP;
		keyUpPressCnt=0;
	}

	return keyVal;
}
#define SCAN_KEY_CONTINUE_INTERVAL  (3)
uint32 keyScanKey(void)
{
	static uint32 LastKeyVal=0;
	static int32 KeyContinueCnt=0;
	uint32 CurKeyVal;
	uint32 KeySta=0;
	CurKeyVal=getKey();
	if(CurKeyVal==KEY_SCAN_UNFINISHED)
		return 0;
	if(LastKeyVal!=CurKeyVal)
	{
		if(CurKeyVal)
			KeySta=makeEvent(CurKeyVal,KEY_PRESSED);
		else
			KeySta=makeEvent(LastKeyVal,KEY_RELEASE);
		LastKeyVal=CurKeyVal;
		KeyContinueCnt=-10;
	}
	else
	{
		if(CurKeyVal)
		{
			KeyContinueCnt++;
			if(KeyContinueCnt>=SCAN_KEY_CONTINUE_INTERVAL)
			{
				KeyContinueCnt=0;
				KeySta=makeEvent(CurKeyVal,KEY_CONTINUE);
			}
			else
				KeySta=0;
		}
		else
			KeySta=0;
	}
	return KeySta;
}

#define T_KEY_SHORT             0x00
#define T_KEY_SHORT_UP         0x20
#define T_KEY_LONG              0x40
#define T_KEY_LONG_UP          0x60
#define T_KEY_DOWN              0x80

#define KEY_LONG_TIME    80 //150   // 2-second
#define KEY_LONG_TICK    20 //50    // every 0.5second tick once
static int board_key_process(INT32U keyValue)
{
	static int oldKey=0,oldTime=0;
	INT8U key;

    key = 0;
	if(keyValue == 0)
	{
		if(oldKey)
		{
			key = oldKey;
			if(oldTime>=(KEY_LONG_TIME-KEY_LONG_TICK))
				key|=T_KEY_LONG_UP;
			else if(oldTime>1)
				key|=T_KEY_SHORT_UP;
			else 
				key|=T_KEY_SHORT;
			oldKey = 0;
			oldTime = 0;
		}
	}
	else
	{
		if(oldKey == keyValue)
		{
			oldTime++;
			if(oldTime>=KEY_LONG_TIME)
			{
				oldTime-=KEY_LONG_TICK;
				key = oldKey|T_KEY_LONG;
			}
			else if(oldTime==1)
			{
				key = oldKey|T_KEY_DOWN;
			}
		}
		else
		{
			oldKey = keyValue;
			oldTime = 0;
		}
	}

	return key;
}

uint8 g_key_cnt=0;
static int board_adkey_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	int value;
    static int counts = 0;
	int switch_value, i;

	if(op == IOCTRL_KEY_READ)
	{
	   if((*(INT32U *)para = keyScanKey())==0)
	   	   return -1;			   	   
	}
    else if(op == IOCTRL_KEY_POWER)
    {
		#if (_POWR_OFF_LGC_ == 1)
		value = hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
		#else
		value = !hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
		#endif
		*(int *)para = value; 
    }
	else if(op == IOCTRL_KEY_POWEROFF)
	{
		hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_OUTPUT,GPIO_PULL_DOWN);
		hal_gpioWrite(PWR_KEY_CH,PWR_KEY_PIN,GPIO_LOW);
	}
    else if(op == IOCTL_KEY_SWITCH)
    {
#if 0
        deg_Printf("rec=%d, mic=%d,save=%d\r\n", 
            hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN),
            hal_gpioRead(SW_MIC_KEY_CH, SW_MIC_KEY_PIN),
            hal_gpioRead(SW_SAVE_KEY_CH, SW_SAVE_KEY_PIN));
#endif

	#if 1
        if(hal_gpioRead(SW_RECORD_KEY_CH, SW_RECORD_KEY_PIN))
         {
             switch_value = KEY_EVENT_RECORD;
         }
         else if(hal_gpioRead(SW_MIC_KEY_CH, SW_MIC_KEY_PIN))
         {
             switch_value = KEY_EVENT_AUDIO;
         }
         else 
         {
             switch_value = KEY_EVENT_SAVE;
         }
       
	#endif

		i = board_key_process(switch_value);	

		*(int *)para = i; 
		if(!i)
	   	   return -1;

    }

	return 0;
}

int board_powerCheck(void)
{
	hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_INPUT,GPIO_PULL_DOWN);

	return hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
}


static int board_ir_init(void)
{
#if (1 == IR_MENU_EN)
	ax32xx_gpioDigitalSet(IR_LED_CH,IR_LED_PIN,GPIO_DIGITAL_EN);		// set ir io is digital .
#endif
	return 0;
}

static int board_ir_ioctrl(INT32U prit,INT32U op,INT32U para)
{
#if (1 == IR_MENU_EN)
	static uint32 irState=0;
	if(op == IOCTRL_IR_SET)
	{
	   if(para)	// on
	   {
		   hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
		   hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_HIGH);
		   irState=1;
	   }
	   else		//off
	   {
		   hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_DOWN);
		   hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_LOW);
		   irState=0;
	   }
	}
	else if(op == IOGET_IR_GET)
		*(INT32U*)para=irState;
#endif
	return 0;
}



Board_Node_T  board_bwv1[]=
{
	{//------bat-----------
		.name = DEV_NAME_BATTERY,
		.init = board_battery_init,
		.ioctrl = board_battery_ioctrl, 
		.prit = 0
	},
	{
		.name = DEV_NAME_USENSOR,
		.init = board_usb2_init,
		.ioctrl = board_usb2_ioctrl,
		.prit = 0
	},
	{//------sdc-----------
		.name = DEV_NAME_SDCARD,
		.init = board_sdc_init,
		.ioctrl = board_sdc_ioctrl, 
		.prit = 0
	},
    {//------lcd-----------
		.name = DEV_NAME_LCD,
		.init = board_lcd_init,
		.ioctrl = board_lcd_ioctrl,	
		.prit = 0
    }, 	
    {//------gsensor-----------
		.name = DEV_NAME_GSENSOR,
		.init = board_gsensor_init,
		.ioctrl = board_gsensor_ioctrl, 
		.prit = 0
	},

    {//------usb-----------
		.name = DEV_NAME_USB,
		.init = board_usb_init,
		.ioctrl = board_usb_ioctrl, 
		.prit = 0
	},
	{//------led-----------
		.name = DEV_NAME_LED,
		.init = board_led_init,
		.ioctrl = board_led_ioctrl,	
		.prit = 0
    },
     
	{//------cmos -sensor
		.name = "cmos-sensor",
		.init = NULL,//booard_cmos_init,
		.ioctrl = board_cmos_ioctrl,
		.prit=0
	},
    {//------ad key-----------
		.name = DEV_NAME_ADKEY,
		.init = board_adkey_init,
		.ioctrl = board_adkey_ioctrl, 
		.prit = 0
	},
#if (1 == IR_MENU_EN)
	{//------ir led-----------
		.name = DEV_NAME_IR,
		.init = board_ir_init,
		.ioctrl = board_ir_ioctrl, 
		.prit = 0
	},
#endif
    {
		.name[0] = 0,
		.init = NULL,
		.ioctrl = NULL,
    }
};



#endif

