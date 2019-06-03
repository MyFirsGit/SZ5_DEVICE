#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplayDeleteScreen::CStatusDisplayDeleteScreen()
{
}

CStatusDisplayDeleteScreen::~CStatusDisplayDeleteScreen()
{
}

long CStatusDisplayDeleteScreen::ExecuteCommand(void)
{
	long lret=1;
	lret=deleteScreen();
	this->OnComplete(lret);
	return lret;
}

long CStatusDisplayDeleteScreen::deleteScreen()
{
	long lret=1;

	try{
		if (DeleteScreen_Dynamic == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}

		lret = DeleteScreen_Dynamic(SCREEN_NO);
		if(lret!=0){
			//throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,_T(__FILE__),__LINE__);
			return CStatusDisplayException::ERROR_StatusDisplay_NOFIND_SCREENNO;
		}
	}
	catch(CSysException& e)
	{
	}
	return lret;
}