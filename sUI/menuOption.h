#ifndef __MENU_OPTION_H
#define __MENU_OPTION_H
#include "widget.h"

typedef struct
{
	widgetObj widget;
	resID  image;
	resID  selectImage;
	uiColor color;
	uiColor selectColor;
	winHandle hStr;
	strInfor inforSelect; //2022.03.31
	strInfor inforUnselect;//2022.03.31
	bool useInforSelect;//2022.03.31
	bool select;
	//bool  visible;
	bool  initStrInfor;//2022.03.31
}menuOptionObj;

winHandle menuOptionCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle itemCreatemenuOption(int16 x0,int16 y0,uint16 width,uint16 height);






#endif
