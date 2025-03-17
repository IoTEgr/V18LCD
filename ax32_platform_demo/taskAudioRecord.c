/****************************************************************************
**
 **                              TASK DEMO
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  TASK AUDIO RECORD DEMO
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : task_audio_record.c
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
#include "application.h"

int audio_record_callback(INT32U cmd,INT32U para)
{
    FHANDLE *fd;
	int fdt;
    char *name;
	
	if(cmd == CMD_AUDIO_RECORD_START)
	{
		name = managerFileNameCreater(SysCtrl.wav_list,SUFFIX_WAV,FILEDIR_AUDIO);
		if(name == NULL)
		{
			deg_Printf("audio : create file name fail.\n");
			if(managerFileCount(SysCtrl.wav_list)>=MANAGER_LIST_MAX)
			{
				deg_Printf("audio : file num full.\n");
				audioRecordSetStatus(MEDIA_STAT_FILE_FULL);
			}
			return -1;
		}

		fdt = open(name,FA_CREATE_NEW | FA_WRITE | FA_READ);
		if(fdt<0)
		{
			deg_Printf("audio : open file fail<%s>,%d\n",name,fdt);
			return -1;
		}
		fd = (FHANDLE *)para;
		*fd = (FHANDLE)fdt;
		managerAddFile(SysCtrl.wav_list,FILEDIR_AUDIO,&name[strlen(FILEDIR_AUDIO)]);
		deg_Printf("audio : open file ok<%s>\n",name);
        audioGetCurFileName(name);

		return 0;
	}
	else if(cmd == CMD_AUDIO_RECORD_STOP)
	{
		fd = (FHANDLE *)para;
		deamon_fsSizeModify(-1,fs_size((FHANDLE)*fd));
		close((FHANDLE)*fd);
		return 0;
	}
	else if(cmd == CMD_COM_ERROR)
	{
		if(para == CMD_AUDIO_RECORD_START)
		{
			fd = (FHANDLE *)para;
			close((FHANDLE)*fd);
			return 0;
		}
	}
	else if(cmd == CMD_COM_CHECK) // check sdc sapce
	{
		return managerSpaceCheck(SysCtrl.wav_list,FILEDIR_AUDIO,para);
	}

    return 0;
}
void taskAudioRecordeOpen(uint32 arg1)
{
	AUDIO_RARG_T arg;
	int ret;
	INT16U value,event;

	deg_Printf("audio task enter\n");
	arg.channel = 1;
	arg.samplerate = SAMPLE_RATE_16000;
	arg.looprec = 1;
	arg.looptime = 3*60*10; //3*60; // second
	arg.tar_type = WAV_TYPE_PCM;
    arg.callback = audio_record_callback;

    audioRecordInit(&arg);

	if(managerSpaceCheck(SysCtrl.wav_list,FILEDIR_AUDIO,0)<0) // check space
	{
		hal_sdUnlock();
		SysCtrl.sdcard = SDC_STAT_FULL;
		deg_Printf("wav_list full\n");
	}

	uiOpenWindow(&audioRecordWindow,0);
}
void taskAudioRecordeClose(uint32 arg)
{
	if(audioRecordGetStatus()==MEDIA_STAT_START)
		audioRecordStop();
	audioRecordUninit();
	deg_Printf("audio task exit\n");
}
void taskAudioRecordeService(uint32 arg)
{
	static int lastRecTime=0;
	static u32 lastCheckTime = 0;
	audioRecordService();
	if(lastRecTime!=audioRecordGetTime())
	{
		lastRecTime=audioRecordGetTime();
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	}
	if(MEDIA_STAT_STOP == audioRecordGetStatus())
	{
		if(XOSTimeGet()-lastCheckTime > 500)
		{
			lastCheckTime = XOSTimeGet();
			if(managerSpaceCheck(SysCtrl.wav_list,FILEDIR_AUDIO,0)<0) // check space
			{
				hal_sdUnlock();
				SysCtrl.sdcard = SDC_STAT_FULL;
			}
		}
	}
}

sysTask taskAudioRecorde=
{
	"Audio Recorde",
	NULL,
	taskAudioRecordeOpen,
	taskAudioRecordeClose,
	taskAudioRecordeService,
};



