/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA LAYER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : USER_MEDIA.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  USER_MEDIA_H
    #define  USER_MEDIA_H

	
#include "../hal/inc/hal.h"
#include "../bwlib/bwlib.h"
#include "../xos/xos.h"

//typedef  FIL*        FHANDLE;
//typedef  FRESULT    __RES;

extern INT8U *jpegCache,*aviIdx1Cache,*aviIdx1CacheB, *audioCache,*thumbnailYBuffer,*thumbnailUVBuffer;

extern MSG_T audsTStack[],audsIStack[],audsBStack[],vidsIStack[],vidsBStack[],jpegIStack[],jpegBStack[];
/*******************************************************************************
* Function Name  : shareMemMalloc
* Description    : memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : INT32U size : need memory size 
* Output         : none
* Return         : 
*******************************************************************************/
void *shareMemMalloc(INT32U size);
/*******************************************************************************
* Function Name  : shareMemFree
* Description    : memory share for other application,memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : void *mm : memory addr
* Output         : none
* Return         : 
*******************************************************************************/
void shareMemFree(void *mm);
/*
extern int f_fastseek_init(FIL *fp);

extern int media_open(const char *name,INT32U op);


extern __RES media_close(FHANDLE fd);


extern __RES media_read(FHANDLE  fp,void *buff,UINT len,UINT * reslen);


extern __RES media_write(FHANDLE  fp,const void *buff,UINT len,UINT * reslen);


extern __RES media_lseek(FHANDLE fp,INT32S offset,INT32U point);


extern __RES media_merge(FHANDLE fp1,FHANDLE fp2);

extern INT32U media_fsize(FHANDLE fp);

extern INT32U media_free_size(void);
*/
#endif