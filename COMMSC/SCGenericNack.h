#pragma once
#include "SCExp.h"
#include "scresponsecommand.h"

class SC_API CSCGenericNack : public CSCResponseCommand
{
public:
	CSCGenericNack();
	~CSCGenericNack();
	void SetNACKCode(USHORT nackCode);
	long IsValidCommand();
protected:
	long ExecuteCommand();
private:
	USHORT nackCode;
};