#include "stdafx.h"
#include "TOKENControl.h"

CTokenControl::CTokenControl() : CControl()
{
}

CTokenControl::~CTokenControl()
{
}



long CTokenControl::DoCommand(CTOKENCommands *pRequestItem, long lExecuteMode)
{
	return CControl::DoCommand(pRequestItem, lExecuteMode);
}

