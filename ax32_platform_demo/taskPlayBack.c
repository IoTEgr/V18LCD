#include "application.h"

extern msgDealInfor playBackMsgDeal[];
extern sysTask taskPlayBack;

#define  TASK_VIDEOPLAY_AUTOPLAY         1      //0,playback end,display last frame of current file
                                                         	  		    // 1,playback end,display first frame of current file
                                             				   // 2,playback end,display first frame of next file
#if TASK_VIDEOPLAY_AUTOPLAY >0
    #define  TASK_VIDEOPLAY_FIRSTFRAME      1
#else
    #define  TASK_VIDEOPLAY_FIRSTFRAME      1
#endif


typedef struct Video_Thumbnail_S
{
	char file_name[MANAGER_NAME_LEN+1];
	INT8U index;
	INT8U stat;
	INT16U file_index;
	INT8U *y_buffer;
	INT8U *uv_buffer;
}Video_Thumbnail_T;

//static Video_Thumbnail_T thnumbnailCtrl[THUMBNAIL_CFG_NUM];


int video_playback_start(int index)
{
	char *name;
	int type;
	if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
          	videoPlaybackStop();
	if(managerFileCount(SysCtrl.avi_list)<1)
		return -1;
	deg_Printf("video play : index = %d ",index);
	name = manangerGetFileFullPathName(SysCtrl.avi_list,index,&type);
	if(name == NULL)
	{
		return -1;
	}
    SysCtrl.file_index = index;
   SysCtrl.file_type=type;
    deg_Printf(": %s\n",name);
	if(type == FILE_TYPE_JPG)
	{
		Image_ARG_T arg;
		disp_frame_t *p_lcd_buffer;

        dispLayerGetResolution(DISP_LAYER_VIDEO,&arg.target.width,&arg.target.height);
        //dispRatioResolutionCalculate(VIDEO_RATIO,&arg.target.width,&arg.target.height);  // jpeg scaler the jpg to target size
		
		p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
		u16 x,y;
		hal_lcdGetVideoPos(&x,&y);
		if(p_lcd_buffer)
			hal_dispframeVideoCfg(p_lcd_buffer,x,y,arg.target.width,arg.target.height);
        arg.yout  = p_lcd_buffer->y_addr;
        arg.uvout = p_lcd_buffer->uv_addr;

		arg.media.type = MEDIA_SRC_FS;
		arg.media.src.fd = (FHANDLE)open(name,FA_READ);
		if((int)arg.media.src.fd<0)
			return -1;
		//layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,index,NULL));
	    arg.wait = 1;  // wait decode end
	    if(imageDecodeStart(&arg)<0)
		{
			deg_Printf("jpg decode fail\n");
			close(arg.media.src.fd);
            dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
			return -1;
		}
		INT16U width,height;
		imageDecodeGetResolution(&width,&height);
	    dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,arg.target.width,arg.target.height,VIDEO_ROTATE);
      
		close(arg.media.src.fd);

		deg_Printf("video player : JPG[%d:%d]\n",width,height);
	}
	else
	{
		VIDEO_PARG_T arg;
		INT16U width,height;
		
		arg.media.type = MEDIA_SRC_FS;
	    arg.media.src.fd = (FHANDLE)open(name,FA_READ);
		if((int)arg.media.src.fd<0)
			return -1;
        //layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,index,NULL));
		dispLayerGetResolution(DISP_LAYER_VIDEO,&width,&height);
		arg.tar.width = width;
		arg.tar.height = height;
		hal_lcdGetVideoPos(&arg.pos.x,&arg.pos.y);
		//dispRatioResolutionCalculate(VIDEO_RATIO,&arg.tar.width,&arg.tar.height); 
		//arg.pos.x = (width-arg.tar.width)>>1;
		//arg.pos.y = (height-arg.tar.height)>>1;
		arg.rotate = VIDEO_ROTATE;
		arg.firstframe = TASK_VIDEOPLAY_FIRSTFRAME; // pause at first frame waiting user key
		
        fs_fastseek_init(arg.media.src.fd);  // enable fast seek for this file
        
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end
		if(videoPlaybackStart(&arg)<0)
		{
			deg_Printf("avi decode fail<0x%x>\n",(int)arg.media.src.fd);
			close(arg.media.src.fd);
			return -1;
		}
		
		Media_Res_T *res= videoPlaybackGetResolution();
		INT32U totatime;
		videoPlaybackGetTime(&totatime,NULL);
		deg_Printf("video player : AVI[%d:%d],total time = %dm %ds %dms\n",res->width,res->height,(totatime/1000)/60,(totatime/1000)%60,totatime%1000);
	}
    //layout_video_playback_name(manangerGetFileName(SysCtrl.avi_list,index));
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	deg_Printf("video play start.<%s>\n",name);
	return type;
	
}

