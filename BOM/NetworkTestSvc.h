#pragma once
#include "bomforeservice.h"
#include "FTPCommand.h"

/**
@brief    ����ҵ�� - Ӳ���Լ�ҵ��
*/
class CNetworkTestSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CNetworkTestSvc();
	~CNetworkTestSvc();

	BOOL PingSC(char* pBuff);										// ping��λ������
	static const int MAX_PING_RESULT_LLENGTH = 2000;				// �����Ϣ��󳤶�
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_NETWORK           =  1,                        // Ǯ�����ѡ��
	} DIALOG_GROUP;
};