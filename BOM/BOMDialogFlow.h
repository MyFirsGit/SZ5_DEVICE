#pragma once

#include "DialogFlow.h"
#include "OperationDlg.h"

class CBOMDialogFlow:public CDialogFlow{
public:
	CBOMDialogFlow(CService& service);                                 // ���캯��
	~CBOMDialogFlow();                                                 // ��������

	void AddDialog(UINT dlgID, CBaseDlg* pDlg);                     // ���Dialog
	CBaseDlg* GetDialog(UINT dlgID);
						
	virtual void ShowDialogGroup(UINT dlgGroupID);                          // ��ʾ�Ի�����
	virtual void HideDialogGroup(UINT dlgGroupID);                          // ���ضԻ�����
	COperationDlg* GetCurOperationDlg();
	void NotifyCurrentDialogGroupUpdate();										    //֪ͨ��ǰ������Ĵ������
	bool HasReceptionDlg(UINT dlgGroupID);
	bool HasWelcomeReceptionDlg(UINT dlgGroupID);

protected:
	virtual void ShowDialog(UINT dlgID);                                    // ��ʾDialog
	virtual void HideDialog(UINT dlgID);                                    // ����Dialog
private:
	void OnServiceStateChanged(SERVICE_STATE oldState,SERVICE_STATE newState);
};