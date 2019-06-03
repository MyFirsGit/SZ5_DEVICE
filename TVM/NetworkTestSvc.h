#pragma once
#include "tvmforeservice.h"
#include "FTPCommand.h"

/**
@brief    ����ҵ�� - Ӳ���Լ�ҵ��
*/
class CNetworkTestSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CNetworkTestSvc();
	virtual ~CNetworkTestSvc();

	BOOL PingSC(char* pBuff);										// ping��λ������
	static const int MAX_PING_RESULT_LLENGTH = 2000;				// �����Ϣ��󳤶�
public:
	virtual void NotifyDataModelChanged();
private:
	typedef enum _dialog_group 
	{
		DIALOG_GROUP_NETWORK   =  1,                                // ������Ի�����
	} DIALOG_GROUP;

private:
	//��Ϣ����
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //���� [Enter] ��������Ϣ��ִ�У�
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //���� [F3/Ctrl]    ��������Ϣ�����ˣ�
	void OnStart();
};