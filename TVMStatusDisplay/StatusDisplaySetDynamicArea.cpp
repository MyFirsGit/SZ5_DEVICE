#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplaySetDynamicArea::CStatusDisplaySetDynamicArea(CLedDispInfo& dispInfo)
{
	memmove(&m_dispInfo,&dispInfo,sizeof(CLedDispInfo));
}

CStatusDisplaySetDynamicArea::~CStatusDisplaySetDynamicArea()
{
}

long CStatusDisplaySetDynamicArea::ExecuteCommand(void)
{
	long lret=1;
	lret=setDispBuffer();
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
long CStatusDisplaySetDynamicArea::setDispBuffer()
{
	long result = 0;
	try{
		TDISPINFO dispInfo;
		memcpy(&dispInfo,m_dispInfo.getDispCommandBytes(),sizeof(TDISPINFO));
		if (AddScreenDynamicArea == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}
		result = AddScreenDynamicArea(
			SCREEN_NO, dispInfo.wDYAreaID, dispInfo.wRunMode, dispInfo.wTimeOut
			, dispInfo.wAllProRelate, dispInfo.sProRelateList, dispInfo.wPlayPriority
			, dispInfo.wAreaX, dispInfo.wAreaY, dispInfo.wAreaWidth, dispInfo.wAreaHeight
			, dispInfo.wAreaFMode, dispInfo.wAreaFLine, dispInfo.wAreaFColor
			, dispInfo.wAreaFStunt, dispInfo.wAreaFRunSpeed, dispInfo.wAreaFMoveStep
			);
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