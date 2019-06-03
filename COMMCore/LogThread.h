#pragma once
#include "UtilThread.h"
#include <list>
#include "Log.h"

#define WM_USER_LOG_ENQUEUE (WM_USER + 100)

class  CLogThread : public CUtilThread
{
	DECLARE_DYNCREATE(CLogThread)
	DECLARE_MESSAGE_MAP()
public:
	CLogThread();
	~CLogThread();

	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
protected:
private:
	virtual BOOL OnIdle(long lCount);
	void OnRequestQueue(WPARAM wp, LPARAM lp);
	list<pair<CLog*,CString>*> m_pRequestList;
	
};