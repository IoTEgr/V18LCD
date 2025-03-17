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



#if ((SYSTEM_BOARD == BOARD_TYPE_FPGA))



//#define  LED_N0_CH       GPIO_PA
//#define  LED_N0_PIN      GPIO_PIN7
//#define  LED_N0_ACTIVE   1

//#define  LCD_BK_CH       GPIO_PA
//#define  LCD_BK_PIN      GPIO_PIN8

//#define  GSE_INT_CH      GPIO_PA
//#define  GSE_INT_PIN     GPIO_PIN6

//#define  PWR_KEY_CH      GPIO_PB
//#define  PWR_KEY_PIN     GPIO_PIN1

//#define  USB_DET_CH      GPIO_PA
//#define  USB_DET_PIN     GPIO_PIN13

//#define  ADC_BAT_CH      ADC_CH_MVOUT
//#define  ADC_BGP_CH      ADC_CH_BGOP
#define  ADC_KEY_CH      ADC_CH_VDDSD
//#define  ADC_GSR_CH      ADC_CH_PA6

static int board_led_init(void)
{

	return 0;
}

static int board_led_ioctrl(INT32U prit,INT32U op,INT32U para)
{
    if(op == IOCTRL_LED_CHECK)
	{

	}
	
    if(op == IOCTRL_LED_NO0)
    {

    }
	
	return 0;
}

static int board_lcd_init(void)
{
	//lcd_init();
//--------------LCD backlight--------------------------------

//--------------LCD RESET------------------------------------
	ax32xx_gpioMapSet(GPIO_PB,GPIO_PIN4,GPIO_FUNC_GPIO);
	ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN4,GPIO_OUTPUT);
    ax32xx_gpioDataSet(GPIO_PB,GPIO_PIN4,GPIO_LOW);
	ax32xx_sysCpuMsDelay(50);
	ax32xx_gpioDataSet(GPIO_PB,GPIO_PIN4,GPIO_HIGH);
	ax32xx_sysCpuMsDelay(10);

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

static int board_lcd_ioctrl(INT32U prit,INT32U op,INT32U para)
{
    if(op == IOCTRL_LCD_BKLIGHT)
    {
		//*(INT32U*)para= 1 ;
    }
    else if(op == IOCTRL_LCD_LCMOFF)
    {
        hal_lcdLCMPowerOff();
    }
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

static int board_usb2_init(void)
{
    return 0;
}

static int board_usb2_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_USB_CHECK)
	{ 
       *(INT32U *)para = 1;

		return 0;
	}

	return -1;
}
static int board_usb_init(void)
{
    return 0;
}

static int board_usb_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_USB_CHECK)
	{ 
       *(INT32U *)para = 1;

		return 0;
	}

	return -1;
}
//------------battery --------
#define  BATTERY_MAX     4
#define  BATTERY_AVG     10
#define  BATTERY_INV     20
//const static u16 batteryValueTable[BATTERY_MAX] = {1160,1490,1540,1580};//3
const static u16 batteryValueTable[BATTERY_MAX] = {1621,1700,1720,1930};// 3.6,3.8,4.0,4.2

static int board_battery_init(void)
{
	return 0;
}

static int board_battery_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_BAT_CHECK)
	{
        *(INT32U *)para = 3;
        return 0;
	}

	return -1;
}

//----------------gsensor---------------------
static int board_gsensor_init(void)
{
//	hal_vddGSENEnable(1);
//	return gSensorInit();
    return -1;
}

static int board_gsensor_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	INT32S value=0;
//	switch(op)
//	{
//		case IOCTRL_GSE_SETLOCK : 
//			   return gSensorActiveSet(para);
//			   break;
//		case IOCTRL_GSE_SETPARK:		   	
//			   return gSensorWakeupSet(para);
//			   break;
//		case IOCTRL_GSE_LOCK:
//			   value = gSensorActiveGet();
//			   *(INT32U *)para = value;
//			   break;
//		case IOCTRL_GSE_PARK:
//			   value = gSensorWakeUpGet();
//			    *(INT32U *)para = value;
//			   break;
//		case IOCTRL_GSE_STABLE:
//			   gSensorMircoMotionWait(para);
//			   break;
//	}

	return value;
}

//----------------ad key-----------------------
#define  KEY_MAX    (8+1)
const u16 keyValueTable[KEY_MAX]={0x30,0xA0,0x150,0x1B0,0x200,0x280,0x300,0x350,0xFFFF};
const u8  keyTable[KEY_MAX]={KEY_EVENT_OK,KEY_EVENT_MODE,KEY_EVENT_DOWN,KEY_EVENT_UP,KEY_EVENT_MENU,0,0,KEY_EVENT_POWER,0};

static int board_adkey_init(void)
{
	return 0;
}

