#include "stdafx.h"
#include "UDSNSetMaintainDlg.h"
#include "AppSession.h"
#include "TVM.h"
#include "XButton.h"
#include "ExceptionInfo.h"
#include "UDSNSetSvc.h"
#include "guideinfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CUDSNSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CUDSNSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

static CRect editUDSNinfo[] = 
{
	BASE_INFO_RECT(1,10),
	BASE_INFO_RECT(2,10),
};

#define TXT_ACC_UDSN	_opl(UDSNSET_TXT_ACC_UDSN)+_opl(TXT_COLON)  
#define TXT_AFC_UDSN	_opl(UDSNSET_TXT_AFC_UDSN)+_opl(TXT_COLON)  
//static const char* const TXT_ECT_UDSN						 = 	"ECT流水号：";

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetMaintainDlg::CUDSNSetMaintainDlg(CService* pService)
	: COperationDlg(CUDSNSetMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = _opl(TITLE_UDSN_INFO);				   // 标题
	m_baseInfo->detailInfo.row = 2;                                        // 行数
	m_baseInfo->detailInfo.labelGroup = m_labelUDSNinfo;                   // 标签文字
	m_baseInfo->detailInfo.editGroup.cnt = 2;                              // 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editUDSNinfo;              // 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetMaintainDlg::~CUDSNSetMaintainDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CUDSNSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//try
	//{
	//	if (__super::OnCreate(lpCreateStruct, BASE_AREA | BTN_OK | BTN_RESET | GUIDE_AREA) == -1)
	//	{
	//		return -1;
	//	}

	//	// 设置对话框输入限制
	//	GetEdit(0)->SetLimitText(10);
	//	GetEdit(1)->SetLimitText(10);
	//	// 初始化基本区域的label信息
	//	InitLabelUDSNInfo();
	//	// 初始化Edit框里的数据
	//	InitEditTextUDSNInfo();
	//}
	//catch (CSysException& e)
	//{
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...)
	//{
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetMaintainDlg::InitLabelUDSNInfo()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
	{
		// 初始化USDN信息区域文字
		m_labelUDSNinfo[0].nameLeft = add_hsp(TXT_ACC_UDSN, MAX_HALF_CASE_LEFT_LEN);
		m_labelUDSNinfo[1].nameLeft = add_hsp(TXT_AFC_UDSN, MAX_HALF_CASE_LEFT_LEN);
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域EDIT里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetMaintainDlg::InitEditTextUDSNInfo()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
	{
		CString sAccUDSN, sAfcUDSN, sEctUDSN;

		// 获取ACC,AFC和ECT的流水号
		sAccUDSN.Format(_T("%u"),theCOUNT_INFO.GetUDSNForDisplay());
		sAfcUDSN.Format(_T("%u"),theCOUNT_INFO.GetTransactionNumForDsp());

		// 将ACC,AFC和ECT的流水号设置到界面
		SetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN,sAccUDSN);
		SetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN,sAfcUDSN);

		// 设置编辑框焦点
		SetEditFocus(UDSNINFO_EDIT_GROUP_ACCUDSN);
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      把EDIT框里面数据的保存到SVC里面

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetMaintainDlg::SetUDSNInfoToSvc()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
	{
		// 从界面取得流水号信息
		// 将流水号信息写入文件
		pUDSNSetSvc->SetAccUDSN(_tcstoul(GetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN), NULL, 10));
		pUDSNSetSvc->SetAfcUDSN(_tcstoul(GetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN), NULL, 10));
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查EDIT框里的数据是否合法

@param      void

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CUDSNSetMaintainDlg::CheckInputText()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
    {
        // 检查填写的流水号是否合法
        INT64 number = _ttoi64(GetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN));
		if ( number > 0xFFFFFFFF )
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INPUT_ERROR));
			SetEditFocus(UDSNINFO_EDIT_GROUP_ACCUDSN);
			return false;
		}

        number = _ttoi64(GetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN));
        if ( number > 0xFFFFFFFF )
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INPUT_ERROR));
			SetEditFocus(UDSNINFO_EDIT_GROUP_AFCUDSN);
            return false;
        }

       /* number = _ttoi64(GetEditText(UDSNINFO_EDIT_GROUP_ECTUDSN));
        if ( number > 0xFFFFFFFF ){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INPUT_ERROR));
			SetEditFocus(UDSNINFO_EDIT_GROUP_ECTUDSN);
            return false;
        }*/

        return true;

    }
    catch (CSysException)
	{
        throw;
    }
    catch (...)
	{
        throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认、返回、重置 按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CUDSNSetMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	// 初始化编辑框
	InitEditTextUDSNInfo();
	// 设置导航栏
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INFO_INPUT));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CUDSNSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//CString sMsg = (LPCTSTR)lParam;
	//try
	//{		
	//	// 检查输入是否符合要求
	//	if (CheckInputText())
	//	{
	//		// 将文本设置到服务中
	//		SetUDSNInfoToSvc();
	//		//
	//		m_labelUDSNinfo[0].nameRight = add_hsp_right(GetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN), MAX_HALF_CASE_LEFT_LEN);
	//		m_labelUDSNinfo[1].nameRight = add_hsp_right(GetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN), MAX_HALF_CASE_LEFT_LEN);
	//		// 显示设置成功
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_USDN_SET_FINISH));
	//		// 隐藏掉按钮
	//		HideCompoment(BTN_OK | BTN_RESET | EDIT_BOX);
	//		return __super::XButtonClick(wParam,lParam);
	//	}
	//}
	//catch (CSysException& e)
	//{
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...)
	//{
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return 0;
}