#include "menuItem.h"


static void menuItemCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	menuItemObj* pmenuItem=(menuItemObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pmenuItem->widget.win);
	selectInfor* infor;
	strInfor* stringInfor;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("menuItem win create\n");
			return;
		case MSG_WIN_PAINT:
			if(pmenuItem->select==true)
			{
				if(pmenuItem->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuItem->selectColor);
				if(pmenuItem->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuItem->selectImage,ALIGNMENT_CENTER,pWin->bgColor);
			}
			else
			{
				if(pmenuItem->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuItem->color);
				if(pmenuItem->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuItem->image,ALIGNMENT_CENTER,pWin->bgColor);
			}
			return;
		case MSG_WIN_INVALID_RESID:
			winSetResid(pmenuItem->hImage,INVALID_RES_ID);
			winSetResid(pmenuItem->hStr,INVALID_RES_ID);
			return;
		case MSG_WIN_CHANGE_RESID:
			if(resIdIsImage(msg->parameter.v))
				winSetResid(pmenuItem->hImage,msg->parameter.v);
			else
				winSetResid(pmenuItem->hStr,msg->parameter.v);
			return;
	#if 0
		case MSG_WIN_CHANGE_STRINFOR:
			winSendMsg(pmenuItem->hStr,msg);
			return;
	#else
		case MSG_WIN_CHANGE_STRINFOR:  //2022.03.31
			stringInfor=(strInfor*)(msg->parameter.p);
			pmenuItem->inforUnselect.fontColor=stringInfor->fontColor;
			pmenuItem->inforUnselect.strAlign=stringInfor->strAlign;
			pmenuItem->inforUnselect.font=stringInfor->font;
			if(pmenuItem->initStrInfor==false)
			{
			 	winSendMsg(pmenuItem->hStr,msg);
				pmenuItem->initStrInfor=true;
			}
			return;
	#endif

		case MSG_WIN_SELECT_STRINFOR:
			pmenuItem->useInforSelect=true;
			stringInfor=(strInfor*)(msg->parameter.p);
			pmenuItem->inforSelect.fontColor=stringInfor->fontColor;
			pmenuItem->inforSelect.strAlign=stringInfor->strAlign;
			pmenuItem->inforSelect.font=stringInfor->font;
			//winSendMsg(pmenuItem->hStr,msg);
			return;
	
		case MSG_WIN_SELECT_INFOR:  //2022.03.31
			infor=(selectInfor*)(msg->parameter.p);
			pmenuItem->selectColor=infor->color;
			pmenuItem->selectImage=infor->image;
			return;
	
		case MSG_WIN_UNSELECT_INFOR:
			infor=(selectInfor*)(msg->parameter.p);
			pmenuItem->color=infor->color;
			pmenuItem->image=infor->image;
			return;
		case MSG_WIN_UNSELECT:
			if(pmenuItem->select==false)
				return;
			winSetStrInfor(pmenuItem->hStr,pmenuItem->inforUnselect.font,pmenuItem->inforUnselect.strAlign,pmenuItem->inforUnselect.fontColor);//2022.03.31
			pmenuItem->select=false;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			return;
		case MSG_WIN_SELECT:
			if(pmenuItem->select==true)
				return;
			
		   if(pmenuItem->useInforSelect)
				winSetStrInfor(pmenuItem->hStr,pmenuItem->inforSelect.font,pmenuItem->inforSelect.strAlign,pmenuItem->inforSelect.fontColor);//2022.03.31
			else
				winSetStrInfor(pmenuItem->hStr,pmenuItem->inforUnselect.font,pmenuItem->inforUnselect.strAlign,pmenuItem->inforUnselect.fontColor);
			pmenuItem->select=true;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			return;
		case MSG_WIN_VISIBLE_SET:
			if(msg->parameter.v)
			{
				winSetVisible(pmenuItem->hImage,true);
				winSetVisible(pmenuItem->hStr,true);
			}
			else
			{
				winSetVisible(pmenuItem->hImage,false);
				winSetVisible(pmenuItem->hStr,false);
			}
			break;
		case MSG_WIN_TOUCH:
			break;

		case MSG_WIN_TOUCH_GET_INFOR: // 2022.03.31
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle menuItemCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hmenuItem;
	menuItemObj* pmenuItem;
	hmenuItem=winCreate(x0,y0,width,height,parent,menuItemCB,sizeof(menuItemObj),WIN_WIDGET|style);
	widgetSetId(hmenuItem,id);
	if(hmenuItem!=INVALID_HANDLE)
	{
		pmenuItem=(menuItemObj*)uiHandleToPtr(hmenuItem);
		pmenuItem->color=INVALID_COLOR;
		pmenuItem->selectColor=INVALID_COLOR;
		pmenuItem->image=INVALID_RES_ID;
		pmenuItem->selectImage=INVALID_RES_ID;
		pmenuItem->hImage=INVALID_HANDLE;
		pmenuItem->hStr=INVALID_HANDLE;
		pmenuItem->useInforSelect=false;//2022.03.31
		pmenuItem->select=false;
		pmenuItem->initStrInfor=false; //2022.03.31
	}
	return hmenuItem;
}

winHandle itemCreateMenuItem(int16 x0,int16 y0,uint16 width,uint16 height)
{
	winHandle hmenuItem;
	menuItemObj* pmenuItem;
	hmenuItem=menuItemCreate(x0,y0,width,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	if(hmenuItem!=INVALID_HANDLE)
	{
		pmenuItem=(menuItemObj*)uiHandleToPtr(hmenuItem);
		winSetbgColor(hmenuItem, INVALID_COLOR);
		pmenuItem->hImage=imageIconCreate(x0,y0,height,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
		pmenuItem->hStr=stringIconCreate(x0+height,y0,width-height,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	}
	return hmenuItem;
}




