/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         TIMER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_timer.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"


#define  R_TIMER_CTRL0       TMR0CON
#define  R_TIMER_PLOAD0      TMR0PR
#define  R_TIMER_COUNTER0    TMR0CNT
#define  R_TIMER_STATUS0     TMR0CPND
#define  R_TIMER_CTRL1       TMR1CON
#define  R_TIMER_PLOAD1      TMR1PR
#define  R_TIMER_COUNTER1    TMR1CNT
#define  R_TIMER_STATUS1     TMR1CPND
#define  R_TIMER_CTRL2       TMR2CON
#define  R_TIMER_PLOAD2      TMR2PR
#define  R_TIMER_COUNTER2    TMR2CNT
#define  R_TIMER_STATUS2     TMR2CPND
#define  R_TIMER_CTRL3       TMR3CON
#define  R_TIMER_PLOAD3      TMR3PR
#define  R_TIMER_COUNTER3    TMR3CNT
#define  R_TIMER_STATUS3     TMR3CPND

#define  R_TICK_CTRL         TTCTL
#define  R_TICK_COUNTER      TTCNT
#define  R_TICK_PRLOAD       TTPR

void (*timerISR[TIMER_MAX])(void);
/*******************************************************************************
* Function Name  : ax32xx_timer0IRQHanlder
* Description    : timer0 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer0IRQHandler(void)
{
	R_TIMER_STATUS0|= BIT(5);	
	if(timerISR[TIMER0])
		timerISR[TIMER0]();
}
/*******************************************************************************
* Function Name  : ax32xx_timer1IRQHanlder
* Description    : timer1 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer1IRQHandler(void)
{
	R_TIMER_STATUS1|= BIT(5);	
	if(timerISR[TIMER1])
		timerISR[TIMER1]();
}
/*******************************************************************************
* Function Name  : ax32xx_timer2IRQHanlder
* Description    : timer2 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer2IRQHandler(void)
{
	R_TIMER_STATUS2|= BIT(5);	
	if(timerISR[TIMER2])
		timerISR[TIMER2]();
}
/*******************************************************************************
* Function Name  : ax32xx_timer3IRQHanlder
* Description    : timer3 irq handler
* Input          : 

* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer3IRQHandler(void)
{
	R_TIMER_STATUS3|= BIT(5);	
	if(timerISR[TIMER3])
		timerISR[TIMER3]();
}
/*******************************************************************************
* Function Name  : ax32xx_timerISRRegister
* Description    : timer isr register
* Input          :  u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                       void (*isr)(void) : callback function
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerISRRegister(u8 timer,void (*isr)(void))
{
    if(timer>=TIMER_MAX)
		return -1;
	timerISR[timer] = isr;

	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerStart
* Description    : timer start
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                      u32 frq  : frq .>20hz
                      void (*isr)(void) : callback function
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerStart(u8 timer,u32 frq,void (*isr)(void))
{
	
    if((timer>=TIMER_MAX))// || (frq<20))
		return -1;
    u32 clkdiv,preload;


    if(frq>=750000) // 750K
		clkdiv = 3;// 12M
	else
		clkdiv = 7;// 750K
    preload = (((APB_CLK/frq)/(1<<clkdiv))-1);
	if(preload>0xffff)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD0 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
	          R_TIMER_COUNTER0 = 0x0000;
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD1 =  preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);//
	          R_TIMER_COUNTER1 = 0x0000;
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD2 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);
	          R_TIMER_COUNTER2 = 0x0000;
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD3 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);
	          R_TIMER_COUNTER3 = 0x0000;
			  break;
		default : return -1;break;
	}
	timerISR[timer] = isr;
		
	ax32xx_intEnable(IRQ_TIMER0+timer,1);
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timeStop
* Description    : timer stop
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerStop(u8 timer)
{
    if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  &= ~(BIT(3)|BIT(0));
			  break;
		default : return -1;break;		
	}
	ax32xx_intEnable(IRQ_TIMER0+timer,0);	
    timerISR[timer] = NULL;
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerEnable
* Description    : timer enable
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerEnable(u8 timer)
{
    if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  |= (BIT(3)|BIT(0));
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  |= (BIT(3)|BIT(0));
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  |= (BIT(3)|BIT(0));
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  |= (BIT(3)|BIT(0));
			  break;
		default : return -1;break;		
	}
	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_timerDisable
* Description    : timer disable
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerDisable(u8 timer)
{
    if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  &= ~(BIT(3)|BIT(0));
			  break;
		default : return -1;break;		
	}

	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_timerTickStart
* Description    : tick timer start
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerTickStart(void)
{
	R_TICK_CTRL = 0;
	R_TICK_PRLOAD  = 0xffffffff;
	R_TICK_COUNTER = 0;
	
	R_TICK_CTRL = (1<<6)|
			(0<<5)|
			(0<<4)|
			(2<<0);
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerTickStop
* Description    : tick timer stop
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerTickStop(void)
{
	R_TICK_CTRL = (1<<6);

	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerTickCount
* Description    : tick timer counter get
* Input          : 
* Output         : None
* Return         : count
*******************************************************************************/
u32 ax32xx_timerTickCount(void)
{
	return R_TICK_COUNTER;
}

