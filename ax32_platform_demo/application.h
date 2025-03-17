/****************************************************************************
**
 **                              APPLICATION
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  APPLICATION HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : application.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  APPLICATION_H
    #define  APPLICATION_H
	
// hal layer include
#include "../hal/inc/hal.h"   
// build win lib
#include "../bwlib/bwlib.h" 
// xos
#include "../xos/xos.h"
//-device 
#include "../device/gsensor/gsensor.h"
#include "../device/tft_lcd/tft_lcd.h"
#include "../device/cmos_sensor/cmos_sensor.h"
//-multimedia include.image,audio,video
#include "../multimedia/multimedia.h"

// explorer for file list.resource manager for file manager
#include "./inc/customer.h"
//#include ".\inc\explorer.h"    
#include "./inc/resource_manager.h"  
#include "./inc/config.h"
#include "./inc/board.h"
#include "./inc/display.h"
#include "./resource/resource.h"
#include "./inc/font.h"
#include "./inc/icon.h"


#define SYSDEVICE_Q_SIZE    20

typedef struct System_Ctrl_S
{
	INT8U powerflag;   // power on flag.  key/dcin/gsensor/RTC/...
	INT8U battery;     // battery state;
	INT8U battery_charge;
	INT8U t_usb;           // usb last stat. no usb,dcin,usb-pc
	INT8U usb;           // usb stat. no usb,dcin,usb-pc
	INT8U usensor;     // usensor state
	
	INT8U sdcard;      // sdcard stat.no sdc,sdc unstable,sdc error,sdc full,sdc normal
	INT8U g_lock;      // g sensor active flag
	INT8U f_keysound;  //keysound flag
	INT8U f_update;   //flag update 
	
	INT8U pip_mode;
	INT8U crop_level;	// lcd crop image scale show,
	INT8U astern;     // astern
	INT8U usensorshow;  // usensor show flag
	INT8U bfolder;    // videob folder flag

	INT8S bfd_battery;  // board fd for battery check
	INT8S bfd_usb;       // board fd for usb check
	INT8S bfd_sdcard;   //board fd for sd card check
	INT8S bfd_gsensor; // board fd for gsensor check
	INT8S bfd_ir;		// board fd for ir ctrl
	INT8S bfd_led;      // board fd for led ctrl
	INT8S bfd_lcd;      // board fd for lcd ctrl
	INT8S bfd_key;      // board fd for key check
	INT8S bfd_usensor;  // board fd for usensor detect

	INT32S t_screensave;      // screen save flag
//	INT32S t_autooff;       // auto power off time
	INT32U t_remain;	     // sdc record time
	INT32U t_rec;           // loop rec time
	INT32U t_mrec;
//	INT32U t_ir_flag;		//ir flag: 0: ir close , 1: ir open
	INT32U t_ir_auto;		//ir flag: 0: ir close , 1: ir auto 
	INT32U powerOnTime;
//	INT32U t_powersave;   // power save when dcin out

	INT32S file_index;  // file index for playback
	int         file_type;
	
	INT32S jpg_list;  // avi & jpg file list handle
    INT32S avi_list;
	INT32S wav_list;        // wav file list
	INT32S avia_list;
	INT32S avib_list;

	INT32U sd_freesize;  //sdc free size KB
	INT32U fs_clustsize; // fs cluster size

	INT32U t_wait;    // wait csi & isp stable

	XMsgQ *sysQ;
}System_Ctrl_T;
extern System_Ctrl_T SysCtrl;

extern char tempFileName1[],tempFileName2[];
//-------------system main state table--------------------



//------------system event table-------------------------
enum
{
	SYS_EVENT_SDC =1,    // sdc pulled out or in
	SYS_EVENT_USB,    // usb pulled out or in
	SYS_EVENT_BAT,    // battery state
	SYS_EVENT_FMT,    // sdc formate
	SYS_EVENT_ON,    // power on
	SYS_EVENT_MD,    // motion detetion
	SYS_EVENT_USENSOR,   // usensor pulledout or in
	//SYS_EVENT_ASTERN,  // astern mode
	SYS_EVENT_500MS,
	SYS_EVENT_1S,   //10
	SYS_EVENT_TIME_UPDATE,
	SYS_OPEN_WINDOW,
	SYS_CLOSE_WINDOW,
	SYS_CHILE_COLSE,
	SYS_CHILE_OPEN,
	SYS_TOUCH_WINDOW,
	SYS_DRAW_UI,
	VID_EVENT_REC_STOP,
	VID_EVENT_REC_START,
	VID_EVENT_REC_RESUME,  //20
	VID_EVENT_REC_ERROR,
	VID_EVENT_LOCK_FILE,
	SELFCLOCK_EVENT_POWROFF, // by henry 2022.06.16
	SYS_EVENT_MAX
};
enum
{
	KEY_EVENT_START=SYS_EVENT_MAX,  //23
	KEY_EVENT_OK=KEY_EVENT_START,
	KEY_EVENT_UP,
	KEY_EVENT_DOWN,
	KEY_EVENT_MENU,
	KEY_EVENT_MODE,
	KEY_EVENT_POWER,
	KEY_EVENT_POWEROFF,
	KEY_EVENT_ESC,
	KEY_EVENT_RECORD,//32
    KEY_EVENT_RECORDLONG,
	KEY_EVENT_MENULONG,
	KEY_EVENT_AUDIO,//35
	KEY_EVENT_UPLONG,
	KEY_EVENT_DOWNLONG,
    KEY_EVENT_PHOTO,
    KEY_EVENT_PHOTOLONG,
    KEY_EVENT_RECORD_PHOTO,
    KEY_CLOCK_ON,
    KEY_CLOCK_OFF,
    KEY_EVENT_SAVE,  //43
	KEY_EVENT_END,
	EVENT_MAX=KEY_EVENT_END,
};
/*typedef enum
{
	EVENT_KEY=0,
	EVENT_SYS,
	EVENT_UART,
}EVENT_E;*/
#define makeEvent(type,value)  ((type)|((value)<<16))
#define getType(msg)        (((uint32)msg)&0xffff)
#define getValue(msg)        ((((uint32)msg)>>16)&0xffff)
typedef int (*msgDealFunc)(void* handle,uint32 parameNum,uint32* parame);
typedef struct _msgDealInfor
{
	uint32 msgId;
	msgDealFunc func;
}msgDealInfor;

