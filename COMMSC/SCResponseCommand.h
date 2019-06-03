#pragma once
#include "SCCommand.h"
class SC_API CSCResponseCommand : public CSCCommand
{
public:
	CSCResponseCommand();
	~CSCResponseCommand();

	void SetResponseNTID(NETWORK_TRANS_ID& ntid);
	NETWORK_TRANS_ID& GetResponseNTID();

	void  SerializeBody(VARIABLE_DATA& buffer);
	void DeserializeBody(LPBYTE bodyData,UINT length);

protected:
	NETWORK_TRANS_ID responseNTID;
};
