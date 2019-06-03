#pragma once
#include "Panel.h"
#include "lable.h"
class CGroupBox:public CPanel
{
DECLARE_DYNAMIC(CGroupBox)
public:
	//CGroupBox();

	CGroupBox(UIINFO& uIInfo,CString caption=_T(""));

	virtual ~CGroupBox();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); 

	CLable* GetCaption();


	virtual BOOL Create();

private:

	CLable* m_pLblCaption;

};


