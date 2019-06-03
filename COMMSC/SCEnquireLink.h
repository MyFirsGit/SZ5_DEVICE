#pragma once
#include "SCExp.h"
#include "SCCommand.h"

class SC_API CSCEnquireLink : public CSCCommand
{
public:
	CSCEnquireLink();
	~CSCEnquireLink();
	long IsValidCommand();
protected:
	long ExecuteCommand();
	
};