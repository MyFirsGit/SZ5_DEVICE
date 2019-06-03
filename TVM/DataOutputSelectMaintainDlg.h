#pragma once 
#include "OperationDlg.h"
#include "DataOutputSvc.h"

/**
@brief  数据输出-输出内容选择
*/
class  CDataOutputSelectMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CDataOutputSelectMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDataOutputSelectMaintainDlg(CService* pService) ;
	~CDataOutputSelectMaintainDlg() ;
	enum { IDD = IDD_20131_DATAOUTPUT_SELECT_DLG };
public:
	void UpdateUI();			//更新界面唯一接口

protected:	
	void InitOutputUI();        //初始化界面
	void UpdateOutputData();    //更新界面数据
	void Clear();               //清理界面显示
private:
	LABEL_GROUP     m_LabelGroup[_MAX_ROW];
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);                     //画面创建函数
	afx_msg void  OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);   //画面激活函数
};