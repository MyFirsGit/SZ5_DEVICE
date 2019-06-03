#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplayAddScreen::CStatusDisplayAddScreen()
{
}

CStatusDisplayAddScreen::~CStatusDisplayAddScreen()
{
}

long CStatusDisplayAddScreen::ExecuteCommand(void)
{
	long lret=1;
	lret=addScreen();
	this->OnComplete(lret);
	return lret;
}

long CStatusDisplayAddScreen::addScreen()
{
	long lret=1;
	try{
		int nSendMode = SEND_MODE_NETWORK;
		int nScreenType = 4;
		CString strCom;
		CString strBaud;
		CString strSocketIP = SCREEN_SOCKETIP;
		int nSocketPort = 5005;
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
		char *SocketIP = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,SocketIP,dwNum,0,NULL);
		if (AddScreen_Dynamic == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}
		lret = AddScreen_Dynamic(BX_6Q1, SCREEN_NO, SEND_MODE_NETWORK, SCREEN_WIDTH
			, SCREEN_HEIGHT, SCREEN_TYPE, 1
			, NULL, NULL
			, SocketIP, nSocketPort,0/*TCPģʽ*/, NULL, NULL, NULL
			, NULL, NULL, NULL, NULL
			, NULL);
		if(lret!=0){
			//throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,_T(__FILE__),__LINE__);
			return CStatusDisplayException::ERROR_StatusDisplay_RA_SCREENNO;
		}
	}
	catch(CSysException&e)
	{
	}
	return lret;
}