typedef struct _imageCopyRect
{
	u32 srcX;
	u32 srcY;
	u32 destX;
	u32 destY;
	u32 w;
	u32 h;
}imageCopyRect;

void imageRegionCopy(u8* destBuff,u32 destW,u32 destH,u8* srcBuff,u32 srcW,u32 srcH,imageCopyRect* copy)
{
	u8 *destYaddr;
	u8 *destUVaddr;
	u8 *srcYaddr;
	u8 *srcUVaddr;
    u16 i,j;
	destW=(destW+0x1f)&(~0x1f);
	srcW=(srcW+0x1f)&(~0x1f);
	copy->srcX=copy->srcX&(~0x01);
	copy->srcY=copy->srcY&(~0x01);
	copy->destX=copy->destX&(~0x01);
	copy->destY=copy->destY&(~0x01);
	//===y===
	destYaddr = destBuff;
	destYaddr += destW*copy->destY;
	destYaddr += copy->destX;
	srcYaddr = srcBuff;
	srcYaddr += srcW*copy->srcY;
	srcYaddr += copy->srcX;
	for(j=0;j<copy->h;j++)
	{	
		for(i=0;i<copy->w;i++)
		{
			*destYaddr++ = *srcYaddr++;
		}
		srcYaddr = srcBuff+srcW*(j+1+copy->srcY)+copy->srcX;
		destYaddr = destBuff+destW*(j+1+copy->destY)+copy->destX;
	}
	//===uv===
	destUVaddr = destBuff + destW*destH;
	destUVaddr += ((destW*copy->destY)>>1);
	destUVaddr += copy->destX;
	srcUVaddr = srcBuff + srcW*srcH;
	srcUVaddr += ((srcW*copy->srcY)>>1);
	srcUVaddr += copy->srcX;
	for(j=0;j<(copy->h>>1);j++)
	{
		for(i=0;i<copy->w;i++)
		{
			*destUVaddr++ = *srcUVaddr++;// U
		}
		srcUVaddr = srcBuff + srcW*srcH+srcW*(j+1+(copy->srcY>>1))+copy->srcX;
		destUVaddr = destBuff + destW*destH+destW*(j+1+(copy->destY>>1))+copy->destX;
	}
}


void taskPlayBackOpen(uint32 arg)
{
	deg_Printf("video playback task enter.\n");
	deg_Printf("video a count = %d\n",managerFileCountEx(SysCtrl.avia_list));
	deg_Printf("video b count = %d\n",managerFileCountEx(SysCtrl.avib_list));
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end
	videoPlaybackInit();
	videoPlaybackSetVolume(100);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);

	if(managerFileCount(SysCtrl.avi_list)>0)
	{
		SysCtrl.file_index =managerFileCount(SysCtrl.avi_list);  //by henry 2022.01.19
		video_playback_start(SysCtrl.file_index); 		  
	}
	//msgFuncRegister(playBackMsgDeal);
	uiOpenWindow(&playBackWindow,0);
	
}

void taskPlayBackService(uint32 arg)
{
	static uint32 lastPlayTime=0;
	uint32 playTime=0;
	#if TASK_VIDEOPLAY_AUTOPLAY>0
	if((videoPlaybackGetStatus() == MEDIA_STAT_STOP) && (SysCtrl.file_type==FILE_TYPE_AVI)&&(SysCtrl.astern != ASTERN_ON))
	{
        #if (TASK_VIDEOPLAY_AUTOPLAY > 1)
		SysCtrl.file_index++;
		if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
			SysCtrl.file_index = 1;
        #endif
		video_playback_start(SysCtrl.file_index);	
	}
    #endif
	videoPlaybackGetTime(NULL,&playTime);
	//playTime=playBackGetPlayTime();
	//if(playTime !=0)
	//debg("playTime = :%d\n",playTime);
	playTime = playTime/1000;
	if(lastPlayTime!=playTime)
	{
		lastPlayTime=playTime;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	}

	videoPlaybackService();
}
void taskPlayBackClose(uint32 arg)
{
	if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
          videoPlaybackStop();
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end	
	videoPlaybackUninit();
	hal_mjpB_ecdown();
	//winDestroy(&taskPlayBack.handle);
	deg_Printf("video playback task exit.\n");
}
sysTask taskPlayBack=
{
	"Play Back",
	NULL,
	taskPlayBackOpen,
	taskPlayBackClose,
	taskPlayBackService,	
};


