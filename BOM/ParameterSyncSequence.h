#pragma once
#include "SCSequence.h"

class CParameterSyncSequence:public CSCSequence
{
public:
	CParameterSyncSequence();
	virtual ~CParameterSyncSequence();
	virtual bool CanAcceptSCCommand(CSCCommand* command);
	virtual void OnSCCommandComplete(CSCCommand* command);
protected:
	virtual long ExecuteCommand();
private:
};