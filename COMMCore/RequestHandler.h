#pragma once
#include "UtilThread.h"
#include "UtilTimer.h"
#include "UtilList.h"
#include "Command.h"
#include "CoreExp.h"

class CORE_API CRequestHandler :
	public CUtilThread
{
	DECLARE_DYNCREATE(CRequestHandler)
    DECLARE_MESSAGE_MAP()

public:
	CRequestHandler(void);
	~CRequestHandler(void);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
    void SetPause(bool isPause);

private:
    virtual BOOL OnIdle(LONG lCount);
    void OnRequestQueue(WPARAM wParam, LPARAM lParam);

	DWORD LockCommunication(DWORD dwTimeOut);
	BOOL UnLockCommunication();
	void OnRequestItemCompleted(CCommand*);

private:
	BOOL m_bTimeOut;
	CUtilTimer m_oCUtilTimer;
	HANDLE m_hServiceMutex;

    CCommand* m_pRequestItem;         // 当前正在执行的命令

    CUtilList	m_requestList;				// 待执行的命令队列
    bool        m_isPause;                  // 是否暂停
};
