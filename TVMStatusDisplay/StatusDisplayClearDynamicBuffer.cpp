#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDisplayClearDynamicBuffer::CStatusDisplayClearDynamicBuffer(long lDYAradID)
{
	m_lDYAreaID = lDYAradID;
}

CStatusDisplayClearDynamicBuffer::~CStatusDisplayClearDynamicBuffer()
{
}

long CStatusDisplayClearDynamicBuffer::ExecuteCommand(void)
{
	long lret=1;
	lret=clearDynamicBuffer();
	this->OnComplete(lret);
	return lret;
}

long CStatusDisplayClearDynamicBuffer::clearDynamicBuffer()
{
	if (DeleteScreenDynamicArea == NULL)
	{
		return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
	}
	long lret = 0;
	try{
		lret= DeleteScreenDynamicArea(SCREEN_NO,m_lDYAreaID);
		if(lret!=0)
		{
			throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,
				_T(__FILE__),__LINE__);
		}else
		{
		}
	}
	catch(CSysException&e)
	{
	}

	return lret;
}