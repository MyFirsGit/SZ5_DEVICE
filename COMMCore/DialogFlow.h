#pragma once
#include "StdAfx.h"
#include "Service.h"
#include <map>
#include <vector>
#include "CoreExp.h"

using namespace  std;

/**
@brief	对话框流转控制类                                                                   
*/
class CORE_API CDialogFlow
{
public:
	class CDialogGroup;
    CDialogFlow(CService& service);                                 // 构造函数
    virtual ~CDialogFlow();                                                 // 析构函数

    virtual void AddDialog(UINT dlgID, CDialog* pDlg);              // 添加Dialog

	void AddDialogGroup(UINT dlgGroupID, const TCHAR* pOfficeGuideMsg = _T(""), const TCHAR* pRecpGuideMsg = _T(""), const TCHAR* pRecpGuideMsgEn = _T(""),const TCHAR* pRecpGuideMsgPn = _T("")); // 添加对话框组

    void AddDialogGroupMember(UINT dlgGroupID, UINT dlgID);         // 添加对话框到对话框组

    void AddFlow(UINT srcDlgGroupID, const TCHAR* pButtonMsg, UINT destDlgGroupID);  // 添加画面跳转流程
    bool IsFlowButton(const TCHAR* pButtonMsg);                      // 判断是否是业务流转按钮
    void SetFirstDialogGroup(UINT dlgGroupID);                      // 设置第一个对话框组
    void ShowFirstDialogGroup();                                    // 显示第一个对话框组
    void ShowNextDialogGroup(const TCHAR* pButtonMsg);               // 显示下一个对话框组
    UINT GetCurDialogGroupID();                                     // 获取当前Dialog组编号
    UINT GetPreDialogGroupID();                                     // 获取前次Dialog组编号
    UINT GetFirstDialogGroupID();                                   // 获取第一个Dialog组编号
    CDialog* GetDialog(UINT dlgID);                                 // 获取Dialog
    virtual void ShowDialogGroup(UINT dlgGroupID);                  // 显示对话框组
    virtual void HideDialogGroup(UINT dlgGroupID);                  // 隐藏对话框组
	void HideAllDialog();                                           // 隐藏全部Dialog

protected:
    virtual void ShowDialog(UINT dlgID);                            // 显示Dialog
    virtual void HideDialog(UINT dlgID);                            // 隐藏Dialog
//    void HideAllDialog();                                           // 隐藏全部Dialog
    void DelAllDialog();                                            // 删除全部Dialog
    void DelAllDialogGroup();                                       // 删除全部Dialog组

    bool HasReceptionDlg(UINT dlgGroupID);                          // 是否有普通乘客画面（“终止服务”、“欢迎光临”除外）
    bool HasWelcomeReceptionDlg(UINT dlgGroupID);                   // 是否有“Welcome”乘客画面

    UINT GetNextGroupID(const TCHAR* pButtonMsg);                    // 获取下一个对话框组ID
    CString CreateFlowKey(UINT dlgGroupID, const TCHAR* pButtonMsg); // 生成业务流转Key（内部用）
	 CDialogGroup* GetDialogGroup(UINT dlgGroupID);                 // 查找对话框组

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

        UINT m_groupID;                                             // Dialog组编号
        const TCHAR* m_pGuideMsg;                                    // 站员向导信息
        const TCHAR* m_pRecpGuideMsg;                                // 乘客向导信息
        const TCHAR* m_pRecpGuideMsgEn;                              // 乘客向导英文信息
		const TCHAR* m_pRecpGuideMsgPn;                              // 乘客向导葡萄牙文信息


        vector<UINT> m_dlgList;                                     // 组中包含的Dialog编号
    };

    CService& m_service;                                            // Dialog Flow的Service
    map<UINT, CDialog*> m_dlgMap;                                   // Dialog ID与Dialog映射
    map<UINT, CDialogGroup*> m_dlgGroupMap;                         // Dialog Group ID与Dialog Group对象映射
    map<CString, UINT> m_flowMap;                                   // 跳转按钮Msg与Dialog Group ID映射

    UINT m_firstDlgGroupID;                                         // 第一组编号
    UINT m_curDlgGroupID;                                           // 当前组编号
    UINT m_preDlgGroupID;                                           // 前次组编号
};

