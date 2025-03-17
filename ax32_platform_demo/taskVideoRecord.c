#include "application.h"


char tempFileName1[MANAGER_NAME_LEN+1+8],tempFileName2[MANAGER_NAME_LEN+1+8];  // there are two memories used by different program temparlly.do not mask
extern int task_image_createfile(int channel,char **filename);
extern msgDealInfor videoRecordeMsgDeal[];
extern sysTask taskVideoRecorde;

/*******************************************************************************
* Function Name  : video_record_callback
* Description    : video record callback function.user should fill the function,video record service will callback
* Input          : 
* Output         : none                                            
* Return         : int fd : file handle
*******************************************************************************/
int video_record_callback(INT32U channel,INT32U cmd,INT32U para)
{
	static INT8U chBfileFlag = 0; //channel B created file flag,for lock file using
	char *name;
    FHANDLE *fdt;
	int fd[2],ret=0,index,rectime;
	INT32U bsize,size;
	static INT32U oldfilesizeA,oldfilesizeB;
	if(channel >= VIDEO_CH_MAX)
	{
		deg_Printf("video : unknow channel.%d\n",channel);
		return -1;
	}
    
	if(cmd == CMD_VIDEO_RECORD_START) // video record start
	{
		XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_RESUME,0));
	#if (1 == UPDATE_SETTING_EN)
		rectime = get_info_value("filetime") * 60; //configValue2Int(CONFIG_ID_LOOPTIME);
	#else
		rectime = configValue2Int(CONFIG_ID_LOOPTIME);
	#endif
		if(rectime == 0)
		{
			size = videoRecordSizePreSec(VIDEO_CH_A,10*60);
			if(backrec_sensor_stat())
				size += videoRecordSizePreSec(VIDEO_CH_B,10*60);
			if(SysCtrl.sd_freesize<size)
			{
				SysCtrl.sdcard = SDC_STAT_FULL;
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
				sysLog("un sycl record full");
				XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
				return -1;
			}
			rectime = 10*60;
		}
		fd[0] = -1;
		if(channel == VIDEO_CH_A)  // video A
		{
			//msgDealByType(VID_EVENT_REC_START,winGetCurrent(),0,NULL);
		//	mediaVideoCtrl.stat = MEDIA_STAT_START;
		#if 0 //#if RECORD_MODE == RECORD_MODE_AUTO  // create videob folder
             if((SysCtrl.bfolder==0)&&(SysCtrl.usensor!=USENSOR_STAT_NULL))
             {
			 	strcpy(tempFileName1,FILEDIR_VIDEOB);
				tempFileName1[strlen(tempFileName1)-1] = 0;
				f_mkdir((const TCHAR*)tempFileName1);//FILEDIR_VIDEO);
				SysCtrl.bfolder = 1;
				deg_Printf("video : create folder %s\n",tempFileName1);
				tempFileName1[0] = 0;
             }
		#endif	
			oldfilesizeA = 0;
		#if AVI_TYPE_ODML	
			bsize = videoRecordSizePreSec(VIDEO_CH_A,rectime);
		    if(backrec_sensor_stat())//(api_backrec_status())
				bsize += videoRecordSizePreSec(VIDEO_CH_B,rectime);
			//bsize+=(5*1024);  // for take photo
			memset(tempFileName2,0,MANAGER_NAME_LEN+1+8);
        #if (1 == UPDATE_SETTING_EN)
			if((bsize>SysCtrl.sd_freesize) && (get_info_value("recycle")))
        #else
			if(bsize>SysCtrl.sd_freesize)
        #endif
			{
				index = 1;
				name = manangerFindFileAndDel(SysCtrl.avi_list,&index,MA_FILE_AVIA);
				if(name)
				{
					 fd[0] = open(name,FA_WRITE | FA_READ);	
					 if(fd[0]>=0)
					 {
					 	 strcpy(tempFileName2,name);
						 size = fs_size(fd[0])>>10;
						 bsize = videoRecordSizePreSec(VIDEO_CH_A,rectime);
                         if((size<=(bsize+100*1024))&&(size>=bsize))
                         {
						 	oldfilesizeA =  bsize = fs_size(fd[0])>>10;
							managerDelFileByIndex(SysCtrl.avi_list,index);
                         }
						 else
						 {
						 	close(fd[0]);
							fd[0] = -1;
							oldfilesizeA = bsize = 0;
						 }
					 }
				}
				if(fd[0]<0)
				{
					oldfilesizeA = bsize = 0;
					//tempFileName2[0] = 0;
					memset(tempFileName2,0,MANAGER_NAME_LEN+1+8);
				}
			}
         #endif   
			name = managerFileNameCreater(SysCtrl.avi_list,SUFFIX_AVI,FILEDIR_VIDEOA); // the static memory is unsafe,maybe used by other programe when video recording
			if(name == NULL)
			{
				SysCtrl.sdcard = SDC_STAT_FULL;
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
				memset(tempFileName1,0,MANAGER_NAME_LEN+1+8);
				//tempFileName1[0] = 0;
				sysLog("managerFileNameCreater fail");
				XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
				return -1;
			}
			strcpy(tempFileName1,name);	

		}
		else if(channel == VIDEO_CH_B)// video B
		{	
			oldfilesizeB = 0;
			if(tempFileName1[0] == 0)
			{
				deg_Printf("video : channel error.%d\n",channel);
				XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
				return -1;
			}
			memcpy(tempFileName2,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB)); // create B video file
			memcpy(tempFileName1,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB)); // create B video file
			//deg_Printf("B:%s\n",tempFileName2);
		#if AVI_TYPE_ODML		
			if(managerCheckLock(&tempFileName2[strlen(FILEDIR_VIDEOB)])!=0)
				fd[0] = -1;
			else
			    fd[0] = open(tempFileName2,FA_WRITE | FA_READ);					
			if(fd[0]>=0) // delete file
			{
				 bsize = videoRecordSizePreSec(VIDEO_CH_B,rectime);
				 size = fs_size(fd[0])>>10;
				 if((size<=(bsize+100*1024))&&(size>=bsize))
				 {
				 	ret = managerDelFileByFullName(SysCtrl.avi_list,tempFileName2);
					oldfilesizeB = bsize = size ;
				 }
				 else
				 {
				 	close(fd[0]);
					fd[0] = -1;
					oldfilesizeB = bsize = 0;
				 }
			}
		#endif	
		}
		if(fd[0]<0)
		{
			fd[0] = open(tempFileName1,FA_CREATE_ALWAYS | FA_WRITE | FA_READ);  // FA_CREATE_NEW
			bsize = 0;
		}
		else
		{
			bsize = fs_size(fd[0]);
			close(fd[0]);
			f_rename(tempFileName2,tempFileName1);  // must close before rename 
			fd[0] = open(tempFileName1,FA_WRITE | FA_READ);
			//f_rename(tempFileName2,tempFileName1);
			deg_Printf("using old file:%s.%d b\n",tempFileName2,bsize);
			//sysLog("using old file.");
		}
	 //   fd[0] = open(tempFileName1,FA_WRITE | FA_READ);  // FA_CREATE_NEW
		deg_Printf("video : record start->%s ",tempFileName1);	
	//	sysLog(tempFileName1);

		
	#if AVI_TYPE_ODML == 0		    
		if(channel == VIDEO_CH_A)
		{
			fd[1] = open("VIDEOA.TMP",FA_CREATE_ALWAYS | FA_WRITE | FA_READ);  // FA_CREATE_NEW
		}
		else if(channel == VIDEO_CH_B)
		{
			fd[1] = open("VIDEOB.TMP",FA_CREATE_ALWAYS | FA_WRITE | FA_READ);  // FA_CREATE_NEW	
		}
		
	#else
	    fd[1] = fd[0];
	#endif
		if((fd[0]>=0) && (fd[1]>=0)) // success
		{	        
			fdt = (FHANDLE *)para;
			*fdt = (FHANDLE)fd[0];
			*(fdt+1) = (FHANDLE)fd[1];
			deg_Printf("success.\n");
			if(bsize == 0) // try to alloc size
			{
			//----count video file size	
				if(channel == VIDEO_CH_A)
				{
					if (oldfilesizeA)
						size = bsize = oldfilesizeA;
					else 
					size = bsize = videoRecordSizePreSec(VIDEO_CH_A,rectime);
				}
				else 
				{
					if (oldfilesizeB)
						size = oldfilesizeB ;
					else	
					size = videoRecordSizePreSec(VIDEO_CH_B,rectime);
					if (oldfilesizeA)
						bsize = size + oldfilesizeA;
					else
					bsize = size+videoRecordSizePreSec(VIDEO_CH_A,rectime);
				}
            #if (1 == UPDATE_SETTING_EN)
                //deg_Printf("bsize=%d, remain_space=%d \n", bsize, SysCtrl.sd_freesize);
                if((get_info_value("recycle") == 0) && (SysCtrl.sd_freesize < bsize)) //bsize malloc 880M
                {
                    bsize = 0;
                }
            #endif
				ret = managerSpaceCheck(SysCtrl.avi_list,FILEDIR_VIDEO,bsize);
				if((ret<0)||((ret<<10)<bsize))
				{
					deg_Printf("no more space\n");
					//sysLog("no more space");
				    SysCtrl.sdcard = SDC_STAT_FULL;
					XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
					sysLog("managerSpaceCheck no space");
					if(videoRecordGetStatus() == MEDIA_STAT_START)  // stop current recording
					{
				       	XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
					}
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				if(size>0x3c0000) // 3.8G
				   size = 0x3c0000;
			#if AVI_TYPE_ODML		
			#if FILE_SIZE_PRE_MALLOC
				ret = XOSTimeGet();	
				if(lseek(fd[0],size<<10,0)<0)
				{			
				    deg_Printf("lseek fail.%d b,%dkb\n",size<<10,size);
					//sysLog("lseek fail");
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				fs_sync(fd[0]);
				ret = XOSTimeGet()-ret;
				if(fs_size(fd[0])<(size<<10))
				{
					debg("fs_size:%d,size:%d\n",fs_size(fd[0]),size<<10);
					deg_Printf("lseek fail.\n");
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				lseek(fd[0],0,0);
				bsize = fs_size(fd[0]);	
				deg_Printf("fs alloc time using : %dms,file size : %dkb,speed = %dkb/ms.\n",ret,bsize>>10,(bsize>>10)/ret);
			#endif
			#endif	
			}
			else
			{
				deamon_fsSizeModify(1,bsize);
			}
		#if AVI_TYPE_ODML		
	 		#if FILE_SIZE_PRE_MALLOC
				ret = XOSTimeGet();
				if(fs_fastseek_init(fd[0])<0)
				{
					deg_Printf("fs_fastseek_init fail.\n");
					//sysLog("fs_fastseek_init fail");
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				ret = XOSTimeGet()-ret;
				deg_Printf("fs_fastseek_init time using : %dms.\n",ret);
				if(channel == VIDEO_CH_B)
					chBfileFlag = 2;
				else
					chBfileFlag = 1;
				return (bsize>>10);
			#else		
				return bsize;
			#endif
		#else	
			//if(channel == VIDEO_CH_A)
			//	    SysCtrl.t_rec = 0;			
				return (SysCtrl.sd_freesize>>10);
		#endif	
		}
		else //fail
		{			
		    deg_Printf("fail.\n");
			ret = -1;
			goto VIDEO_RECORD_ERROR_HANDLE;
		}
	}
	else if(cmd == CMD_VIDEO_RECORD_STOP)// video record start
	{
		//if( channel == VIDEO_CH_A)
		{
		//	SysCtrl.t_rec = 0;
		}
		//SysCtrl.t_rec = 0;
		fdt = (FHANDLE *)para;
        		fd[0] = (int)*fdt;
		fd[1] = (int)*(fdt+1);
		if(SysCtrl.sdcard != SDC_STAT_NORMAL)
		{
			SysCtrl.g_lock=0;
			XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_STOP,0));
			goto VIDEO_RECORD_ERROR_HANDLE;
		}
		bsize = fs_size(fd[0]);
		if(bsize>0)
			deamon_fsSizeModify(-1,bsize);
		bsize>>=10;
		deg_Printf("video : fs size = %dG%dM%dKB\n",bsize>>20,(bsize>>10)&0x3ff,(bsize)&0x3ff);
		close((FHANDLE)fd[0]);
	#if AVI_TYPE_ODML == 0		
		close((FHANDLE)fd[1]);
	#endif
		name = tempFileName1;		    
		if(SysCtrl.g_lock)// video A
		{	
			#if (0 == AVI_LOCK_FOLDER)
				managerUnlockFile(name);  // maybe file name is locked when the second callback
				strcpy(tempFileName2,tempFileName1);  // backup file name
				managerLockFile(tempFileName1); // rename in resource manager list
				if(channel == VIDEO_CH_A)
				{
					memcpy(tempFileName2,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));
					memcpy(tempFileName1,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));						
				}
				else if(channel == VIDEO_CH_B)
				{
					memcpy(tempFileName2,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
					memcpy(tempFileName1,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
				}
			#else
				managerUnlockFile(name);  // maybe file name is locked when the second callback
				strcpy(tempFileName2,tempFileName1);  // backup file name
				managerLockFile(tempFileName1); // rename in resource manager list
				if(channel == VIDEO_CH_A)
				{
					memcpy(tempFileName2,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));
					memcpy(tempFileName1,FILEDIR_LOCKA,strlen(FILEDIR_LOCKA));						
				}
				else if(channel == VIDEO_CH_B)
				{
					memcpy(tempFileName2,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
					memcpy(tempFileName1,FILEDIR_LOCKB,strlen(FILEDIR_LOCKB));
				}
			#endif
			f_rename(tempFileName2,tempFileName1);  // rename in file system
			deg_Printf("video : rename  .%s->%s\n",tempFileName2,tempFileName1);
			deg_Printf("video : lock this file.\n");
		}

		{
			int i=0,j=0;
			while(tempFileName1[j])
			{
				if(tempFileName1[j] == '/')
					i = j;
				j++;
			}
			if((i==0)&&(tempFileName1[0] != '/'))
				i=0;
			else
				i+=1;
			name = &tempFileName1[i];	// point to file name
		}
		
		//name = &tempFileName1[strlen(FILEDIR_VIDEOA)];
		deg_Printf("video : stop ");
		if(channel == VIDEO_CH_A)
		{
			oldfilesizeA = 0;
			ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOA,name);	// add video a
			deg_Printf("%s%s\n",FILEDIR_VIDEOA,name);

		}
		else if(channel == VIDEO_CH_B)
		{
			ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOB,name);	// add video b
			deg_Printf("%s%s\n",FILEDIR_VIDEOB,name);

		}
		if(chBfileFlag)
		{
		     chBfileFlag--;
			 if(chBfileFlag == 0)
			 {
			 	SysCtrl.g_lock = 0; // clear g-sensor lock active flag
			 }
		}
		task_video_record_caltime();
		deg_Printf("video : fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
		return 0;
	}
	else if(cmd == CMD_COM_ERROR)// video record error
	{
		chBfileFlag = 0;
		SysCtrl.g_lock = 0; // clear g-sensor lock active flag
		//if(para == CMD_VIDEO_RECORD_START) // start fail
		{
			fdt = (FHANDLE *)para;
			fd[0] = (int)*fdt;
			fd[1] = (int)*(fdt+1);
			ret = 0;
			SysCtrl.g_lock = 0; // clear g-sensor lock active flag
			if(hal_sdGetCardState()==SDC_STATE_ERROR)
			{
				SysCtrl.sdcard = SDC_STAT_ERROR;
				XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
				deg_Printf("video : error\n");
			}
			else if(channel == VIDEO_CH_A)
			{
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_ON, 0));  // restart
				deg_Printf("[CMD_COM_ERROR]video : send event on\n\n\n");
				//sysLog("video : send event on");
			}
			deg_Printf("video : error fs size %d\n",fs_size(fd[0]));
			deg_Printf("video : fs before size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
			bsize = fs_size(fd[0]);
			if(close((FHANDLE)fd[0]) < 0)
				bsize = 0 ;
			if(bsize>0){
		#if AVI_TYPE_ODML
				name = tempFileName1;		    
				name = &tempFileName1[strlen(FILEDIR_VIDEOA)];
				if(channel == VIDEO_CH_A)
				{
					ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOA,name);	// add video a
					deg_Printf("%s%s\n",FILEDIR_VIDEOA,name);
				}
				else if(channel == VIDEO_CH_B)
				{
					ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOB,name);	// add video b
					deg_Printf("%s%s\n",FILEDIR_VIDEOB,name);
				}
		#endif
			}
				
			//close((FHANDLE)fd[0]);
		#if AVI_TYPE_ODML == 0		
			close((FHANDLE)fd[1]);
		    goto VIDEO_RECORD_ERROR_HANDLE;
		#else
		    deamon_fsSizeModify(-1,bsize);	
		#endif
		    return -1;
		}
	}
	else if(cmd == CMD_COM_CHECK) // check sdc sapce
	{
		if(para==0)
		{
		 #if (1 == UPDATE_SETTING_EN)
			ret = videoRecordSizePreSec(VIDEO_CH_A, get_info_value("filetime") * 60 /*configValue2Int(CONFIG_ID_LOOPTIME)*/);
		 #else
		 	ret = videoRecordSizePreSec(VIDEO_CH_A, configValue2Int(CONFIG_ID_LOOPTIME));
		 #endif
			para = ret;
		}
		ret = managerSpaceCheck(SysCtrl.avi_list,FILEDIR_VIDEO,para);//REMAIN_SAPCE_VIDEO);
		if(ret<0)
		{
			SysCtrl.sdcard = SDC_STAT_FULL;
			XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
			sysLog("CMD_COM_CHECK no space");
		}
		return ret;
	}
	//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
	return 0;

