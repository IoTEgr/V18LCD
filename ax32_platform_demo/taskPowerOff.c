#include "application.h"


int uninit(void)
{
	int ret,temp;

//    hal_batDetectEnable(0);
    boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0); // back light off
    deg_Printf("system power off\n");

    if(SysCtrl.battery != 0)
    {
	    while(1) // wait power release
	    {
	        ret = boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			if((ret<0) ||(temp==0))
				break;
	    }
    }

//	boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWEROFF,0);
	temp = configValue2Int(CONFIG_ID_PARKMODE);
    boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_STABLE,temp); // wait gsensor stable	

    boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETPARK,temp); // gsensor park mode set.do not care gsensor.
    boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0); // led off
    boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0); // usensor off
    hal_timerStop(TIMER0); 
	hal_sysUninit();


//  XOSTimeDly(100);
//	hal_timerStop(TIMER0); 

	boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(temp && (SysCtrl.usb==USB_STAT_NULL))
	{
		deg_Printf("during system off, usb dc in, restart system \n");
		//ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
		//ax32xx_wkiCleanPending();
		//hal_vddWKOEnable(0);
		SysCtrl.usb = USB_STAT_DCIN;
		hal_sysReset(); // reset
		while(1);
		//	hal_vddWKOEnable(1);
		//	goto POWER_OFF_DCIN;
	}else{
		ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
		ax32xx_wkiCleanPending();
		hal_vddWKOEnable(0);
		ax32xx_sysCpuMsDelay(50);
	}
	while(1)
	{
		boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_LOCK,(INT32U)&temp);
		if(hal_rtcAlarmStatusGet(1))
			hal_sysReset();
		temp = 0;
		boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
		if(temp && (SysCtrl.usb==USB_STAT_NULL))
		{
			SysCtrl.usb = USB_STAT_DCIN;
			hal_sysReset(); // reset
		//	hal_vddWKOEnable(1);
		//	goto POWER_OFF_DCIN;
		}
		//XOSTimeDly(1000);
		//deg_Printf(".");
		//hal_vddWKOEnable(0);
       // ax32xx_wkiCleanPending();
	}
	return 0; // makesure the program won't be here
}

void taskPowerOffOpen(uint32 arg)
{
	uint32 lcdOn=0;
	
#if KEY_PRESSED_POWER_TAP
    if(get_key_press_power_clock() == 0)
#endif

		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn==0)
	        boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);   // screen on	

	if(!get_key_press_power_clock())
	{
		layout_logo_show(0,1,0); //layout_logo_show(1,1,0);                                       // power off.music en,wait music end
	}
    winUninit();
    userConfigSave();
	uninit();
}

sysTask taskPowerOff=
{
	"Power Off",
	NULL,
	taskPowerOffOpen,
	NULL,
	NULL,
};


