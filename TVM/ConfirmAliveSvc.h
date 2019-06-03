#pragma once
#include "BackService.h"


/**
@brief 定期集计Service
*/
class CConfirmAliveSvc :	public CBackService
{
	DECLARE_MESSAGE_MAP()

public:
	CConfirmAliveSvc(void);
	~CConfirmAliveSvc(void);
protected:
	void OnStart();
	bool OnStop();
private:
	enum ENUM_TIMER {
		CONFIRM_ALIVE_TIMER = 1, 
	};
	void OnTimer(UINT nEventID);
	HANDLE m_hConfirmAlive;
};