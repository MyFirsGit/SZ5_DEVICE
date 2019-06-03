#pragma  once
#include "stdafx.h"
#include "scapi.h"
#include "SCExp.h"

class SC_API CHeaderManager
{
private:
	static NETWORK_TRANS_ID current;
	static WORD msgNo;
public:
	static NETWORK_TRANS_ID AquireNewID(TRANSFER_COMMAND_CODE commandID);
	static CMD_HEADER AquireHeader(TRANSFER_DATA_TYPE msgType,TRANSFER_COMMAND_CODE commandID);
	static NETWORK_TRANS_ID GetCurrentID();
};