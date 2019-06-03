#pragma once
#include "SCExp.h"
#include "SCCommand.h"

class SC_API CSCDataMsg : public CSCCommand
{
public:
	CSCDataMsg();
	~CSCDataMsg();
	long ExecuteCommand();


protected:
	long IsValidCommand();
};