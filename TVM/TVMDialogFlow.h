#pragma once

#include "DialogFlow.h"
#include "OperationDlg.h"
#include "ReceptionDlg.h"

class CTVMDialogFlow:public CDialogFlow{
public:
	CTVMDialogFlow(CService& service);                                 // ���캯��
	~CTVMDialogFlow();                                                 // ��������

	void AddDialog(UINT dlgID, CBaseDlg* pDlg);                     // ���Dialog
	CBaseDlg* GetDialog(UINT dlgID);
						
	virtual void ShowDialogGroup(UINT dlgGroupID);                          // ��ʾ�Ի�����
	virtual void HideDialogGroup(UINT dlgGroupID);                          // ���ضԻ�����
	COperationDlg* GetCurOperationDlg();
	CReceptionDlg* GetCurReceptionDlg();
	void NotifyCurrentDialogGroupUpdate();										    //֪ͨ��ǰ������Ĵ������
	bool HasReceptionDlg(UINT dlgGroupID);
	bool HasWelcomeReceptionDlg(UINT dlgGroupID);

	const TCHAR* GetCurrentGuideMsg();									// ��ȡ��ǰ����Ա����������

protected:
	virtual void ShowDialog(UINT dlgID);                                    // ��ʾDialog
	virtual void HideDialog(UINT dlgID);                                    // ����Dialog
private:
	void OnServiceStateChanged(SERVICE_STATE oldState,SERVICE_STATE newState);
};