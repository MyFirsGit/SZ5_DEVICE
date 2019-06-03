#pragma once
#include "bomforeservice.h"

/**
@brief    保守业务 - 硬件自检业务
*/
class CPrinterTestSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestSvc();
	~CPrinterTestSvc();

	void PrinterPrint();			// 打印
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_PRINT           =  1,                          // 钱箱测试选择
	} DIALOG_GROUP;
};