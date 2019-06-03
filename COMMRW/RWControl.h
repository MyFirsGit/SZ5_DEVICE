#pragma once
#include "Control.h"
#include "RWExp.h"
//#include "RWLIBControl.h"

/** 
@brief   RWÃüÁî¿ØÖÆ
*/
class RW_API CRWControl : public CControl
{
public:
	CRWControl(void);
	~CRWControl(void);
	//void SetHwnd(HWND hWnd);
	//HWND GetHwnd(void);
    long Initialize();
    long Shutdown(void);

private:
	//HWND m_hWnd;
    //CRWLIBControl* m_pRWLibControl;
};
