#pragma once
#include "Control.h"
#include "THExp.h"
//#include "THLIBControl.h"

/** 
@brief   THÃüÁî¿ØÖÆ
*/
class TH_API CMIMControl : public CControl
{
public:
	CMIMControl(void);
	~CMIMControl(void);
	//void SetHwnd(HWND hWnd);
	//HWND GetHwnd(void);
    long Initialize();
    long Shutdown(void);

private:
	//HWND m_hWnd;
    //CTHLIBControl* m_pTHLibControl;
};
