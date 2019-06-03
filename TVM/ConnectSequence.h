#pragma once
#include "SCSequence.h"
#include "parametersyncsequence.h"

class CConnectSequence:public CSCSequence
{
public:
	CConnectSequence();
	virtual ~CConnectSequence();
	virtual bool CanAcceptSCCommand(CSCCommand* command);
	virtual void OnSCCommandComplete(CSCCommand* command);
protected:
	virtual long ExecuteCommand();
private:
	CParameterSyncSequence* parameterSyncSequence;
	typedef enum _tagStatus
	{
		INIT = 0x00,										//初始化
		AUTHED = 0x01,								//认证通过
		PARAMETERSYNCED = 0x02,	//参数同步完成
		MODEUPLOADED = 0x03,			//SLE运营状态上传完成。
		COMPLETED = 0x04,				    //上位数据同步完成。
	}STATUS;
	STATUS m_status;
};
