#pragma once
#include "BOMForeService.h"

/**
@brief    RFIDд���趨
*/
class CRFIDModeSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRFIDModeSetSvc();
	~CRFIDModeSetSvc();
	int GetCurrentRFIDMode();			//��ȡ��ǰRFIDģʽ
	void SetRFIDMode(int);			//�趨RFIDģʽ

private:
	typedef enum _dialog_group
	{
		DIALOG_GROUP_RFID_MODE_SET = 1,                       
	} DIALOG_GROUP;

	int m_sRfidMode;					//RFIDģʽ��0 - ֻ��ģʽ��1 - ��дģʽ��
};