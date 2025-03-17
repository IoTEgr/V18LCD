#include"uiWin.h"

enum
{
	DATETIME_SELECT_ID=0,
};
static widgetCreateInfor dateTimeWin[] =
{
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(142),R_COLOR_GRAY2,WIN_ABS_POS),
	createItemManage(DATETIME_SELECT_ID,Rx(0),Ry(0), Rw(180),Rh(142),INVALID_COLOR),
	widgetEnd(),
};



