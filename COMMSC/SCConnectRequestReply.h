#pragma once
#include "SCExp.h"
#include "scresponsecommand.h"

class SC_API CSCConnectRequestReply : public CSCResponseCommand
{
public:
	CSCConnectRequestReply();
	~CSCConnectRequestReply();
	long ExecuteCommand();
protected:
	long IsValidCommand();

};