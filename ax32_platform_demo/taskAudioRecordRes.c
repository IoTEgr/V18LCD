#include"uiWin.h"

enum
{
	AUDIO_REC_TIME_ID=0,
    AUDIO_REC_WORK_ID=1,
    AUDIO_REC_FILENAME_ID=2,
	AUDIO_REC_BATERRY_ID,

	AUDIO_REC_TIME_H1_ID,
	AUDIO_REC_TIME_H2_ID,
	AUDIO_REC_TIME_1_ID,// :
	AUDIO_REC_TIME_M1_ID,
	AUDIO_REC_TIME_M2_ID,
	AUDIO_REC_TIME_2_ID,// :
	AUDIO_REC_TIME_S1_ID,
	AUDIO_REC_TIME_S2_ID,

	AUDIO_REC_MIC_ID, //MIC
	AUDIO_REC_NAME0_ID,
	AUDIO_REC_NAME1_ID,
	AUDIO_REC_NAME2_ID,
	AUDIO_REC_NAME3_ID,
	AUDIO_REC_NAME4_ID,
	AUDIO_REC_WAV_ID, //WAV
	
};


static widgetCreateInfor audioRecordeWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),/*R_COLOR_GRAY*/R_COLOR_BLACK,WIN_ABS_POS),

#if 0
    createImageIcon(AUDIO_REC_BATERRY_ID,      Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringIcon(INVALID_WIDGET_ID,Rx(30), Ry(50), Rw(260), Rh(40),strToResId("Audio Record"),ALIGNMENT_CENTER, R_COLOR_YELLOW,2),
	createStringIcon(AUDIO_REC_TIME_ID,Rx(100), Ry(95), Rw(120), Rh(25),strToResId("00:00:00"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
    createImageIcon(AUDIO_REC_WORK_ID,      Rx(140), Ry(170), Rw(40), Rh(40), R_ID_ICON_AUDIORECORD,ALIGNMENT_CENTER),
    createStringIcon(AUDIO_REC_FILENAME_ID, Rx(60), Ry(130), Rw(200), Rh(30),strToResId("MIC00001.WAV"),ALIGNMENT_CENTER, R_COLOR_WHITE,1),
#else
	createImageIcon(AUDIO_REC_BATERRY_ID,      Rx(290), Ry(0), Rw(28), Rh(16), R_ID_ICON_AUBATTERY3,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_WORK_ID,        Rx(40), Ry(100+18), Rw(240), Rh(40), R_ID_ICON_AUDIORECORDSTART,ALIGNMENT_CENTER),

	createImageIcon(AUDIO_REC_TIME_H1_ID,		Rx(60), Ry(28), Rw(28), Rh(28), R_ID_ICON_TIME0_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_TIME_H2_ID,		Rx(88), Ry(28), Rw(28), Rh(28), R_ID_ICON_TIME0_P,ALIGNMENT_CENTER),
	
	createImageIcon(AUDIO_REC_TIME_1_ID,		Rx(116), Ry(28), Rw(16), Rh(28), R_ID_ICON_TIME_P,ALIGNMENT_CENTER), 
	
	createImageIcon(AUDIO_REC_TIME_M1_ID,		Rx(132), Ry(28), Rw(28), Rh(28), R_ID_ICON_TIME0_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_TIME_M2_ID,		Rx(160), Ry(28), Rw(28), Rh(28), R_ID_ICON_TIME0_P,ALIGNMENT_CENTER),
	
	createImageIcon(AUDIO_REC_TIME_2_ID,		Rx(188), Ry(28), Rw(16), Rh(28), R_ID_ICON_TIME_P,ALIGNMENT_CENTER),  
	
	createImageIcon(AUDIO_REC_TIME_S1_ID,		Rx(204), Ry(28), Rw(28), Rh(28), R_ID_ICON_TIME0_P,ALIGNMENT_CENTER), 
	createImageIcon(AUDIO_REC_TIME_S2_ID,		Rx(232), Ry(28), Rw(28), Rh(28), R_ID_ICON_TIME0_P,ALIGNMENT_CENTER), //4

	createImageIcon(AUDIO_REC_MIC_ID,		Rx(44-4), Ry(220), Rw(56), Rh(20), R_ID_ICON_MIC,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_NAME0_ID,		Rx(100), Ry(220), Rw(20), Rh(20), R_ID_ICON_NAME0_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_NAME1_ID,		Rx(120), Ry(220), Rw(20), Rh(20), R_ID_ICON_NAME1_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_NAME2_ID,		Rx(140), Ry(220), Rw(20), Rh(20), R_ID_ICON_NAME2_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_NAME3_ID,		Rx(160), Ry(220), Rw(20), Rh(20), R_ID_ICON_NAME3_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_NAME4_ID,		Rx(180), Ry(220), Rw(20), Rh(20), R_ID_ICON_NAME4_P,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_REC_WAV_ID,		Rx(200+4), Ry(220), Rw(76), Rh(20), R_ID_ICON_WAV,ALIGNMENT_CENTER),
#endif
widgetEnd(),
};


static void audioRecTimeShow(winHandle handle,INT32U secodes)
{

	static char TimeStr[9];
	INT32U Minutes;
	Minutes=secodes/60;
	secodes=secodes%60;
#if 0
	TimeStr[0]='0';
	TimeStr[1]='0';
	TimeStr[2]=':';
	TimeStr[3]=(Minutes/10)+'0';
	TimeStr[4]=(Minutes%10)+'0';
	TimeStr[5]=':';
	TimeStr[6]=(secodes/10)+'0';
	TimeStr[7]=(secodes%10)+'0';
	TimeStr[8]=0;
	winSetResid(winItem(handle,AUDIO_REC_TIME_ID),strToResId(TimeStr));
#else
	winSetResid(winItem(handle,AUDIO_REC_TIME_M1_ID),R_ID_ICON_TIME0_P + Minutes/10);
	winSetResid(winItem(handle,AUDIO_REC_TIME_M2_ID),R_ID_ICON_TIME0_P + Minutes%10);

	winSetResid(winItem(handle,AUDIO_REC_TIME_S1_ID),R_ID_ICON_TIME0_P + secodes/10);
	winSetResid(winItem(handle,AUDIO_REC_TIME_S2_ID),R_ID_ICON_TIME0_P + secodes%10);

#endif

}

static void audioRecBaterryShow_charge(winHandle handle)
{
	winSetVisible(winItem(handle,AUDIO_REC_BATERRY_ID),false);
	switch(SysCtrl.battery_charge)
	{
		case  BATTERY_STAT_0:
			winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY0);
			break;
		case  BATTERY_STAT_1:
			winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY1);
			break;
		case  BATTERY_STAT_2:
			winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY2);
			break;
		case  BATTERY_STAT_3:
			winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY3);
		break;
		case  BATTERY_STAT_4:
			winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY4);
		break;
		case  BATTERY_STAT_5:
			winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY5);
		break;
		default:
			deg_Printf("[%d] %s\r\n", __LINE__, __FUNCTION__);
		break;
	}
	winSetVisible(winItem(handle,AUDIO_REC_BATERRY_ID),true);
}

static audioRecBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		audioRecBaterryShow_charge(handle);
		//winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,AUDIO_REC_BATERRY_ID),R_ID_ICON_AUBATTERY0);
	winSetVisible(winItem(handle,AUDIO_REC_BATERRY_ID),true);
}


//<AUDIO/MIC00016.WAV>
static char audio_filename[30] = {0};

int audioGetCurFileName(char *filename)
{
    strncpy(audio_filename, filename, sizeof(audio_filename));
    deg_Printf("audio_filename= %s\r\n", audio_filename);
}

int audioParseCurFileName(char *filename)
{
    int num = 0;
    int pos = 0;

    if(audio_filename == NULL) 
    {
        CHKPNT;
        return;
    }

    for(num = 0; num < sizeof(audio_filename); num++)
    {
        if(audio_filename[num] == '>')
        {
            break;
        }

        if(audio_filename[num] == '/')
        {
            pos = num+1;
        }

        if(pos > 0)
        {
            filename[num - pos] = audio_filename[num];            
        }
    }

    return 0;
}

static void audioFileNameShow(winHandle handle)
{
	uint8 i=0;
	static char filename[15] = {0};

    audioParseCurFileName(&filename);
    deg_Printf("FINE filename= %s\r\n", filename);

    //SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);

#if 0
	winSetResid(winItem(handle,AUDIO_REC_FILENAME_ID), strToResId(filename));
#else
	for(i=0;i<5;i++)
	{
		winSetResid(winItem(handle,AUDIO_REC_NAME0_ID + i ),R_ID_ICON_NAME0_P + ( filename[3+i] -'0') );
	}


#endif
}

uint8 g_audio_state_flag=0xff;
void AudioShow_RecState(winHandle handle)
{
 	if ( audioRecordGetStatus() == MEDIA_STAT_STOP )
	{
		if( g_audio_state_flag != 1)
		{
			g_audio_state_flag =1;
			winSetResid(winItem(handle,AUDIO_REC_WORK_ID),R_ID_ICON_AUDIORECORDSTOP);
		}
	}
 	else
 	{
		if( g_audio_state_flag != 0)
		{
			g_audio_state_flag =0;
			winSetResid(winItem(handle,AUDIO_REC_WORK_ID),R_ID_ICON_AUDIORECORDSTART);
		}
 	}
}