/*static int board_key_process(INT32U keyValue)
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
static int oldKey=0,oldEvent=0;
static int board_key_process(INT32U keyValue)
{
	static int oldTime=0,event=0;
	INT8U key;
	int temp;

    key = 0;
	if(keyValue == 0)
	{
		if(oldKey)
		{
			key = oldKey;
			temp = XOSTimeGet()-oldTime;
			if(temp>=(KEY_LONG_TIME-KEY_LONG_TICK))
				key|=T_KEY_LONG_UP;
			else if(temp>KEY_DIFFER)
				key|=T_KEY_SHORT_UP;
			else 
				key|=T_KEY_SHORT;
			event = key&0xf0;
			oldKey = 0;
			oldTime = XOSTimeGet();
		}
	}
	else
	{
		if(oldKey == keyValue)
		{
			temp = XOSTimeGet()-oldTime;
			//oldTime++;
			if(temp>=KEY_LONG_TIME)
			{
				oldTime+=KEY_LONG_TICK;
				if(oldEvent!=T_KEY_LONG)  // only send long event once
				    key = oldKey|T_KEY_LONG;
				oldEvent = T_KEY_LONG;
			}
			else if((temp>=KEY_DIFFER)&&(event==0))
			{
				event = T_KEY_DOWN;				
				key = oldKey|T_KEY_DOWN;
				oldEvent = T_KEY_DOWN;
			}
		}
		else
		{
			event = 0;
			oldKey = keyValue;
		//	key = oldKey|T_KEY_DOWN;  // key down
			oldTime = XOSTimeGet();
			oldEvent = T_KEY_DOWN;
		}
	}

	return key;
}*/
uint32 getKey(INT32U keyValue)
{
	static uint32 keyPowerPressCnt=0;
	//static uint32 keyPowerReleaseCnt=0;
	uint32 keyVal;
	keyVal= keyValue;
	if(keyVal==KEY_EVENT_POWER)
	{
		keyPowerPressCnt++;
		//keyPowerReleaseCnt=0;
		if(keyPowerPressCnt>=50)
			keyVal=KEY_EVENT_POWEROFF;
		else
			keyVal=0;	
	}
	else
	{
		if(keyPowerPressCnt>0&&keyPowerPressCnt<50){
			//keyPowerReleaseCnt=3;
			keyVal = KEY_EVENT_POWER;
		}
		keyPowerPressCnt=0;
		//keyPowerPressCnt=0;
	}
	return keyVal;
}
#define SCAN_KEY_CONTINUE_INTERVAL  (5)
int board_key_process(INT32U keyValue)
{
	static uint32 LastKeyVal=0;
	static int32 KeyContinueCnt=0;
	uint32 CurKeyVal;
	uint32 KeySta=0;
	CurKeyVal=getKey(keyValue);
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
int board_adkey_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	int value,i;
	if(op == IOCTRL_KEY_POWER)
    {
        value = hal_adcGetChannel(ADC_KEY_CH);
        for(i=0;i<KEY_MAX;i++)
        {
           if(value<=keyValueTable[i])
              break;
        }
        value = (keyTable[i] == KEY_EVENT_POWER);
        
		*(int *)para = value; 
    }
	else if(op == IOCTRL_KEY_READ)
	{
       // value = hal_adcGetChannel(ADC_KEY_CH);
	   // sample 3
		value = (hal_adcGetChannel(ADC_KEY_CH) + hal_adcGetChannel(ADC_KEY_CH) + hal_adcGetChannel(ADC_KEY_CH))/3;
        //if(value <= 350)
		//	debg("[value:%d]",value);
		for(i=0;i<KEY_MAX-1;i++)
        {
           if(value<=keyValueTable[i]){
			   break;
		   }
        }
       value = keyTable[i];

        i = board_key_process(value);	

        *(int *)para = i; 
        if(!i)
           return -1;			   	   
	}
	else if(op == IOCTRL_KEY_POWEROFF)
	{

	}
	return 0;
}

//int board_powerCheck(void)
//{
//	hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_INPUT,GPIO_PULL_DOWN);
//
//	return hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
//}

























Board_Node_T  board_fpga[]=
{	
    {//------gsensor-----------
		.name = DEV_NAME_GSENSOR,
		.init = board_gsensor_init,
		.ioctrl = board_gsensor_ioctrl, 
		.prit = 0
	},
	{
		.name = DEV_NAME_USENSOR,
		.init = board_usb2_init,
		.ioctrl = board_usb2_ioctrl,
		.prit = 0
	},
    {//------usb-----------
		.name = DEV_NAME_USB,
		.init = board_usb_init,
		.ioctrl = board_usb_ioctrl, 
		.prit = 0
	},
	{//------bat-----------
		.name = DEV_NAME_BATTERY,
		.init = board_battery_init,
		.ioctrl = board_battery_ioctrl, 
		.prit = 0
	},
	{//------led-----------
		.name = DEV_NAME_LED,
		.init = board_led_init,
		.ioctrl = board_led_ioctrl,	
		.prit = 0
    },
    {//------lcd-----------
		.name = DEV_NAME_LCD,
		.init = board_lcd_init,
		.ioctrl = board_lcd_ioctrl,	
		.prit = 0
    },   
	{//------cmos -sensor
		.name = "cmos-sensor",
		.init = NULL,//booard_cmos_init,
		.ioctrl = board_cmos_ioctrl,
		.prit=0
	},
	{//------sdc-----------
		.name = DEV_NAME_SDCARD,
		.init = NULL,
		.ioctrl = board_sdc_ioctrl, 
		.prit = 0
	},
	
    {//------ad key-----------
		.name = DEV_NAME_ADKEY,
		.init = board_adkey_init,
		.ioctrl = board_adkey_ioctrl, 
		.prit = 0
	},
			


    {
		.name[0] = 0,
		.init = NULL,
		.ioctrl = NULL,
    }
};



#endif

