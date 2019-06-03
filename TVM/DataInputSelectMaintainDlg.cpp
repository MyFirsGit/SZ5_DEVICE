#include "stdafx.h"
#include "DataInputSvc.h"
#include "DataInputSelectMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDataInputSelectMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CDataInputSelectMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataInputSelectMaintainDlg::CDataInputSelectMaintainDlg(CService* pService) : 
COperationDlg(CDataInputSelectMaintainDlg::IDD, pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataInputSelectMaintainDlg::~CDataInputSelectMaintainDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:创建成功    -1:创建失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CDataInputSelectMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct , GUIDE_AREA))
	{
		return -1;
	}

	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LableThree);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_MSGBOX_CONFIRM));
	theAPP_SESSION.SetKeyboard(KEY_ENTER | KEY_RETURN | KEY_PAGE);
	//初始化导出界面
	InitDataInputUI();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::UpdateUI()
{
	Clear();
	UpdateDataInputUI();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::InitDataInputUI()
{
	m_baseInfoArea->SetLableWidth(0,RESOLUTION * 220);
	m_baseInfoArea->SetLableWidth(1,RESOLUTION * 190);
	m_baseInfoArea->SetLableWidth(2,RESOLUTION * 190);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(2,DT_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::UpdateDataInputUI()
{
	CDataInputSvc *pSvc = (CDataInputSvc *)GetService();
	map<WORD,CDataInputSvc::ParameterInputStruct>& m_DataModol = pSvc->GetDataModel();
	map<WORD,CDataInputSvc::ParameterInputStruct>::iterator iter = m_DataModol.begin();

	m_LableThree[0].nameLeft   = _opl(TXT_DATAINPUT_DATA_KIND);
	m_LableThree[0].nameCenter = _opl(TXT_DATAINPUT_LOCAL_VERSION);
	m_LableThree[0].nameRight  = _opl(TXT_DATAINPUT_IMPORT_VERSION);
	// 在线提示不可以导入
	
// 	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
// 		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
// 	}
// 	else{
		// 判断是否插入U盘
		if(!PathIsDirectory(theTVM_INFO.GetUSBDrive())){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
			//return;
		}
		//else if(pSvc->IsUSBAuthed()){
			if(pSvc->IsIndexFileExist()){
				if(pSvc->GetCurrentPage() == pSvc->GetTotalPage()){
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_MSGBOX_CONFIRM_PAGE_UP));
				}
				else if(pSvc->GetCurrentPage() == 1){
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_MSGBOX_CONFIRM));
				}
			}
			else// 提示索引文档不存在
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_ERROR_INDEX_EXIST));
		//}
		//else 
		//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
	//}

	//翻页功能
	int nPreFix = pSvc->GetPagePrefix();
	for (int n = 0;n<nPreFix;n++)
	{
		iter++;
	}
	for (int i = 1;i < _MAX_ROW; i++)
	{	
		if (nPreFix < m_DataModol.size())
		{
			CDataInputSvc::ParameterInputStruct& inputdata = iter->second;
			CString strLocalCurrentVer,strLocalFutureVer,strImportCurrentVer,strImportFutureVer;
			//本地当前版本
			if (inputdata.localCurrentVersion == 0 || inputdata.localCurrentVersion == -1)
			{
				strLocalCurrentVer = _T("--");
			}
			else
			{
				strLocalCurrentVer.Format(_T("%d"),inputdata.localCurrentVersion);
			}
			//本地将来版本
			if (inputdata.localFutureVersion == 0 || inputdata.localFutureVersion == -1)
			{
				strLocalFutureVer = _T("--");
			}
			else
			{
				strLocalFutureVer.Format(_T("%d"),inputdata.localFutureVersion);
			}
			//导入当前版本
			if (inputdata.importCurrentVersion == 0 || inputdata.importCurrentVersion == -1)
			{
				strImportCurrentVer = _T("--");
			}
			else
			{
				strImportCurrentVer.Format(_T("%d"),inputdata.importCurrentVersion);
			}
			//导入将来版本
			if (inputdata.importFutureVersion == 0 || inputdata.importFutureVersion == -1)
			{
				strImportFutureVer = _T("--");
			}
			else
			{
				strImportFutureVer.Format(_T("%d"),inputdata.importFutureVersion);
			}
			//设置数据
			m_LableThree[i].nameLeft   = inputdata.name;
			m_LableThree[i].nameCenter = strLocalCurrentVer  + _T("/") + strLocalFutureVer;
			m_LableThree[i].nameRight  = strImportCurrentVer + _T("/") + strImportFutureVer;
			//下一个数据
			nPreFix++;
			iter++;
		}else
		{
			m_LableThree[i].nameLeft   = _T("");
			m_LableThree[i].nameCenter = _T("");
			m_LableThree[i].nameRight  = _T("");
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清除界面显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::Clear()
{
	for (int i = 0 ; i< _MAX_ROW; i++)
	{
		m_LableThree[i].nameLeft   = _T("");
		m_LableThree[i].nameCenter = _T("");
		m_LableThree[i].nameRight  = _T("");
	}
	m_baseInfoArea->Invalidate();
}