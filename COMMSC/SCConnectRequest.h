#pragma once
#include "SCExp.h"
#include "SCCommand.h"

class SC_API CSCConnectRequest : public CSCCommand
{
public:
	CSCConnectRequest();
	~CSCConnectRequest();
	long IsValidCommand();

protected:
	long ExecuteCommand();

};