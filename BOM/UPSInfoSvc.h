#pragma once
#include "bomforeservice.h"

/**
@brief    ����ҵ�� - Ӳ���Լ�ҵ��
*/
class CUPSInfoSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CUPSInfoSvc();
	~CUPSInfoSvc();

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_UPS_INFO           =  1,                       
	} DIALOG_GROUP;
};