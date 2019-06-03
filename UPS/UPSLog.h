#pragma once
#include "UPSExp.h"
#include "Log.h"

#define theUPS_LOG	CUPSLog::GetInstance()		// ups log ����Ψһ�ӿ�

/**
@brief UPS��־trace
*/
class UPS_API CUPSLog : public CLog
{
public:
	void WriteLogFile(CString cmdName, LPBYTE lpData, int iLen, SEND_MODE iMode);

	static CUPSLog&  GetInstance()
	{
		return UPSLogInstance;
	}

private:
	CUPSLog();
	virtual  ~CUPSLog();

	static  CUPSLog UPSLogInstance;
};
