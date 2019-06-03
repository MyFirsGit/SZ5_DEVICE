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
@brief	CDialogFlow ��CService�ڲ��ã�
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow::~CDialogFlow()
{
    DelAllDialog();
    DelAllDialogGroup();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Dialog

@param      (i) UINT dlgID          Dialog ID
@param      (i) CDialog* pDlg      Dialog

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddDialog(UINT dlgID, CDialog* pDlg)
{
    m_dlgMap[dlgID] = pDlg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡDialog

@param      (i) UINT dlgID          Dialog ID

@retval     CDialog*   Dialogָ�룬δ�ҵ��׳��쳣

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
CDialog* CDialogFlow::GetDialog(UINT dlgID)
{
	map<UINT, CDialog*>::const_iterator foundIter = m_dlgMap.find(dlgID);

    // δ�ҵ����׳��쳣
    if (foundIter == m_dlgMap.end()) {
        throw CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("Dialogδ�ҵ�"));
    }

    return foundIter->second;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾDialog

@param      (i) UINT dlgID          Dialog ID

@retval     ��

@exception  ��
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
@brief      ����Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::HideDialog(UINT dlgID)
{
    CDialog* pDlg = GetDialog(dlgID);
    pDlg->ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȫ��Dialog

@param      ��

@retval     ��

@exception  ��
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
@brief      ɾ��ȫ��Dialog

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ӶԻ�����

@param      (i)UINT dlgGroupID                          �Ի�����ID
@param      (i)const char* pOfficeGuideMsg              վԱ����Ϣ
@param      (i)const char* pRecpGuideMsg                �˿�����Ϣ
@param      (i)const char* pRecpGuideMsgEn              �˿�����Ϣ(Ӣ��)
@param      (i)const char* pRecpGuideMsgPn              �˿�����Ϣ(��������)


@retval     ��

@exception  ��
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
@brief      ���ҶԻ�����

@param      (i)UINT dlgGroupID                          �Ի�����ID

@retval     CDialogGroup*       �Ի�����

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow::CDialogGroup* CDialogFlow::GetDialogGroup(UINT dlgGroupID)
{
    map<UINT, CDialogGroup*>::const_iterator foundIter = m_dlgGroupMap.find(dlgGroupID);

    // δ�ҵ����׳��쳣
    if (foundIter == m_dlgGroupMap.end()) {
        throw CInnerException(m_service.GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("Dialog��δ�ҵ�"));
    }

    return foundIter->second;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ��ȫ��Dialog��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ӶԻ��򵽶Ի�����

@param      (i)UINT dlgGroupID                     �Ի�����ID
@param      (i)UINT dlgID                          �Ի���ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddDialogGroupMember(UINT dlgGroupID, UINT dlgID)
{
    GetDialogGroup(dlgGroupID)->m_dlgList.push_back(dlgID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�Ի�����

@param      (i)UINT dlgGroupID                     �Ի�����ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::ShowDialogGroup(UINT dlgGroupID)
{
    CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);
    // ��ʾ���е�����Dialog
    for(unsigned int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
        ShowDialog(pDlgGroup->m_dlgList[i]);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ضԻ�����

@param      (i)UINT dlgGroupID                     �Ի�����ID

@retval     ��

@exception  ��
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
@brief      ���õ�һ���Ի�����

@param      (i)UINT dlgGroupID                     �Ի�����ID

@retval     ��

@exception  ��
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
@brief      ����ҵ����תKey���ڲ��ã�

@param      (i)UINT dlgGroupID          �Ի�����ID
@param      (i)const char* pButtonMsg   ��ť���·��͵ĸ�����Ϣ

@retval     CString     ҵ����תKey

@exception  ��
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
@brief      ��ӻ�����ת����

@param      (i)UINT srcDlgGroupID       Դ�Ի�����ID
@param      (i)const char* pButtonMsg   ��ť���·��͵ĸ�����Ϣ
@param      (i)UINT destDlgGroupID      Ŀ��Ի�����ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDialogFlow::AddFlow(UINT srcDlgGroupID, const TCHAR* pButtonMsg, UINT destDlgGroupID)
{
    CString flowKey = CreateFlowKey(srcDlgGroupID, pButtonMsg);

    m_flowMap[flowKey] = destDlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��һ���Ի�����

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ȡ��һ���Ի�����ID

@param      (i) const char* pButtonMsg      ��ť��Ϣ

@retval     UINT    ��һ���Ի�����ID(δ�ҵ�ʱ����0)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetNextGroupID(const TCHAR* pButtonMsg)
{
    CString flowKey = CreateFlowKey(m_curDlgGroupID, pButtonMsg);

    map<CString, UINT>::const_iterator foundIter = m_flowMap.find(flowKey);

    // δ�ҵ����׳��쳣
    if (foundIter == m_flowMap.end()) {
        return 0;
    }
    else {
        return foundIter->second;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ���ҵ����ת��ť

@param      (i) const char* pButtonMsg      ��ť��Ϣ

@retval     bool    true:��ҵ����ת��ť   false:����ҵ����ת��ť

@exception  ��
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
@brief      ��ʾ��һ���Ի�����

@param      (i) const char* pButtonMsg      ��ť��Ϣ

@retval     ��

@exception  ��
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
@brief      ��ȡ��ǰDialog����

@param      ��

@retval     UINT, ��ǰDialog����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetCurDialogGroupID()
{
    return m_curDlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡǰ��Dialog����

@param      ��

@retval     UINT, ǰ��Dialog����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetPreDialogGroupID()
{
    return m_preDlgGroupID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��һ��Dialog����

@param      ��

@retval     UINT, ��һ��Dialog����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CDialogFlow::GetFirstDialogGroupID()
{
    return m_firstDlgGroupID;
}
