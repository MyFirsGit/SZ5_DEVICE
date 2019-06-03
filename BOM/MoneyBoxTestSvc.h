#pragma once
#include "bomforeservice.h"
#include "MoneyBoxTestMaintainDlg.h"

/**
@brief    保守业务 - 硬件自检业务
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
	//DeviceTestSvc的 DialogGroup, 之所以设为public, 是因为UpdateDialogFlow方法里面需要传入该枚举常量做为传入参数
	typedef enum _dialog_group {
		DIALOG_GROUP_MONEYBOX           =  1,                          // 钱箱测试选择
	} DIALOG_GROUP;
};