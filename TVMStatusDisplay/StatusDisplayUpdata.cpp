#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplayUpdate::CStatusDisplayUpdate()
{
}

CStatusDisplayUpdate::~CStatusDisplayUpdate()
{
}

long CStatusDisplayUpdate::ExecuteCommand(void)
{
	long lret=1;
	lret=updateDisp();
	this->OnComplete(lret);
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送数据

@param      无

@retval     long 返回结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayUpdate::updateDisp()
{
	long result = 0;
	try{
		if(NULL == g_hStatusDisplay){
			return CStatusDisplayException::ERROR_StatusDisplay_OPEN_COM;
		}
		if (SendDynamicAreasInfoCommand == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}
		int nDelAllDYArea = 1;
		char* dlist = "";
		result = SendDynamicAreasInfoCommand(SCREEN_NO,nDelAllDYArea,dlist);
		if(result!=0)
		{
			throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,
				_T(__FILE__),__LINE__);
		}else
		{
		}
	}
	catch(CSysException& e)
	{
	}
	return result;
}