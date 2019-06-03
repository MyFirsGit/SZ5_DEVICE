#pragma once
#include "stdafx.h"
#include <afxsock.h>
#include "scexp.h"
#include "delegate.h"

class SC_API CSCSocket : public CSocket
{

public:
	CSCSocket();
	virtual ~CSCSocket();

	delegate<void(int)> Receiving;
	virtual void OnReceive(int nErrorCode);
	delegate<void (bool,VARIABLE_DATA&)> Received;

	virtual void OnClose(int nErrorCode);
	delegate<void(int)> Closed;

	delegate<void(const unsigned char*, int)> Sending;
	long Send(const unsigned char* lpBuf, int nBufLen);
	delegate<void(const unsigned char*, int)> Sended;
private:
	bool TryReceiveOnePackage(VARIABLE_DATA& buffer);

};