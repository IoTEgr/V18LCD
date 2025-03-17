/****************************************************************************
       ***             ***                      MAXLIB-GRAPHC                  
      ** **           ** **                                                    
     **   **         **   **            THE MAXLIB FOR IMAGE SHOW PROCESS      
    **     **       **     **                                                  
   **       **     **       **              MAX ROURCE ICON MANAGEMENT         
  **         **   **         **                                                
 **           ** **           **              (C) COPYRIGHT 2016 MAX           
**             ***             **                                              
                                                                               
* File Name   : user_icon.c                                                    
* Author      : Mark.Douglas                                                   
* Version     : V0100                                                          
* Date        : 05/25/2016                                                     
* Description : This file for maxlib resource icon managemant                  
*                                                                              
* History     :                                                                
* 2016-05-25  :                                                                
*      <1>.This is created by mark,set version as v0103.                       
*      <2>.Add basic functions.                                                
******************************************************************************/

#ifndef USER_ICON_H 
   #define USER_ICON_H



#define  TRANSPARENT_COLOR    112

enum
{
   R_ID_ICON_CLOCKPOWEROFFC=R_ID_TYPE_ICON, 
   R_ID_ICON_CLOCKPOWEROFFE,
   R_ID_ICON_AUBATTERY0,
   R_ID_ICON_AUBATTERY1,
   R_ID_ICON_AUBATTERY2,
   R_ID_ICON_AUBATTERY3,
   R_ID_ICON_AUBATTERY4,
   R_ID_ICON_AUBATTERY5,
   R_ID_ICON_AUDIORECORDSTART,
   R_ID_ICON_AUDIORECORDSTOP,
   R_ID_ICON_CLOCKRECT1,
   R_ID_ICON_CLOCK_BATTERY0,
   R_ID_ICON_CLOCK_BATTERY1,
   R_ID_ICON_CLOCK_BATTERY2,
   R_ID_ICON_CLOCK_BATTERY3,
   R_ID_ICON_CLOCK_BATTERY4,
   R_ID_ICON_CLOCK_BATTERY5,
   R_ID_ICON_DATE_B0,
   R_ID_ICON_DATE_B1,
   R_ID_ICON_DATE_B2,
   R_ID_ICON_DATE_B3,
   R_ID_ICON_DATE_B4,
   R_ID_ICON_DATE_B5,
   R_ID_ICON_DATE_B6,
   R_ID_ICON_DATE_B7,
   R_ID_ICON_DATE_B8,
   R_ID_ICON_DATE_B9,
   R_ID_ICON_DATE_BD,
   R_ID_ICON_DATE_L0,
   R_ID_ICON_DATE_L1,
   R_ID_ICON_DATE_L2,
   R_ID_ICON_DATE_L3,
   R_ID_ICON_DATE_L4,
   R_ID_ICON_DATE_L5,
   R_ID_ICON_DATE_L6,
   R_ID_ICON_DATE_L7,
   R_ID_ICON_DATE_L8,
   R_ID_ICON_DATE_L9,
   R_ID_ICON_DATE_LD,
   R_ID_ICON_MENUAUDIO,
   R_ID_ICON_MENUBATTERY,
   R_ID_ICON_MENUCLOCK,
   R_ID_ICON_MENUDELALL,
   R_ID_ICON_MENUDELONE,
   R_ID_ICON_MENUEV,
   R_ID_ICON_MENUFORMAT,
   R_ID_ICON_MENUHZ,
   R_ID_ICON_MENUIMAGEQ,
   R_ID_ICON_MENULANGUAGE,
   R_ID_ICON_MENULIGHTNORMAL,
   R_ID_ICON_MENULOOPRECORD,
   R_ID_ICON_MENUMOTION,
   R_ID_ICON_MENUPOWEROFF,
   R_ID_ICON_MENURESET,
   R_ID_ICON_MENURESOLUTION,
   R_ID_ICON_MENUSCRENNOFF,
   R_ID_ICON_MENUSTRAMP,
   R_ID_ICON_MENUTV,
   R_ID_ICON_MENUVERSION,
   R_ID_ICON_MENUVOLUME,
   R_ID_ICON_MENUVOLUME2,
   R_ID_ICON_MTBATTERY0,
   R_ID_ICON_MTBATTERY1,
   R_ID_ICON_MTBATTERY2,
   R_ID_ICON_MTBATTERY3,
   R_ID_ICON_MTBATTERY4,
   R_ID_ICON_MTBATTERY5,
   R_ID_ICON_MTMENU,
   R_ID_ICON_MTMICOFF,
   R_ID_ICON_MTMICON,
   R_ID_ICON_MTMORE,
   R_ID_ICON_MTMOTION,
   R_ID_ICON_MTOFF,
   R_ID_ICON_MTON,
   R_ID_ICON_MTPAUSE,
   R_ID_ICON_MTPHOTO,
   R_ID_ICON_MTPLAY,
   R_ID_ICON_MTPLAY2,
   R_ID_ICON_MTRECORD,
   R_ID_ICON_MTSDCNORMAL,
   R_ID_ICON_MTSDCNULL,
   R_ID_ICON_PICTURE,
   R_ID_ICON_RECASTATE,
   R_ID_ICON_RECSTATE,
   R_ID_ICON_WEEK_EN_FRI,
   R_ID_ICON_WEEK_EN_MON,
   R_ID_ICON_WEEK_EN_SAT,
   R_ID_ICON_WEEK_EN_SUN,
   R_ID_ICON_WEEK_EN_THUR,
   R_ID_ICON_WEEK_EN_TUE,
   R_ID_ICON_WEEK_EN_WED,
   R_ID_ICON_WEEK_JA_FRI,
   R_ID_ICON_WEEK_JA_MON,
   R_ID_ICON_WEEK_JA_SAT,
   R_ID_ICON_WEEK_JA_SUN,
   R_ID_ICON_WEEK_JA_THUR,
   R_ID_ICON_WEEK_JA_TUE,
   R_ID_ICON_WEEK_JA_WED,
   R_ID_ICON_WEEK_KO_FRI,
   R_ID_ICON_WEEK_KO_MON,
   R_ID_ICON_WEEK_KO_SAT,
   R_ID_ICON_WEEK_KO_SUN,
   R_ID_ICON_WEEK_KO_THUR,
   R_ID_ICON_WEEK_KO_TUE,
   R_ID_ICON_WEEK_KO_WED,
   R_ID_ICON_WEEK_SC_FRI,
   R_ID_ICON_WEEK_SC_MON,
   R_ID_ICON_WEEK_SC_SAT,
   R_ID_ICON_WEEK_SC_SUN,
   R_ID_ICON_WEEK_SC_THUR,
   R_ID_ICON_WEEK_SC_TUE,
   R_ID_ICON_WEEK_SC_WED,
   R_ID_ICON_MIC,
   R_ID_ICON_NAME0_P,
   R_ID_ICON_NAME1_P,
   R_ID_ICON_NAME2_P,
   R_ID_ICON_NAME3_P,
   R_ID_ICON_NAME4_P,
   R_ID_ICON_NAME5_P,
   R_ID_ICON_NAME6_P,
   R_ID_ICON_NAME7_P,
   R_ID_ICON_NAME8_P,
   R_ID_ICON_NAME9_P,
   R_ID_ICON_TIME0_P,
   R_ID_ICON_TIME1_P,
   R_ID_ICON_TIME2_P,
   R_ID_ICON_TIME3_P,
   R_ID_ICON_TIME4_P,
   R_ID_ICON_TIME5_P,
   R_ID_ICON_TIME6_P,
   R_ID_ICON_TIME7_P,
   R_ID_ICON_TIME8_P,
   R_ID_ICON_TIME9_P,
   R_ID_ICON_TIME_P,
   R_ID_ICON_WAV,

  R_ICON_MAX
};
extern R_ICON_T User_Icon_Table[];




#endif

