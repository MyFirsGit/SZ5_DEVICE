#pragma once
#include "BOMForeService.h"

/**
@brief    RFID写入设定
*/
class CRFIDModeSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRFIDModeSetSvc();
	~CRFIDModeSetSvc();
	int GetCurrentRFIDMode();			//获取当前RFID模式
	void SetRFIDMode(int);			//设定RFID模式

private:
	typedef enum _dialog_group
	{
		DIALOG_GROUP_RFID_MODE_SET = 1,                       
	} DIALOG_GROUP;

	int m_sRfidMode;					//RFID模式（0 - 只读模式，1 - 可写模式）
};