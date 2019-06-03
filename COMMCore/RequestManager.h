#pragma once
#include "UtilThread.h"
#include "UtilTimer.h"
#include "Control.h"
#include "RequestHandler.h"
#include "Afxtempl.h"
#include <windef.h>
#include "CoreExp.h"

//
//typedef CRequestHandler * LPCRequestHandler;
//typedef CMap<DWORD,DWORD,LPCRequestHandler,LPCRequestHandler> CMapDWORDToHandle;
//
//class CORE_API CRequestManager :
//	public CUtilThread
//{
//	DECLARE_DYNCREATE(CRequestManager)
//	DECLARE_MESSAGE_MAP()
//public:
//
//	CRequestManager(void);
//	virtual ~CRequestManager(void);
//	virtual BOOL InitInstance();
//	virtual int ExitInstance();
//public:
//    void SetController(CControl * pController);
//    void SetIdleMode(bool bIdleMode);
//    void SetPause(bool isPause);
//
//private:
//    //virtual BOOL OnIdle(LONG lCount);
//	void OnRequestQueue(WPARAM wParam, LPARAM lParam);
//	void OnRequestComplete(WPARAM wParam, LPARAM lParam);
//
//    CRequestHandler* CreateRequestHandler();
//	void DeleteRequestHandler(DWORD dwThreadID);
//    void DeleteAllRequestHandler(void);
//	
//private:
//	LPCRequestHandler m_pMainRequestHandler;		// main queue thread
//	CMapDWORDToHandle	m_mapRequestHandler;		// RequestHandler list
//	bool				m_bIdleMode;				// Mode Idle flag
//	CControl*	        m_pController;				// SP Control pointer
//	CUtilTimer			m_oCUtilTimer;
//
//};
