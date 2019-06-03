#pragma once
#include "StdAfx.h"
#include "Service.h"
#include <map>
#include <vector>
#include "CoreExp.h"

using namespace  std;

/**
@brief	�Ի�����ת������                                                                   
*/
class CORE_API CDialogFlow
{
public:
	class CDialogGroup;
    CDialogFlow(CService& service);                                 // ���캯��
    virtual ~CDialogFlow();                                                 // ��������

    virtual void AddDialog(UINT dlgID, CDialog* pDlg);              // ���Dialog

	void AddDialogGroup(UINT dlgGroupID, const TCHAR* pOfficeGuideMsg = _T(""), const TCHAR* pRecpGuideMsg = _T(""), const TCHAR* pRecpGuideMsgEn = _T(""),const TCHAR* pRecpGuideMsgPn = _T("")); // ��ӶԻ�����

    void AddDialogGroupMember(UINT dlgGroupID, UINT dlgID);         // ��ӶԻ��򵽶Ի�����

    void AddFlow(UINT srcDlgGroupID, const TCHAR* pButtonMsg, UINT destDlgGroupID);  // ��ӻ�����ת����
    bool IsFlowButton(const TCHAR* pButtonMsg);                      // �ж��Ƿ���ҵ����ת��ť
    void SetFirstDialogGroup(UINT dlgGroupID);                      // ���õ�һ���Ի�����
    void ShowFirstDialogGroup();                                    // ��ʾ��һ���Ի�����
    void ShowNextDialogGroup(const TCHAR* pButtonMsg);               // ��ʾ��һ���Ի�����
    UINT GetCurDialogGroupID();                                     // ��ȡ��ǰDialog����
    UINT GetPreDialogGroupID();                                     // ��ȡǰ��Dialog����
    UINT GetFirstDialogGroupID();                                   // ��ȡ��һ��Dialog����
    CDialog* GetDialog(UINT dlgID);                                 // ��ȡDialog
    virtual void ShowDialogGroup(UINT dlgGroupID);                  // ��ʾ�Ի�����
    virtual void HideDialogGroup(UINT dlgGroupID);                  // ���ضԻ�����
	void HideAllDialog();                                           // ����ȫ��Dialog

protected:
    virtual void ShowDialog(UINT dlgID);                            // ��ʾDialog
    virtual void HideDialog(UINT dlgID);                            // ����Dialog
//    void HideAllDialog();                                           // ����ȫ��Dialog
    void DelAllDialog();                                            // ɾ��ȫ��Dialog
    void DelAllDialogGroup();                                       // ɾ��ȫ��Dialog��

    bool HasReceptionDlg(UINT dlgGroupID);                          // �Ƿ�����ͨ�˿ͻ��棨����ֹ���񡱡�����ӭ���١����⣩
    bool HasWelcomeReceptionDlg(UINT dlgGroupID);                   // �Ƿ��С�Welcome���˿ͻ���

    UINT GetNextGroupID(const TCHAR* pButtonMsg);                    // ��ȡ��һ���Ի�����ID
    CString CreateFlowKey(UINT dlgGroupID, const TCHAR* pButtonMsg); // ����ҵ����תKey���ڲ��ã�
	 CDialogGroup* GetDialogGroup(UINT dlgGroupID);                 // ���ҶԻ�����

    class CDialogGroup
{
    public:
        CDialogGroup(UINT dlgGroupID, const TCHAR* pGuideMsg, const TCHAR* pRecpGuideMsg, const TCHAR* pRecpGuideMsgEn,const TCHAR* pRecpGuideMsgPn)
        {
            m_groupID = dlgGroupID;
            m_pGuideMsg = pGuideMsg;
            m_pRecpGuideMsg = pRecpGuideMsg;
            m_pRecpGuideMsgEn = pRecpGuideMsgEn;
			m_pRecpGuideMsgPn = pRecpGuideMsgPn;
        };

        ~CDialogGroup() {};

        UINT m_groupID;                                             // Dialog����
        const TCHAR* m_pGuideMsg;                                    // վԱ����Ϣ
        const TCHAR* m_pRecpGuideMsg;                                // �˿�����Ϣ
        const TCHAR* m_pRecpGuideMsgEn;                              // �˿���Ӣ����Ϣ
		const TCHAR* m_pRecpGuideMsgPn;                              // �˿�������������Ϣ


        vector<UINT> m_dlgList;                                     // ���а�����Dialog���
    };

    CService& m_service;                                            // Dialog Flow��Service
    map<UINT, CDialog*> m_dlgMap;                                   // Dialog ID��Dialogӳ��
    map<UINT, CDialogGroup*> m_dlgGroupMap;                         // Dialog Group ID��Dialog Group����ӳ��
    map<CString, UINT> m_flowMap;                                   // ��ת��ťMsg��Dialog Group IDӳ��

    UINT m_firstDlgGroupID;                                         // ��һ����
    UINT m_curDlgGroupID;                                           // ��ǰ����
    UINT m_preDlgGroupID;                                           // ǰ������
};

