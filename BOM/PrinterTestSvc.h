#pragma once
#include "bomforeservice.h"

/**
@brief    ����ҵ�� - Ӳ���Լ�ҵ��
*/
class CPrinterTestSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestSvc();
	~CPrinterTestSvc();

	void PrinterPrint();			// ��ӡ
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_PRINT           =  1,                          // Ǯ�����ѡ��
	} DIALOG_GROUP;
};