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
		INIT = 0x00,										//��ʼ��
		AUTHED = 0x01,								//��֤ͨ��
		PARAMETERSYNCED = 0x02,	//����ͬ�����
		MODEUPLOADED = 0x03,			//SLE��Ӫ״̬�ϴ���ɡ�
		COMPLETED = 0x04,				    //��λ����ͬ����ɡ�
	}STATUS;
	STATUS m_status;
};
