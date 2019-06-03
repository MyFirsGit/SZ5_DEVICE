#pragma once
#include "Command.h"
#include "sccommand.h"
#include "SCExp.h"

class SC_API CSCSequence:public CCommand
{
public:
	CSCSequence();
	virtual ~CSCSequence();
	//virtual long Start();
	virtual bool CanAcceptSCCommand(CSCCommand* command);
	virtual void OnSCCommandComplete(CSCCommand* command);
};