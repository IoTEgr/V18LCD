/****************************************************************************
**
 **                              TASK 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                 TASK USB
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : task_usb.c
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

#define  USB_ICON_MASS     RES_ICON_PCCAM_MODE//RES_ICON_USB_MODE
#define  USB_ICON_CAM      RES_ICON_PCCAM_MODE
extern msgDealInfor usbDeviceMsgDeal[];
static void taskUsbDeviceOpen(uint32 arg)
{
	uint32 mode;
	deg_Printf("usb task enter.0x%x\n",arg);
    boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0);   // screen off
	dispLayerUninit(DISP_LAYER_VIDEO);
	mode = 0;
	//set lcd video layer color
	hal_lcdSetBufYUV(hal_lcdGetShowingBuffer(),0,0x80,0x80);

	if(arg==USB_DEV_PCCAMERA) // pc-cam   
	{
		extern void setSoftwareDetachUsensor(BOOL en);
	   setSoftwareDetachUsensor(TRUE);
	   if(backrec_adpt())
	   {
			backrec_stop();
		}
	   VIDEO_ARG_T arg1;
	   arg1.resolution.width = 1280;
	   arg1.resolution.height= 720;
	   videoRecordInit(&arg1); // enable csi&mjpeg 
	   mode=1;
	}
	else
	{
		mode=0;
	}
	deg_Printf("mode:%d\n",mode);
	if(mode==0)
	 	logo_image_show(USB_ICON_MASS);
	else
		logo_image_show(USB_ICON_CAM);
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY); 
	hal_usbdInit(mode);
	msgFuncRegister(usbDeviceMsgDeal);
    XOSTimeDly(100);
    boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);   // screen off
}
static void taskUsbDeviceClose(uint32 arg)
{
	hal_usbdUninit();
	hal_usbdInit(0);
	dispLayerInit(DISP_LAYER_OSD0);
}
static void taskUsbDeviceService(uint32 arg)
{
	if(false == hal_usbdProcess())
	{
		deg_Printf("usb update\n");
		boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0); // back light off
		taskStart(TASK_USB_UPDATE,0);
		//taskStart(TASK_POWER_OFF,0);
	}

}
sysTask taskUsbDevice=
{
	"usb device",
	NULL,
	taskUsbDeviceOpen,
	taskUsbDeviceClose,
	taskUsbDeviceService,
};


static int usbDeviceSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int usbDeviceSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
	{
		taskStart(TASK_POWER_OFF,0);
	}
	return 0;
}
static int usbDeviceSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
	static int show_count = 0;

	show_count++;

	if(show_count >=40 )
	{
		show_count = 0;
	}

	deg_Printf("show_count=%x\r\n", show_count);
	switch(show_count)
	{
		case 0:logo_image_show(USB_ICON_MASS);
		case 5:logo_image_show(USB_ICON_CAM);
		case 10:logo_image_show(RES_ICON_POWER_OFF);
		case 15:logo_image_show(RES_ICON_POWER_ON);
		default:
			break;
	}
*/
	return 0;
}

static int usbDeviceKeyMsgEsc(winHandle handle,uint32 parameNum,uint32* parame)
{
	taskUsbDeviceClose(0);
    set_usb_to_video_flag(TRUE);
	taskStart(TASK_VIDEO_RECORD, 0);

	return 0;
}

static int usbDeviceKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
    return usbDeviceKeyMsgEsc(handle, parameNum, parame);

	return 0;
}

static int usbDeviceKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

	deamon_clock_check(1);
	deamon_auto_poweroff(1);
	deamon_screen_save(1);

    return usbDeviceKeyMsgEsc(handle, parameNum, parame);

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//CHKPNT;
	}
	return 0;
}

msgDealInfor usbDeviceMsgDeal[]=
{
	{SYS_EVENT_SDC,usbDeviceSysMsgSD},
	{SYS_EVENT_USB,usbDeviceSysMsgUSB},
	{SYS_EVENT_1S,usbDeviceSysMsg1S},
	{KEY_EVENT_MODE,usbDeviceKeyMsgMode},
	{KEY_EVENT_ESC,usbDeviceKeyMsgEsc},
	{KEY_EVENT_POWER,usbDeviceKeyMsgPower},
	{EVENT_MAX,NULL},
};

