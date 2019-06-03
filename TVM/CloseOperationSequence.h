#pragma once
#include "SCSequence.h"

class CCloseOperationSequence:public CSCSequence
{
public:
	CCloseOperationSequence();
	virtual ~CCloseOperationSequence();
	virtual bool CanAcceptSCCommand(CSCCommand* command);
	virtual void OnSCCommandComplete(CSCCommand* command);
protected:
	virtual long ExecuteCommand();
};