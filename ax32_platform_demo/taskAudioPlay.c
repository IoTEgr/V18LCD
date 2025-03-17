#include "application.h"

int audio_play_start(int idx)
{
	Audio_ARG_T arg;
	INT32U totaltime;
	char *name;
	int type;

	name = manangerGetFileFullPathName(SysCtrl.wav_list,idx,&type);
	if(name == NULL)
		return -1;
	arg.media.src.fd = (FHANDLE)open(name,FA_READ);
	if((int)arg.media.src.fd < 0)
		return -1;

	arg.channel = 1;
	arg.samplerate = SAMPLE_RATE_AUTO;
	arg.type = FILE_TYPE_WAV;
	arg.media.type = MEDIA_SRC_FS;

	if(audioPlaybackStart(&arg)<0)
	{
		deg_Printf("audio task : play fail\n");
		return -1;
	}
	audioPlaybackGetTime(&totaltime,NULL);	
	deg_Printf("audio task : play ok.total time = %dm %ds %dms\n",(totaltime/1000)/60,(totaltime/1000)%60,totaltime%1000);
	return 0;
}

void taskAudioPlayOpen(uint32 arg1)
{
	audioPlaybackInit();
	SysCtrl.file_index = 1;
	//audio_play_start(SysCtrl.file_index);
	uiOpenWindow(&audioPlayWindow,0);
}
void taskAudioPlayClose(uint32 arg)
{
	if(audioPlaybackGetStatus() != MEDIA_STAT_STOP)
		audioPlaybackStop();	
    audioPlaybackUninit();
	deg_Printf("audio play task exit\n");
}
void taskAudioPlayService(uint32 arg)
{
	audioPlaybackService();
}

sysTask taskAudioPlay=
{
	"Audio Play",
	NULL,
	taskAudioPlayOpen,
	taskAudioPlayClose,
	taskAudioPlayService,
};



