#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplayDeleteDArea::CStatusDisplayDeleteDArea()
{
}

CStatusDisplayDeleteDArea::~CStatusDisplayDeleteDArea()
{
}

long CStatusDisplayDeleteDArea::ExecuteCommand(void)
{
	long lret=1;
	lret=deleteDynamicArea();
	this->OnComplete(lret);
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     long ���ؽ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayDeleteDArea::deleteDynamicArea()
{
	long result = 0;
	try{
		//if(NULL == g_hStatusDisplay){
		//	return CStatusDisplayException::ERROR_StatusDisplay_OPEN_COM;
		//}
		//if (SendDeleteDynamicAreasCommand == NULL)
		//{
		//	return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		//}
		int nDelAllDYArea = 1;
		char* dlist = "";
		result = SendDeleteDynamicAreasCommand(SCREEN_NO,nDelAllDYArea,dlist);
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