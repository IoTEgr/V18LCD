#include "menuOption.h"


static void menuOptionCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	menuOptionObj* pmenuOption=(menuOptionObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pmenuOption->widget.win);
	selectInfor* infor;
	strInfor* stringInfor;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("menuOption win create\n");
			return;
		case MSG_WIN_PAINT:
			if(pmenuOption->select==true)
			{
				if(pmenuOption->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuOption->selectColor);
				if(pmenuOption->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuOption->selectImage,ALIGNMENT_CENTER,pWin->bgColor);
			}
			else
			{
				if(pmenuOption->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuOption->color);
				if(pmenuOption->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuOption->image,ALIGNMENT_CENTER,pWin->bgColor);
			}
			return;
		case MSG_WIN_CHANGE_RESID:
			if(!resIdIsImage(msg->parameter.v))
				winSetResid(pmenuOption->hStr,msg->parameter.v);
			return;
		case MSG_WIN_INVALID_RESID: //2022.03031
			winSetResid(pmenuOption->hStr,INVALID_RES_ID);
			return;
		case MSG_WIN_CHANGE_STRINFOR:
		#if 0
			winSendMsg(pmenuOption->hStr,msg);
		#else  //2022.03.31
			stringInfor=(strInfor*)(msg->parameter.p);
			pmenuOption->inforUnselect.fontColor=stringInfor->fontColor;
			pmenuOption->inforUnselect.strAlign=stringInfor->strAlign;
			pmenuOption->inforUnselect.font=stringInfor->font;
			if(pmenuOption->initStrInfor==false)
			{
			 	winSendMsg(pmenuOption->hStr,msg);
				pmenuOption->initStrInfor=true;
			}
		#endif
			return;
		case MSG_WIN_SELECT_STRINFOR: //2022.03.31
			pmenuOption->useInforSelect=true;
			stringInfor=(strInfor*)(msg->parameter.p);
			pmenuOption->inforSelect.fontColor=stringInfor->fontColor;
			pmenuOption->inforSelect.strAlign=stringInfor->strAlign;
			pmenuOption->inforSelect.font=stringInfor->font;
			//winSendMsg(pmenuItem->hStr,msg);
			return;
		case MSG_WIN_SELECT_INFOR:
			infor=(selectInfor*)(msg->parameter.p);
			pmenuOption->selectColor=infor->color;
			pmenuOption->selectImage=infor->image;
			return;
		case MSG_WIN_UNSELECT_INFOR:
			infor=(selectInfor*)(msg->parameter.p);
			pmenuOption->color=infor->color;
			pmenuOption->image=infor->image;
			return;
		case MSG_WIN_UNSELECT:
			if(pmenuOption->select==false)
				return;
			//2022.03.31
			winSetStrInfor(pmenuOption->hStr,pmenuOption->inforUnselect.font,pmenuOption->inforUnselect.strAlign,pmenuOption->inforUnselect.fontColor);
			pmenuOption->select=false;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			return;
		case MSG_WIN_SELECT:
			if(pmenuOption->select==true)
				return;
			//2022.03.31
			if(pmenuOption->useInforSelect)
				winSetStrInfor(pmenuOption->hStr,pmenuOption->inforSelect.font,pmenuOption->inforSelect.strAlign,pmenuOption->inforSelect.fontColor);
			else
				winSetStrInfor(pmenuOption->hStr,pmenuOption->inforUnselect.font,pmenuOption->inforUnselect.strAlign,pmenuOption->inforUnselect.fontColor);
			pmenuOption->select=true;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			return;
		case MSG_WIN_VISIBLE_SET:
			if(msg->parameter.v)
				winSetVisible(pmenuOption->hStr,true);
			else
				winSetVisible(pmenuOption->hStr,false);
			break;
		case MSG_WIN_TOUCH:
			break;
		case MSG_WIN_TOUCH_GET_INFOR:  //2022.03.31
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle menuOptionCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hmenuOption;
	menuOptionObj* pmenuOption;
	hmenuOption=winCreate(x0,y0,width,height,parent,menuOptionCB,sizeof(menuOptionObj),WIN_WIDGET|style);
	widgetSetId(hmenuOption,id);
	if(hmenuOption!=INVALID_HANDLE)
	{
		pmenuOption=(menuOptionObj*)uiHandleToPtr(hmenuOption);
		pmenuOption->color=INVALID_COLOR;
		pmenuOption->selectColor=INVALID_COLOR;
		pmenuOption->image=INVALID_RES_ID;
		pmenuOption->selectImage=INVALID_RES_ID;
		pmenuOption->hStr=INVALID_HANDLE;
		pmenuOption->useInforSelect=false; //2022.03.31
		pmenuOption->select=false;
		pmenuOption->initStrInfor=false; //2022.03.31
	}
	return hmenuOption;
}

winHandle itemCreatemenuOption(int16 x0,int16 y0,uint16 width,uint16 height)
{
	winHandle hmenuOption;
	menuOptionObj* pmenuOption;
	hmenuOption=menuOptionCreate(x0,y0,width,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	if(hmenuOption!=INVALID_HANDLE)
	{
		pmenuOption=(menuOptionObj*)uiHandleToPtr(hmenuOption);
		winSetbgColor(hmenuOption, INVALID_COLOR);
		pmenuOption->hStr=stringIconCreate(x0,y0,width,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	}
	return hmenuOption;
}




