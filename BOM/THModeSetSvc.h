#pragma once
#include "BOMForeService.h"

/**
@brief    ����ҵ�� - THģʽ�趨ҵ��
*/
class CTHModeSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTHModeSetSvc();
	~CTHModeSetSvc();	
	int GetCurrentTHMode();			//��ȡ��ǰTHģʽ
	void SetTHMode(int);			//�趨THģʽ

private:
	typedef enum _dialog_group
	{
		DIALOG_GROUP_TH_MODE_SET = 1,                       
	} DIALOG_GROUP;
};