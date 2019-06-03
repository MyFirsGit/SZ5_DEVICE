#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplaySetDynamicBuffer::CStatusDisplaySetDynamicBuffer(CLedDispInfo& dispInfo)
{
	memmove(&m_dispInfo,&dispInfo,sizeof(CLedDispInfo));
}

CStatusDisplaySetDynamicBuffer::~CStatusDisplaySetDynamicBuffer()
{
}

long CStatusDisplaySetDynamicBuffer::ExecuteCommand(void)
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
long CStatusDisplaySetDynamicBuffer::setDispBuffer()
{
	long result = 0;
	try
	{
		if(NULL == g_hStatusDisplay){
			return CStatusDisplayException::ERROR_StatusDisplay_OPEN_COM;
		}
		TDISPINFOTEXT dispInfoText;
		memcpy(&dispInfoText,m_dispInfo.getDispTextCommandBytes(),sizeof(TDISPINFOTEXT));
		if (AddScreenDynamicAreaText == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}

		result = AddScreenDynamicAreaText(
			dispInfoText.wScreenNo, dispInfoText.wDYAreaID, dispInfoText.sText
			, dispInfoText.wShowSingle, dispInfoText.sFontName, dispInfoText.wFontSize
			, dispInfoText.wBold, dispInfoText.wFontColor, dispInfoText.wStunt
			, dispInfoText.wRunSpeed, dispInfoText.wShowTime
			);
		if(result!=0)
		{
			throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,
				_T(__FILE__),__LINE__);
		}else
		{
		}
	}
	catch (CSysException& e)
	{
	}

	return result;
}