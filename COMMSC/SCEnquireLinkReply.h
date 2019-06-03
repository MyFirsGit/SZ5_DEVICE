#pragma once
#include "SCExp.h"
#include "scresponsecommand.h"

class SC_API CSCEnquireLinkReply : public CSCResponseCommand
{
public:
	CSCEnquireLinkReply();
	~CSCEnquireLinkReply();
	long IsValidCommand();
protected:
	long ExecuteCommand();

};