VIDEO_RECORD_ERROR_HANDLE:	
	XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
	deg_Printf("video : error.delete current file\n");
	close((FHANDLE)fd[0]);
#if AVI_TYPE_ODML == 0	
	close((FHANDLE)fd[1]);
#endif
    if(channel == VIDEO_CH_A)
    {
		memcpy(tempFileName1,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));
    }
	else if(channel == VIDEO_CH_B)
	{
		memcpy(tempFileName1,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
	}
    f_unlink(tempFileName1);  
#if AVI_TYPE_ODML == 0  
    if(channel == VIDEO_CH_A)
		f_unlink("VIDEOA.TMP");
	if(channel == VIDEO_CH_B)
	    f_unlink("VIDEOB.TMP");

#endif
	task_video_record_caltime();
	return ret;
}
int video_record_start(void)
{	
	int ret;
    
	if(SysCtrl.sdcard!=SDC_STAT_NORMAL)
	{
		XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
		return -1;
	}
	if(videoRecordGetStatus() == MEDIA_STAT_START)
		return;
    while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
	SysCtrl.t_rec = 0;
	msgDealByType(VID_EVENT_REC_START,winGetCurrent(),0,NULL);
	mediaVideoCtrl.stat = MEDIA_STAT_START;
    ret = videoRecordStart();
   // videoRecordFileStartB(); 
	deg_Printf("video record start.%d\n",ret);	
	
	return ret;
}
int task_record_stop(void)
{
	int ret;

//	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
	if(videoRecordGetStatus() != MEDIA_STAT_START)
		return;
	ret = videoRecordStop();
	XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_STOP,0));
	SysCtrl.t_rec = 0;
   // videoRecordFileStopB();
	SysCtrl.g_lock = 0; // clear g-sensor lock active flag
	deg_Printf("video record stop.%d,%d\n",ret,videoRecordGetStatus());
 //   task_video_record_caltime();
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
	
	return ret;
}
void task_video_record_caltime(void)
{
	INT32U size1,size2;

	
	size1 = videoRecordSizePreSec(VIDEO_CH_A,1);
	
	size2 = SysCtrl.sd_freesize;

    deg_Printf("rec time : %dkb/s,%dkb\n",size1,size2);
	SysCtrl.t_remain = size2/size1;
}

