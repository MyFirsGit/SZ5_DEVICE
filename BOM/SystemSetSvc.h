#pragma once
#include "bomforeservice.h"
#include "UIDefinition.h"

#include "SvcDef.h"		//rhd
//#include "CPSCOMLIBAPI.h"

/**
@brief    ����ҵ�� - Ӳ���Լ�ҵ��
*/
class CSystemSetSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSystemSetSvc();
	~CSystemSetSvc();

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_SYSTEM_SET                     =  1,                          // �豸�Լ�ѡ��
	} DIALOG_GROUP;



};