enum
{
	KEY_PRESSED=0,
	KEY_CONTINUE,
	KEY_RELEASE,
	KEY_STATE_INVALID,
};
//-----------device msg id-----------------
enum
{
	DEVICE_MSG_NULL=0,
	DEVICE_MSG_SDC,
	DEVICE_MSG_USB,
	DEVICE_MSG_BATTERY,
	DEVICE_MSG_GSENSOR,

	DEVICE_MSG_MAX
};

//-----------battery stat-------------------
enum
{
	BATTERY_STAT_0=0,
	BATTERY_STAT_1,
	BATTERY_STAT_2,
	BATTERY_STAT_3,
	BATTERY_STAT_4,
	BATTERY_STAT_5,

	BATTERY_STAT_MAX
};
//-----------power flag-------------------
enum
{
	POWERON_FLAG_KEY=0,
	POWERON_FLAG_DCIN,
	POWERON_FLAG_GSENSOR,
	POWERON_FLAG_RTC,

	POWERON_FLAG_MAX
};
//-----------usb stat-------------------
enum
{
	USB_STAT_NULL = 0,
	USB_STAT_DCIN,
	USB_STAT_PC,

	USB_STAT_MAX
};
//----------sd card stat----------------
enum
{
	SDC_STAT_NULL = 0,
	SDC_STAT_UNSTABLE,
	SDC_STAT_ERROR,
	SDC_STAT_FULL,
	SDC_STAT_NORMAL,

	SDC_STAT_MAX
};
//-----------usensor stat----------------
enum
{
	USENSOR_STAT_NULL=0,
	USENSOR_STAT_UNSTABLE,
	USENSOR_STAT_IN,

	USENSOR_SHOW_OFF=0,
	USENSOR_SHOW_ON,

	USENSOR_STAT_MAX
};
//-----------astern stat-----------------
enum
{
	ASTERN_OFF=0,
	ASTERN_ON
};

enum
{
	USB_DEV_MASS=0,
	USB_DEV_PCCAMERA,
};


#define  IR_CHECK_CH		GPIO_PB
#define  IR_CHECK_PIN		GPIO_PIN6


/*******************************************************************************
* Function Name  : taskSysScanDev
* Description    : system service in deamon
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void taskSysScanDev(uint32 scanKey);

int deamon_screen_save(int event);
/*******************************************************************************
* Function Name  : deamon_auto_poweroff
* Description    : system auto power off check
* Input          : int event : system event
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_auto_poweroff(int ClearPowerOffTime);
/*******************************************************************************
* Function Name  : deamon_fsFreeSize
* Description    : deamon get fs free size
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_fsFreeSize(void);
/*******************************************************************************
* Function Name  : deamon_fsSizeAdd
* Description    : deamon dec size from free size
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_fsSizeModify(INT8S dec,INT32U size);
/*******************************************************************************
* Function Name  : task_usb_host
* Description    : usb host task for uvc & mass
* Input          : void *para : for task initial use
* Output         : none                                            
* Return         : 
*******************************************************************************/
int task_usb_host(void *para);

/*******************************************************************************
* Function Name  : upgrade
* Description    : upgrade
* Input          : 
* Output         : none                                            
* Return         : int : 0: ->upgrade fail
                            -1:->no upgrade file
                           <-1:->upgrade fail,fireware maybe error
*******************************************************************************/
int upgrade(void);
/*******************************************************************************
* Function Name  : self_test
* Description    : self_test
* Input          : 
* Output         : none                                            
* Return         : int : 0: ->self_test pass
*******************************************************************************/
int self_test(void);



void task_video_record_caltime(void);

int logo_image_show(INT32U idx);
int logo_music_show(INT16U idx);
int layout_logo_show(INT8U music_en,INT8U wait,INT8U stat);
char *layout_version_get(void);
unsigned char *layout_keysound_load(INT32U *len);
int deamon_keysound_play(void);

void systemService(void);
void taskChange(void);
void taskInfor(int n);
void hal_check_tfdebg(void);
int intToString(char* str,uint8 len,uint32 data);

void msgDeal(void);
void msgDealByType(uint32 msgType,void* handle,uint32 parameNum,uint32* parame);
void msgFuncRegister(msgDealInfor* infor);
void unshieldedMsgFuncRegister(msgDealInfor* infor);
void msgDealByMsgInfor(msgDealInfor* infor,uint32 msgType,void* handle,uint32 parameNum,uint32* parame);
bool menuIsOpen(void);

#include "taskManage.h"














#endif

