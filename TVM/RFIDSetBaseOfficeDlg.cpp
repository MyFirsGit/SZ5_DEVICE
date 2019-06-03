#include "stdafx.h"
#include "RFIDSetBaseOfficeDlg.h"
#include "RFIDSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 初始化常量字符串
static const TCHAR* const STR_READONLY_MODE   =  _T("禁用模式");
static const TCHAR* const STR_WRITABLE_MODE   =  _T("使用模式");

static const TCHAR* const TITLE_SELECT_RFID_MODE = _T("请选择RFID读写模式");

static const TCHAR* const TXT_CURRENT_MODE = _T("当前模式：");
static const TCHAR* const TXT_SWITCH_MODE  = _T("选择模式：");

static const TCHAR* const WM_BTN_READONLY_MODE = _T("ReadOnly");
static const TCHAR* const WM_BTN_WRITABLE_MODE  = _T("Writable");


IMPLEMENT_DYNAMIC(CRFIDSetBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRFIDSetBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnModeItemClick)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*     pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRFIDSetBaseOfficeDlg::CRFIDSetBaseOfficeDlg(CService* pService)
: COperationDlg(CRFIDSetBaseOfficeDlg::IDD, pService)
{
	//m_baseInfo->titleInfo.titleName = TITLE_SELECT_RFID_MODE;			// 标题
	//m_baseInfo->detailInfo.row = 2;									// 行数
	//m_baseInfo->detailInfo.labelGroup = m_lableRFIDModeInfo;          // 标签文字（2列）

	//// 初始化普通模式相关数据
	//BTN_INFO readOnlyMode;
	//readOnlyMode.btnMsg = WM_BTN_READONLY_MODE;
	//readOnlyMode.isVisible = true;
	//readOnlyMode.text= STR_READONLY_MODE;
	//readOnlyMode.type = BUTTON_TYPE_NORMAL;
	//m_vec_Right_1_BTN_INFO.push_back(readOnlyMode);

	//// 初始化缩退模式相关数据
	//BTN_INFO writableMode;
	//writableMode.btnMsg = WM_BTN_WRITABLE_MODE;
	//writableMode.isVisible = true;
	//writableMode.text = STR_WRITABLE_MODE;
	//readOnlyMode.type = BUTTON_TYPE_NORMAL;
	//m_vec_Right_1_BTN_INFO.push_back(writableMode);

	//m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRFIDSetBaseOfficeDlg::~CRFIDSetBaseOfficeDlg()
{
	//m_Right_1_Group->ShowButtons(true);
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
int CRFIDSetBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//try{
	//	// 创建基本信息区域、返回、确定按钮和功能按钮区域
	//	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA | BTN_OK )){
	//		return -1;
	//	}

	//	// 初始化模式信息
	//	InitRFIDModeInfo();
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRFIDSetBaseOfficeDlg::OnModeItemClick(WPARAM wParam,LPARAM lParam)
{
	//CRFIDModeSetSvc* pRFIDModeSetSvc = (CRFIDModeSetSvc*)GetService();
	//try{
	//	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	//	// 获取当前RFID模式
	//	int sRFIDMode = theSETTING.GetRFIDWriteStatus();

	//	// 显示导航栏
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));

	//	if (btnInfo->btnMsg == WM_BTN_READONLY_MODE){	// 禁用模式按钮按下
	//		m_lableRFIDModeInfo[1].nameRight = STR_READONLY_MODE;

	//		if (0 == sRFIDMode){	// 如果当前模式为禁用模式
	//			m_btnOK->SetEnabled(false);
	//		}
	//		else{	// 如果当前模式为使用模式				
	//			m_btnOK->SetEnabled(true);
	//		}
	//	}
	//	else if (btnInfo->btnMsg == WM_BTN_WRITABLE_MODE){	// 使用模式按钮按下
	//		m_lableRFIDModeInfo[1].nameRight = STR_WRITABLE_MODE;

	//		if (1 == sRFIDMode){	// 如果当前模式为使用模式
	//			m_btnOK->SetEnabled(false);
	//		}
	//		else{	// 如果当前模式为禁用模式
	//			m_btnOK->SetEnabled(true);
	//		}
	//	}
	//	//显示基本客户区域
	//	ShowCompoment(BASE_AREA);
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(pRFIDModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRFIDSetBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//CRFIDModeSetSvc* pRFIDModeSetSvc = (CRFIDModeSetSvc*)GetService();
	//try{
	//	// 获取当前选择的模式按钮
	//	const vector<BTN_INFO>& selectBtnInfos = m_Right_1_Group->GetSelectedBtnInfo();
	//	// 如果成功获取到模式
	//	if(selectBtnInfos.size() == 1){
	//		const BTN_INFO& selectBtnInfo = selectBtnInfos.at(0);
	//		// 判断获取到的RFID模式
	//		int sTHMode = selectBtnInfo.btnMsg == WM_BTN_READONLY_MODE ? 0 : 1;
	//		// 设置当前模式
	//		pRFIDModeSetSvc->SetRFIDMode(sTHMode);
	//		// 重新获取当前模式
	//		int newMode = pRFIDModeSetSvc->GetCurrentRFIDMode();
	//		// 显示当前模式
	//		if (0 == newMode){
	//			m_lableRFIDModeInfo[0].nameRight= STR_READONLY_MODE;
	//		}
	//		else{
	//			m_lableRFIDModeInfo[0].nameRight= STR_WRITABLE_MODE;
	//		}

	//		// 显示设置成功，隐藏掉按钮
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_FINISH));
	//		HideCompoment(BTN_OK);
	//		m_Right_1_Group->ShowButtons(false);

	//		// 刷新区域
	//		m_baseInfoArea->Invalidate();
	//	}
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(pRFIDModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));
	//}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      InitTHModeInfo

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CRFIDSetBaseOfficeDlg::InitRFIDModeInfo()
{
	CRFIDModeSetSvc* pRFIDModeSetSvc = (CRFIDModeSetSvc*)GetService();
	try{	
		// 初始化模式切换信息
		m_lableRFIDModeInfo[0].nameLeft = add_hsp(TXT_CURRENT_MODE,MAX_HALF_CASE_LEFT_LEN);
		m_lableRFIDModeInfo[1].nameLeft = add_hsp(TXT_SWITCH_MODE,MAX_HALF_CASE_LEFT_LEN);

		// 根据当前的RFID模式，设置基本信息
		int sRFIDMode = pRFIDModeSetSvc->GetCurrentRFIDMode();
		// 如果sRFIDMode等于0，设置为禁用模式；等于1，设置为使用模式
		if (0 == sRFIDMode){
			m_lableRFIDModeInfo[0].nameRight = STR_READONLY_MODE;
		}
		else{
			m_lableRFIDModeInfo[0].nameRight = STR_WRITABLE_MODE;
		}
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pRFIDModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}