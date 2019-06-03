#pragma once
#include "SCSequence.h"
#include <vector>

class CStartOperationSequence:public CSCSequence
{
public:
	CStartOperationSequence();
	virtual ~CStartOperationSequence();
	virtual bool CanAcceptSCCommand(CSCCommand* command);
	virtual void OnSCCommandComplete(CSCCommand* command);
protected:
	vector<long> msgSequences;
	int currentMsgIndex;
	virtual long ExecuteCommand();
};