#pragma once
#include "bomforeservice.h"
#include "MoneyBoxTestMaintainDlg.h"

/**
@brief    ����ҵ�� - Ӳ���Լ�ҵ��
*/
class CMoneyBoxTestSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CMoneyBoxTestSvc();
	~CMoneyBoxTestSvc();

	void OnOK();

private:
	//DeviceTestSvc�� DialogGroup, ֮������Ϊpublic, ����ΪUpdateDialogFlow����������Ҫ�����ö�ٳ�����Ϊ�������
	typedef enum _dialog_group {
		DIALOG_GROUP_MONEYBOX           =  1,                          // Ǯ�����ѡ��
	} DIALOG_GROUP;
};