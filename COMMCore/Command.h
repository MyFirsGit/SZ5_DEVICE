#pragma once
#include "UtilObject.h"
#include "CoreExp.h"
#include "Service.h"
#include "Bag.h"
#include "CoreAPI.h"
#include "delegate.h"

class CService;
class CORE_API CCommand :
	public CUtilObject
{
public:
	CCommand(void);
	virtual ~CCommand(void);
	unsigned long GetTimeOut();
	void SetTimeOut(unsigned long ulTimeOut);
	BOOL IsCompleted();
	delegate<void (CCommand*)> Executing;				  //当前执行之前引发的事件
	long Execute();
	delegate<void(CCommand*)> Completing;
	delegate<void (CCommand*)> Completed;				  //当前命令完成的事件
	void OnComplete(long lCompleteCode);
	long GetResultCode();
	void SetCommandID(unsigned long ulCmdID);
	unsigned long GetCommandID(void);
	void SetAutoDeleted(bool bAutoDeleted = TRUE);
	bool GetAutoDeleted();
    void SetService(CService* pService);
    CService* GetService();
	void SetNeedCallbackService(bool bNeedCallbackService = true);
	bool GetNeedCallbackService();
	// implemented later.
	
	virtual long IsValidCommand();
	
	CBag* GetBag();

protected:
    bool IsTimeOut(unsigned long ulStartTime);
	virtual long ExecuteCommand() = 0;

private:
	void SetCompleted(long lCompletedCode);
	BOOL IsValidCommandID();
	void EnsureHandle();
	CBag* m_Bag;
	long m_lCompletedCode;
	bool m_bCompleted;
	bool m_bAutoDeleted;
	bool m_bNeedCallbackService;
	unsigned long m_ulCommandID;
	unsigned long m_ulTimeOut;
    CService* m_Service;
	HANDLE m_Handle;
	friend long WaitForCommandComplete(CCommand* pRequestItem,DWORD waitForSeconds);
	friend class CControl;
};

long WaitForCommandComplete(CCommand* pRequestItem,DWORD waitForSeconds = INFINITE);