void task_video_record_caltime_rename(void)
{
	INT32U size1,size2;

	
	size1 = videoRecordSizePreSec(VIDEO_CH_A,1);
	
	size2 = deamon_fsFreeSize();

    deg_Printf("rec time : %dkb/s,%dkb\n",size1,size2);
	SysCtrl.t_remain = size2/size1;
}

void taskVideoRecTimeIncreace(void)
{
	//if(videoRecordGetStatus() == MEDIA_STAT_START)
	//	SysCtrl.t_rec++;
}
INT32S photoFd=-1;
char *photoname=NULL;
void videoCapturePhoto(void)
{
	
	if((videoRecordGetStatus() == MEDIA_STAT_START)&&(photoFd<0))
	{
		photoFd = task_image_createfile(VIDEO_CH_A,&photoname);
		if(photoFd>=0)
		{
			if(videoRecordTakePhoto(photoFd)<0)
			{
				close(photoFd);
				f_unlink(photoname);
				photoFd = -1;
				deg_Printf("recorder : tabke photo fail\n");
			}
			else
				deg_Printf("recorder : tabke photo start\n");
		}
		else if (photoFd == -2)
			videoRecordTakePhoto(photoFd);
	}
}


void taskVideoRecordeOpen(uint32 arg1)
{
	VIDEO_ARG_T arg;
	INT32S ret;
    deamon_usb_to_video_check_card();
	deg_Printf("video record task enter.\n");
    //sysLog("video record task enter.");
	//task_get_state(NULL,(INT8U *)&curstat);
	arg.quality = JPEG_Q_40;
	arg.fps = 30;
	arg.timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	arg.audio = configValue2Int(CONFIG_ID_AUDIOREC);
	arg.rectime = configValue2Int(CONFIG_ID_LOOPTIME);

#if UPDATE_SETTING_EN
	if(get_info_value("recycle")) //if(arg.rectime==0)
	{
		//arg.rectime = 10*60;  // 10 min
		arg.looprecord = 1; //0;
	}
	else
		arg.looprecord = 0; //1;

    arg.rectime = get_info_value("filetime") * 60;
#else
	arg.rectime = configValue2Int(CONFIG_ID_LOOPTIME);
	if(arg.rectime==0)
	{
		arg.rectime = 10*60;  // 10 min
		arg.looprecord = 0;
	}
	else
		arg.looprecord = 1;

#endif

	ret = configValue2Int(CONFIG_ID_RESOLUTION);
	arg.resolution.width = ret>>16;
	arg.resolution.height = ret&0xffff;	
    arg.callback = video_record_callback;
//	arg.fastwr = FILE_FAST_WRITE;
	arg.ftype = AVI_TYPE_ODML;
	arg.syncwr = FILE_SYNC_WRITE;
	
    videoRecordInit(&arg);	
#if USENSOR_AUDIO_ON == 0
		if (arg.audio)
			arg.audio = 0;
#endif
#if USENSOR_RESOLUTION == 1
	arg.resolution.width = 640;
	arg.resolution.height = 480;
#elif USENSOR_RESOLUTION == 2
	arg.resolution.width = 1280;
	arg.resolution.height = 720;
#elif USENSOR_RESOLUTION == 3
	arg.resolution.width = 1280;
	arg.resolution.height = 960;
#else
	arg.resolution.width = 640;
	arg.resolution.height = 480;
#endif
	videoRecordInitB(&arg);	
	task_video_record_caltime(); // re-cal
	
    //layout_video_record_init();
	dispLayerInit(DISP_LAYER_VIDEO); // preview sensor,必须先初始化再设置pip


    SysCtrl.crop_level = 0;
#if  HAL_CFG_LCD_CROP
    dispLayerSetFrontCrop_426_240(SysCtrl.crop_level);
#else
    dispLayerSetFrontCrop(SysCtrl.crop_level);
#endif
	if(!api_backrec_status())
		SysCtrl.pip_mode = DISP_PIP_FRONT;
	else
		api_backrec_relinking();
    dispLayerSetPIPMode(SysCtrl.pip_mode);
	//msgFuncRegister(videoRecordeMsgDeal);

	uiOpenWindow(&videoRecordWindow,0);
	photoFd=-1;
	photoname=NULL;
}

