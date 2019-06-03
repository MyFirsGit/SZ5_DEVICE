#pragma once

#include "OperationDlg.h"
#include "CSaleStaticSvc.h"

/**
@brief     销售统计基本画面
*/

class CSaleStaticBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSaleStaticBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSaleStaticBaseOfficeDlg(CService* pService) ;
	~CSaleStaticBaseOfficeDlg() ;

	enum { IDD = IDD_01701_SALE_STATIC_DLG };

	//PLABEL_GROUP_THREE m_baseInfo;
private:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDraw(CDC* pDC);
};