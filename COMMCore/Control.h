#pragma once
#include "CoreExp.h"
#include "Command.h"
#include "CoreAPI.h"

class CRequestHandler;
class CORE_API CControl
{
public:
    CControl(void);
    virtual ~CControl(void);
    virtual long Shutdown();
    virtual long Initialize();
	delegate<long (CCommand*)> BeforeDoCommand;
    long DoCommand(CCommand *pRequestItem, long lExecuteMode = SP_EXECMODE_SYNC);
//public:
   // virtual long DoIdleSense(void);
    //void SetIdleTimer(int iTimer);
    //int GetIdleTimer();
    //void SetIdleMode(bool bIdleMode);
    void SetPause(bool isPause);            // …Ë÷√ «∑Ò‘›Õ£RequestManagerµƒMainRequestHandler

private:
    //CRequestManager* m_pCRequestManager;
	CRequestHandler* m_pRequestHandler;
    //int m_iTimer;


};