void taskVideoRecordeService(uint32 arg)
{
	if (videoRecordTakePhotoStatus() == MEDIA_STAT_ERROR)
	{
		if(SysCtrl.sdcard == SDC_STAT_FULL )
		{
			SysCtrl.sdcard = SDC_STAT_NORMAL;
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
			videoRecordSetPhotoStatus(MEDIA_STAT_STOP);
		}
	}
	if(photoFd>=0)
	{
		if(videoRecordTakePhotoStatus() == MEDIA_STAT_STOP)
		{
			deg_Printf("recorder: take photo [%s] sucess\n",photoname);
			deamon_fsSizeModify(-1,fs_size(photoFd));
	        close(photoFd);
	        managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&photoname[strlen(FILEDIR_PHOTO)]);
			photoFd = -1;
			deg_Printf("recorder : take photo stop.\n");
		}
	}
#if FILE_SYNC_DELETE > 0	
	f_unlink_ext_service(); // fs service
#endif	

#if UPDATE_SETTING_EN
    if((get_info_value("recycle")==0) && (videoRecordGetStatus() == MEDIA_STAT_START) && (deamon_fsFreeSize() < LOOP_REC_OFF_REMAIN_SPACE))
#else
	 if((videoRecordGetStatus() == MEDIA_STAT_START) && (deamon_fsFreeSize() < LOOP_REC_OFF_REMAIN_SPACE))
