#include "stdafx.h"
#include "TimeSetBaseOfficeDlg.h"
#include "TimeSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTimeSetBaseOfficeDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CTimeSetBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::CTimeSetBaseOfficeDlg(CService* pService)
	:COperationDlg(CTimeSetBaseOfficeDlg::IDD, pService)
{
	m_baseInfo->detailInfo.labelGroup = m_labelGroup;           
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::~CTimeSetBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int  0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTimeSetBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)){
		return -1;
	}
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, RESOLUTION * 200);				// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, RESOLUTION * 400);				// 设置第二列宽度

	//InitlabelTimeInfo();											// 初始化BASE区域LABEL里面的数据
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      画面激活

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelTimeInfo();											// 初始化BASE区域LABEL里面的数据
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::InitlabelTimeInfo()
{
	_DATE_TIME currentTime = ComGetCurTime();
	m_labelGroup[0].nameLeft = _T("[1] ")+_opl(TTILE_SET_TIME);
	m_labelGroup[0].nameRight = currentTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::UpdateUI()
{
	InitlabelTimeInfo();
	CTimeSetSvc* pSvc = (CTimeSetSvc*)GetService();
	CTimeSetSvc::Model& model = pSvc->GetDataModel();
	if(1 == model.TimeChange)
	{
		ShowCodeContent();			//基本信息区显示对应项设置的数据
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      基本信息区显示对应项设置的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CTimeSetBaseOfficeDlg::ShowCodeContent()
{
	CTimeSetSvc* pSvc = (CTimeSetSvc*)GetService();
	CTimeSetSvc::Model& model = pSvc->GetDataModel();
	if(FALSE == model.TimeLegal)
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_COMMUNICATION_SET_INPUT_ERROR);
		return ;
	}
	else
	{
		CString TimeStr = _T("");
		TimeStr.Format(_T("%s-%s-%s %s:%s:%s"),model.setYear,model.setMonth,model.setDay,model.setHour,model.setMinute,model.setSecond);
		m_labelGroup[0].nameRight = TimeStr;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

