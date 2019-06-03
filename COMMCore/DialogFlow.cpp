#include "StdAfx.h"
#include "DialogFlow.h"
#include "InnerException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/**
@brief	CDialogFlow （CService内部用）
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow::CDialogFlow(CService& service)
    :m_service(service)
    ,m_firstDlgGroupID(-1)
    ,m_curDlgGroupID(-1)
    ,m_preDlgGroupID(-1)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow::~CDialogFlow()
{
    DelAllDialog();
    DelAllDialogGroup();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加Dialog

@param      (i) UINT dlgID          Dialog ID
@param      (i) CDialog* pDlg      Dialog

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddDialog(UINT dlgID, CDialog* pDlg)
{
    m_dlgMap[dlgID] = pDlg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     CDialog*   Dialog指针，未找到抛出异常

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
CDialog* CDialogFlow::GetDialog(UINT dlgID)
{
	map<UINT, CDialog*>::const_iterator foundIter = m_dlgMap.find(dlgID);

    // 未找到则抛出异常
    if (foundIter == m_dlgMap.end()) {
        throw CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("Dialog未找到"));
    }

    return foundIter->second;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::ShowDialog(UINT dlgID)
{
    try {
        CDialog* pDlg = GetDialog(dlgID);
        pDlg->ShowWindow(SW_SHOW);
        pDlg->PostMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
    }
    catch (CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::HideDialog(UINT dlgID)
{
    CDialog* pDlg = GetDialog(dlgID);
    pDlg->ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏全部Dialog

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::HideAllDialog()
{
    for (map<UINT, CDialog*>::iterator dlgIter = m_dlgMap.begin(); dlgIter != m_dlgMap.end( ); dlgIter++) {
        CDialog* pDlg = dlgIter->second;
        if (pDlg!=NULL && IsWindow(pDlg->m_hWnd)) {
            pDlg->ShowWindow(SW_HIDE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除全部Dialog

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::DelAllDialog()
{
    for (map<UINT, CDialog*>::iterator dlgIter = m_dlgMap.begin(); dlgIter != m_dlgMap.end( ); dlgIter++) {
        CDialog* pDlg = dlgIter->second;

        if (pDlg != NULL) {
            if (pDlg->GetSafeHwnd() != NULL) {
                pDlg->DestroyWindow();
            }
            delete pDlg;
            pDlg = NULL;
        }
    }

    m_dlgMap.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加对话框组

@param      (i)UINT dlgGroupID                          对话框组ID
@param      (i)const char* pOfficeGuideMsg              站员向导消息
@param      (i)const char* pRecpGuideMsg                乘客向导消息
@param      (i)const char* pRecpGuideMsgEn              乘客向导消息(英文)
@param      (i)const char* pRecpGuideMsgPn              乘客向导消息(葡萄牙文)


@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddDialogGroup(UINT dlgGroupID, const TCHAR* pOfficeGuideMsg, const TCHAR* pRecpGuideMsg, const TCHAR* pRecpGuideMsgEn, const TCHAR* pRecpGuideMsgPn)
{
	CDialogGroup* pDlgGroup = NULL;

	pDlgGroup = new CDialogGroup(dlgGroupID, pOfficeGuideMsg, pRecpGuideMsg, pRecpGuideMsgEn,pRecpGuideMsgPn);

	if(NULL != pDlgGroup)m_dlgGroupMap[dlgGroupID] = pDlgGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      查找对话框组

@param      (i)UINT dlgGroupID                          对话框组ID

@retval     CDialogGroup*       对话框组

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow::CDialogGroup* CDialogFlow::GetDialogGroup(UINT dlgGroupID)
{
    map<UINT, CDialogGroup*>::const_iterator foundIter = m_dlgGroupMap.find(dlgGroupID);

    // 未找到则抛出异常
    if (foundIter == m_dlgGroupMap.end()) {
        throw CInnerException(m_service.GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("Dialog组未找到"));
    }

    return foundIter->second;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除全部Dialog组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::DelAllDialogGroup()
{
    UINT dlgGroupID = 0;
    CDialogGroup* pDlgGroup = NULL;

    for (map<UINT, CDialogGroup*>::iterator groupIter = m_dlgGroupMap.begin(); groupIter != m_dlgGroupMap.end( ); groupIter++) {
        pDlgGroup = groupIter->second;
        delete pDlgGroup;
    }

    m_dlgGroupMap.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加对话框到对话框组

@param      (i)UINT dlgGroupID                     对话框组ID
@param      (i)UINT dlgID                          对话框ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddDialogGroupMember(UINT dlgGroupID, UINT dlgID)
{
    GetDialogGroup(dlgGroupID)->m_dlgList.push_back(dlgID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示对话框组

@param      (i)UINT dlgGroupID                     对话框组ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::ShowDialogGroup(UINT dlgGroupID)
{
    CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);
    // 显示组中的所有Dialog
    for(unsigned int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
        ShowDialog(pDlgGroup->m_dlgList[i]);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏对话框组

@param      (i)UINT dlgGroupID                     对话框组ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::HideDialogGroup(UINT dlgGroupID)
{
    CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);

    for(unsigned int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
        HideDialog(pDlgGroup->m_dlgList[i]);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置第一个对话框组

@param      (i)UINT dlgGroupID                     对话框组ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::SetFirstDialogGroup(UINT dlgGroupID)
{
    m_firstDlgGroupID = dlgGroupID;
    m_curDlgGroupID = dlgGroupID;
    m_preDlgGroupID = dlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      生成业务流转Key（内部用）

@param      (i)UINT dlgGroupID          对话框组ID
@param      (i)const char* pButtonMsg   按钮按下发送的附加消息

@retval     CString     业务流转Key

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CDialogFlow::CreateFlowKey(UINT dlgGroupID, const TCHAR* pButtonMsg)
{
    CString flowKey = _T("");
    flowKey.Format(_T("%d%s"), dlgGroupID, (LPCTSTR)pButtonMsg);

    return flowKey;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加画面跳转流程

@param      (i)UINT srcDlgGroupID       源对话框组ID
@param      (i)const char* pButtonMsg   按钮按下发送的附加消息
@param      (i)UINT destDlgGroupID      目标对话框组ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddFlow(UINT srcDlgGroupID, const TCHAR* pButtonMsg, UINT destDlgGroupID)
{
    CString flowKey = CreateFlowKey(srcDlgGroupID, pButtonMsg);

    m_flowMap[flowKey] = destDlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示第一个对话框组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::ShowFirstDialogGroup()
{
    if (m_firstDlgGroupID != -1) {
		SetFirstDialogGroup(m_firstDlgGroupID);
        ShowDialogGroup(m_firstDlgGroupID);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取下一个对话框组ID

@param      (i) const char* pButtonMsg      按钮信息

@retval     UINT    下一个对话框组ID(未找到时返回0)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetNextGroupID(const TCHAR* pButtonMsg)
{
    CString flowKey = CreateFlowKey(m_curDlgGroupID, pButtonMsg);

    map<CString, UINT>::const_iterator foundIter = m_flowMap.find(flowKey);

    // 未找到则抛出异常
    if (foundIter == m_flowMap.end()) {
        return 0;
    }
    else {
        return foundIter->second;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否是业务流转按钮

@param      (i) const char* pButtonMsg      按钮信息

@retval     bool    true:是业务流转按钮   false:不是业务流转按钮

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CDialogFlow::IsFlowButton(const TCHAR* pButtonMsg)
{
    if (GetNextGroupID(pButtonMsg) == 0) {
        return false;
    }
    else {
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示下一个对话框组

@param      (i) const char* pButtonMsg      按钮信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::ShowNextDialogGroup(const TCHAR* pButtonMsg)
{
    m_preDlgGroupID = m_curDlgGroupID;
    m_curDlgGroupID = GetNextGroupID(pButtonMsg);

    HideDialogGroup(m_preDlgGroupID);
    ShowDialogGroup(m_curDlgGroupID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前Dialog组编号

@param      无

@retval     UINT, 当前Dialog组编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetCurDialogGroupID()
{
    return m_curDlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取前次Dialog组编号

@param      无

@retval     UINT, 前次Dialog组编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetPreDialogGroupID()
{
    return m_preDlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取第一个Dialog组编号

@param      无

@retval     UINT, 第一个Dialog组编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetFirstDialogGroupID()
{
    return m_firstDlgGroupID;
}
