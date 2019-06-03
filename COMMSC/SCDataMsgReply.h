#pragma once
#include "SCExp.h"
#include "scresponsecommand.h"

class SC_API CSCDataMsgReply : public CSCResponseCommand
{
public:
	CSCDataMsgReply();
	~CSCDataMsgReply();
	long IsValidCommand();
	

protected:
	long ExecuteCommand();	
};