#endif
	{
        task_record_stop();
        task_video_record_caltime_rename();
        SysCtrl.sdcard = SDC_STAT_FULL;
        XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
    }

	videoRecordService(); // record service
	//videoRecordServiceB();  // record B service
}
void taskVideoRecordeClose(uint32 arg)
{
	task_record_stop();
	if(photoFd>=0)
	{
		if(videoRecordTakePhotoStatus() == MEDIA_STAT_STOP)
		{
			deamon_fsSizeModify(-1,fs_size(photoFd));
	        close(photoFd);
	        managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&photoname[strlen(FILEDIR_PHOTO)]);
			deg_Printf("recorder : take photo stop.\n");
		}
		else
		{
			deg_Printf("recorder : take photo unfinish.delete file:%s\n",photoname);
			close(photoFd);
			f_unlink(photoname);
		}
	}
#if FILE_SYNC_DELETE > 0	
	while(f_unlink_ext_service());
#endif	
	if(SysCtrl.sdcard == SDC_STAT_FULL)
	{
		SysCtrl.sdcard = SDC_STAT_NORMAL;
	}
	photoFd=-1;
	photoname=NULL;
	//if(hal_lcdGetShowingRotate()==LCD_ROTATE_0)
	//	hal_rtcCallBackRelease(rtcfd);
	dispLayerUninit(DISP_LAYER_VIDEO);  
	//winDestroy(&taskVideoRecorde.handle);
	deg_Printf("video record task exit.\n");
}
sysTask taskVideoRecorde=
{
	"Video Recorde",
	NULL,
	taskVideoRecordeOpen,
	taskVideoRecordeClose,
	taskVideoRecordeService,
};


