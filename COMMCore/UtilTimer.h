#pragma once
#include "CoreExp.h"

class CORE_API CUtilTimer
{
public:
	CUtilTimer(void);
	~CUtilTimer(void);

public:
	void SetTimeOut(const DWORD& dwTimeOut);
	CUtilTimer(const DWORD& dwTimeOut);
	BOOL isTimeOut();
private:
	DWORD m_dwLastTime;
	DWORD m_dwTimeOut;

public:
	void Reset(void);
};
