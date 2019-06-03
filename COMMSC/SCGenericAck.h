#pragma once
#include "SCExp.h"
#include "scresponsecommand.h"

class SC_API CSCGenericAck : public CSCResponseCommand
{
public:
	CSCGenericAck();
	~CSCGenericAck();
	long IsValidCommand();
protected:
	long ExecuteCommand();
private:

};