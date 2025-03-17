#include"uiWin.h"

enum
{
	AUDIO_REC_SELECT_ID=0,

};


static widgetCreateInfor audioPlayWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
	createItemManage(AUDIO_REC_SELECT_ID,Rx(0),Ry(0), Rw(320),Rh(240),INVALID_COLOR),
	widgetEnd(),
};



